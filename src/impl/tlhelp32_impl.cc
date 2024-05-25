#include <winapi20/impl/tlhelp32_impl.h>

#include <iostream>
#include <winapi20/impl/errhandlingapi_impl.h>
#include <winapi20/detail/exception.h>
#include <winapi20/detail/windows_headers.h>
#include <utility/utility.hh>
#include <utility/windows_header_predef.hh>

#if defined(__has_include)
# if __has_include(<tlhelp32.h>)
#   include <tlhelp32.h>
# elif __has_include(<TlHelp32.h>)
#   include <TlHelp32.h>
# else
#   error "tlhelp32.h not found"
# endif // __has_include(<tlhelp32.h>)
#endif // __has_include

using winapi::enums::operator&;
using winapi::enums::operator|;

using namespace winapi::th32;
using enum Snapshot::IncludeFlags;

struct SnapshotIterator
{
  template <bool option, typename T, typename F>
  [[nodiscard]] static auto iterator(
      Snapshot const* snapshot,
      Snapshot::IncludeFlags flag,
      F f
  ) -> std::conditional_t<option, std::optional<T>, T>
  {
    if(not *snapshot)
      throw winapi::windows_exception("snapshot is not valid");

    if(not static_cast<std::underlying_type_t<Snapshot::IncludeFlags>>(snapshot->flags() & flag))
      throw winapi::windows_exception("snapshot contains no processes. specify IncludeFlags correctly");

    typename T::raw_type entry;
    entry.dwSize = sizeof(typename T::raw_type);
    if(not f(snapshot->handle_raw(), &entry)) {
      if constexpr(option)
        return std::nullopt;
      else
        throw winapi::windows_exception(winapi::last_error_string());
    }
    return T::from_raw(entry);
  }

  template <typename T, typename F1, typename F2>
  [[nodiscard]] static auto collect(Snapshot const* snapshot, Snapshot::IncludeFlags flag, F1 f1, F2 f2) -> std::vector<T>
  {
    std::vector<T> entries;
    auto entry = std::make_optional<T>(iterator<false, T>(snapshot, flag, f1));
    if(not entry)
      return entries;
    while(entry) {
      entries.push_back(*entry);
      entry = iterator<true, T>(snapshot, flag, f2);
    }
    snapshot->m_flags_valid.insert(flag);
    return entries;
  }
};

namespace winapi::th32
{
  auto ProcessEntry::from_raw(PROCESSENTRY32W const& entry) -> ProcessEntry {
    return {
        .pid = entry.th32ProcessID,
        .parent_pid = entry.th32ParentProcessID,
        .thread_count = entry.cntThreads,
        .thread_base_priority = entry.pcPriClassBase,
        .name = winapi::detail::into_utf8(entry.szExeFile)
    };
  }

  std::ostream& operator<<(std::ostream& stream, const ProcessEntry &entry) {
    stream << "ProcessEntry { pid: " << entry.pid << ", parent_pid: " << entry.parent_pid
           << ", thread_count: " << entry.thread_count
           << ", thread_base_priority: " << entry.thread_base_priority
           << ", name: " << entry.name << " }";
    return stream;
  }

  auto ModuleEntry::from_raw(MODULEENTRY32W const& entry) -> ModuleEntry {
    return {
        .pid = entry.th32ProcessID,
        .base_address = reinterpret_cast<uintptr_t>(entry.modBaseAddr),
        .size = entry.modBaseSize,
        .name = detail::into_utf8(entry.szModule),
        .path = detail::into_utf8(entry.szExePath)
    };
  }

  std::ostream& operator<<(std::ostream &stream, const ModuleEntry &entry) {
    stream << "ModuleEntry { pid: " << entry.pid
           << ", base_address: " << entry.base_address
           << ", size: " << entry.size
           << ", name: " << entry.name
           << ", path: " << entry.path << " }";
    return stream;
  }

  Snapshot::Snapshot(Snapshot::IncludeFlags flags, uint32_t pid) noexcept(false)
    : m_flags(flags)
    , m_pid(pid)
    , m_handle(nullptr)
    , m_flags_valid(std::set<Snapshot::IncludeFlags>())
  {
    this->m_handle = ::CreateToolhelp32Snapshot(static_cast<DWORD>(flags), static_cast<DWORD>(pid));
    if(this->m_handle == INVALID_HANDLE_VALUE)
      throw winapi::windows_exception(winapi::last_error_string());
  }

  Snapshot::~Snapshot() {
    ::CloseHandle(this->m_handle);
  }

  auto Snapshot::valid() const noexcept -> bool {
    return this->m_handle != nullptr and this->m_handle != INVALID_HANDLE_VALUE;
  }

  auto Snapshot::processes() const noexcept(false) -> std::vector<ProcessEntry> {
    return SnapshotIterator::collect<ProcessEntry>(this, IncludeFlags::Process, ::Process32FirstW, ::Process32NextW);
  }

  auto Snapshot::modules() const noexcept(false) -> std::vector<ModuleEntry> {
    return SnapshotIterator::collect<ModuleEntry>(this, IncludeFlags::Module, ::Module32FirstW, ::Module32NextW);
  }
} // namespace winapi::th32