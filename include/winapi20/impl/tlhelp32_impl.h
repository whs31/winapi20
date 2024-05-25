#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>
#include <iosfwd>
#include <winapi20/detail/export.h>
#include <winapi20/detail/definitions.h>
#include <winapi20/detail/template_util.h>

typedef struct tagPROCESSENTRY32W PROCESSENTRY32W;
typedef struct tagMODULEENTRY32W MODULEENTRY32W;
struct SnapshotIterator;

namespace winapi::th32
{
  /// \brief Syntax sugar for specifying the current process.
  enum PID : uint32_t { CurrentProcess = 0 };

  /**
   * \brief Describes an entry from a list of the processes residing in the system address space when a snapshot was taken.
   * \see https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/ns-tlhelp32-processentry32
   */
  struct WINAPI20_EXPORT ProcessEntry
  {
    using raw_type = PROCESSENTRY32W;

    /// \brief The process ID.
    uint32_t pid;

    /// \brief The parent process ID.
    uint32_t parent_pid;

    /// \brief The number of execution threads started by this process.
    size_t thread_count;

    /// \brief The base priority of any threads started by this process.
    int64_t thread_base_priority;

    /// \brief A handle to the module in the context of the owning process.
    void* module;

    /// \brief The module name.
    std::string name;

    /// \brief The module path.
    std::string path;

    [[nodiscard]] static auto from_raw(raw_type const& entry) -> ProcessEntry;

    WINAPI20_EXPORT friend std::ostream& operator<<(std::ostream& stream, ProcessEntry const& entry);
  };

  /**
   * \brief Describes an entry from a list of the modules belonging to the specified process.
   * \see https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/ns-tlhelp32-moduleentry32w
   */
  struct WINAPI20_EXPORT ModuleEntry
  {
    using raw_type = MODULEENTRY32W;

    /// \brief The identifier of the process whose modules are to be examined.
    uint32_t pid;

    /// \brief The base address of the module in the context of the owning process.
    uintptr_t base_address;

    /// \brief The size of the module, in bytes.
    size_t size;

    /// \brief The name of the module.
    std::string name;

    /// \brief The path of the module.
    std::string path;

    [[nodiscard]] static auto from_raw(raw_type const& entry) -> ModuleEntry;

    WINAPI20_EXPORT friend std::ostream& operator<<(std::ostream& stream, ModuleEntry const& entry);
  };

  class WINAPI20_EXPORT Snapshot
  {
    public:
      /**
       * \brief The portions of the system to be included in the snapshot.
       * \details This parameter can be one or more of the following values.
       * \see https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/nf-tlhelp32-createtoolhelp32snapshot
       */
      enum class IncludeFlags : uint32_t
      {
        /// \brief Includes all heaps of the process specified in th32ProcessID in the snapshot.
        HeapList = 1 << 0,

        /// \brief Includes all processes in the system in the snapshot.
        Process  = 1 << 1,

        /// \brief Includes all threads in the system in the snapshot.
        Thread   = 1 << 2,

        /// \brief Includes all modules of the process specified in th32ProcessID in the snapshot.
        Module   = 1 << 3,

        /// \brief Includes all 32-bit modules of the process specified in th32ProcessID in the snapshot when called from a 64-bit process.
        Module32 = 1 << 4,

        /// \brief Includes all processes and threads in the system, plus the heaps and modules of the process specified in th32ProcessID.
        All      = HeapList | Process | Thread | Module,

        /// \brief Indicates that the snapshot handle is to be inheritable.
        Inherit  = 0x80000000
      };

      explicit Snapshot(IncludeFlags flags, uint32_t pid) noexcept(false);
      ~Snapshot();

      [[nodiscard]] inline auto flags() const noexcept -> IncludeFlags { return this->m_flags; }
      [[nodiscard]] inline auto pid() const noexcept -> uint32_t { return this->m_pid; }
      [[nodiscard]] inline auto handle_raw() const noexcept -> void* { return this->m_handle; }

      [[nodiscard]] auto valid() const noexcept -> bool;
      [[nodiscard]] auto processes() const noexcept(false) -> std::vector<ProcessEntry>;
      [[nodiscard]] auto modules() const noexcept(false) -> std::vector<ModuleEntry>;

      template <typename T>
      [[nodiscard]] auto entries() const noexcept(false) -> std::vector<T>;

      template <typename T>
      [[nodiscard]] auto find_first_by_name(
          std::string_view name,
          CaseSensitivity cs = CaseSensitivity::CaseInsensitive
      ) const noexcept(false) -> std::optional<T>;

      template <typename T>
      [[nodiscard]] auto find_last_by_name(
          std::string_view name,
          CaseSensitivity cs = CaseSensitivity::CaseInsensitive
      ) const noexcept(false) -> std::optional<T>;

      template <typename T>
      [[nodiscard]] auto find_by_name(
          std::string_view name,
          CaseSensitivity cs = CaseSensitivity::CaseInsensitive
      ) const noexcept(false) -> std::vector<T>;

      inline explicit operator bool() const noexcept { return this->valid(); }

      friend ::SnapshotIterator;

    private:
      IncludeFlags m_flags;
      uint32_t m_pid;
      void* m_handle;
      std::set<IncludeFlags> mutable m_flags_valid;
  };
}

namespace winapi::th32
{
  template <typename T>
  inline auto Snapshot::entries() const noexcept(false) -> std::vector<T>
  {
    static_assert(
        std::is_same_v<T, ProcessEntry> or std::is_same_v<T, ModuleEntry>,
        "Type must be either ProcessEntry or ModuleEntry"
    );
    if constexpr(std::is_same_v<T, ProcessEntry>)
      return this->processes();
    if constexpr(std::is_same_v<T, ModuleEntry>)
      return this->modules();
  }

  template <typename T>
  inline auto Snapshot::find_first_by_name(
    std::string_view name,
    CaseSensitivity cs
  ) const noexcept(false) -> std::optional<T>
  {
    auto vec = this->entries<T>();
    if(vec.empty())
      return std::nullopt;
    auto entry = std::find_if(
        vec.begin(),
        vec.end(),
        [&](auto const& entry) -> bool {
          return utility::sensitive_compare(name, entry.name, cs);
        });
    if(entry != vec.end())
      return *entry;
    return std::nullopt;
  }

  template <typename T>
  inline auto Snapshot::find_last_by_name(
    std::string_view name,
    CaseSensitivity cs
  ) const noexcept(false) -> std::optional<T>
  {
    auto vec = this->entries<T>();
    if(vec.empty())
      return std::nullopt;
    auto entry = std::find_if(
        vec.rbegin(),
        vec.rend(),
        [&](auto const& entry) -> bool {
          return utility::sensitive_compare(name, entry.name, cs);
        });
    if(entry != vec.rend())
      return *entry;
    return std::nullopt;
  }

  template <typename T>
  inline auto Snapshot::find_by_name(
    std::string_view name,
    CaseSensitivity cs
  ) const noexcept(false) -> std::vector<T>
  {
    auto res = std::vector<T>();
    auto vec = this->entries<T>();
    if(vec.empty())
      return res;
    for(auto const& entry : vec)
      if(utility::sensitive_compare(name, entry.name, cs))
        res.push_back(entry);
    return entries;
  }
}
