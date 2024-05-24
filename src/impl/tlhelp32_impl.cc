#include <winapi20/impl/tlhelp32_impl.h>
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

namespace winapi::th32
{
  Snapshot::Snapshot(Snapshot::IncludeFlags flags, uint32_t pid) noexcept(false)
    : m_flags(flags)
    , m_pid(pid)
    , m_handle(nullptr)
    , m_processes_valid(false)
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

  auto Snapshot::first() const noexcept(false) -> ProcessEntry {
    if(not *this)
      throw winapi::windows_exception("snapshot is not valid");

    if(not static_cast<std::underlying_type_t<Snapshot::IncludeFlags>>(this->m_flags & Snapshot::IncludeFlags::Process))
      throw winapi::windows_exception("snapshot contains no processes. specify IncludeFlags::Process");

    auto entry_raw = PROCESSENTRY32W();
    entry_raw.dwSize = sizeof(PROCESSENTRY32W);
    if(not ::Process32First(this->m_handle, &entry_raw))
      throw winapi::windows_exception(winapi::last_error_string());
    return ProcessEntry {
      .pid = entry_raw.th32ProcessID,
      .parent_pid = entry_raw.th32ParentProcessID,
      .thread_count = entry_raw.cntThreads,
      .thread_base_priority = entry_raw.pcPriClassBase,
      .name = detail::into_utf8(entry_raw.szExeFile)
    };
  }

  auto Snapshot::next() const noexcept(false) -> std::optional<ProcessEntry> {
    if(not *this)
      throw winapi::windows_exception("snapshot is not valid");

    auto entry_raw = PROCESSENTRY32W();
    entry_raw.dwSize = sizeof(PROCESSENTRY32W);
    if(not ::Process32Next(this->m_handle, &entry_raw))
      return std::nullopt;
    return ProcessEntry {
      .pid = entry_raw.th32ProcessID,
      .parent_pid = entry_raw.th32ParentProcessID,
      .thread_count = entry_raw.cntThreads,
      .thread_base_priority = entry_raw.pcPriClassBase,
      .name = detail::into_utf8(entry_raw.szExeFile)
    };
  }

  auto Snapshot::processes() const noexcept(false) -> std::vector<ProcessEntry>
  {
    std::vector<ProcessEntry> entries;
    auto entry = std::make_optional<ProcessEntry>(this->first());
    while(entry) {
      entries.push_back(*entry);
      entry = this->next();
    }
    this->m_processes_valid = true;
    return entries;
  }

  auto Snapshot::process_by_name(string_view name, CaseSensitivity cs) const noexcept(false)
    -> std::optional<ProcessEntry>
  {
    if(not this->m_processes_valid)
      std::ignore = this->processes();

    auto name_cs = string(name);
    if(cs == CaseSensitivity::CaseInsensitive)
      name_cs = winapi::utility::to_lowercase(name_cs);
    for(auto const& entry : this->processes())
      if(cs == CaseSensitivity::CaseSensitive ? name_cs == entry.name : name_cs == winapi::utility::to_lowercase(entry.name))
        return entry;
    return std::nullopt;
  }
} // namespace winapi::th32