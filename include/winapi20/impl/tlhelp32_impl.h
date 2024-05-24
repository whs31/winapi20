#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <winapi20/detail/export.h>

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
    /// \brief The process ID.
    uint32_t pid;

    /// \brief The parent process ID.
    uint32_t parent_pid;

    /// \brief The number of execution threads started by this process.
    size_t thread_count;

    /// \brief The base priority of any threads started by this process.
    int64_t thread_base_priority;

    /// \brief The name of the executable file associated with the process.
    std::string name;
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

      enum class CaseSensitivity
      {
        CaseSensitive,
        CaseInsensitive
      };

      explicit Snapshot(IncludeFlags flags, uint32_t pid) noexcept(false);
      ~Snapshot();

      [[nodiscard]] inline auto flags() const noexcept -> IncludeFlags { return this->m_flags; }
      [[nodiscard]] inline auto pid() const noexcept -> uint32_t { return this->m_pid; }
      [[nodiscard]] inline auto handle_raw() const noexcept -> void* { return this->m_handle; }

      [[nodiscard]] auto valid() const noexcept -> bool;
      [[nodiscard]] auto processes() const noexcept(false) -> std::vector<ProcessEntry>;
      [[nodiscard]] auto process_by_name(std::string_view name, CaseSensitivity cs) const noexcept(false) -> std::optional<ProcessEntry>;

      inline explicit operator bool() const noexcept { return this->valid(); }

    protected:
      [[nodiscard]] auto first() const noexcept(false) -> ProcessEntry;
      [[nodiscard]] auto next() const noexcept(false) -> std::optional<ProcessEntry>;

    private:
      IncludeFlags m_flags;
      uint32_t m_pid;
      void* m_handle;
      bool mutable m_processes_valid;
  };
}