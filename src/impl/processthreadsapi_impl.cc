#include <winapi20/impl/processthreadsapi_impl.h>

#include <winapi20/detail/cvt.h>
#include <winapi20/detail/exception.h>
#include <winapi20/detail/windows_headers.h>
#include <winapi20/impl/errhandlingapi_impl.h>

#undef CreateProcess

namespace winapi::checks
{
  static_assert(static_cast<uint64_t>(AccessRights::Delete) == DELETE, "AccessRights::Delete != DELETE");
  static_assert(static_cast<uint64_t>(AccessRights::ReadControl) == READ_CONTROL, "AccessRights::ReadControl != READ_CONTROL");
  static_assert(static_cast<uint64_t>(AccessRights::Synchronize) == SYNCHRONIZE, "AccessRights::Synchronize != SYNCHRONIZE");
  static_assert(static_cast<uint64_t>(AccessRights::WriteDAC) == WRITE_DAC, "AccessRights::WriteDAC != WRITE_DAC");
  static_assert(static_cast<uint64_t>(AccessRights::WriteOwner) == WRITE_OWNER, "AccessRights::WriteOwner != WRITE_OWNER");
  static_assert(static_cast<uint64_t>(AccessRights::CreateProcess) == PROCESS_CREATE_PROCESS, "AccessRights::CreateProcess != PROCESS_CREATE_PROCESS");
  static_assert(static_cast<uint64_t>(AccessRights::CreateThread) == PROCESS_CREATE_THREAD, "AccessRights::CreateThread != PROCESS_CREATE_THREAD");
  static_assert(static_cast<uint64_t>(AccessRights::DuplicateHandle) == PROCESS_DUP_HANDLE, "AccessRights::DuplicateHandle != PROCESS_DUP_HANDLE");
  static_assert(static_cast<uint64_t>(AccessRights::QueryInformation) == PROCESS_QUERY_INFORMATION, "AccessRights::QueryInformation != PROCESS_QUERY_INFORMATION");
  static_assert(static_cast<uint64_t>(AccessRights::QueryLimitedInformation) == PROCESS_QUERY_LIMITED_INFORMATION, "AccessRights::QueryLimitedInformation != PROCESS_QUERY_LIMITED_INFORMATION");
  static_assert(static_cast<uint64_t>(AccessRights::SetInformation) == PROCESS_SET_INFORMATION, "AccessRights::SetInformation != PROCESS_SET_INFORMATION");
  static_assert(static_cast<uint64_t>(AccessRights::SetQuota) == PROCESS_SET_QUOTA, "AccessRights::SetQuota != PROCESS_SET_QUOTA");
  static_assert(static_cast<uint64_t>(AccessRights::SuspendResume) == PROCESS_SUSPEND_RESUME, "AccessRights::SuspendResume != PROCESS_SUSPEND_RESUME");
  static_assert(static_cast<uint64_t>(AccessRights::Terminate) == PROCESS_TERMINATE, "AccessRights::Terminate != PROCESS_TERMINATE");
  static_assert(static_cast<uint64_t>(AccessRights::VirtualMemoryOperation) == PROCESS_VM_OPERATION, "AccessRights::VirtualMemoryOperation != PROCESS_VM_OPERATION");
  static_assert(static_cast<uint64_t>(AccessRights::VirtualMemoryRead) == PROCESS_VM_READ, "AccessRights::VirtualMemoryRead != PROCESS_VM_READ");
  static_assert(static_cast<uint64_t>(AccessRights::VirtualMemoryWrite) == PROCESS_VM_WRITE, "AccessRights::VirtualMemoryWrite != PROCESS_VM_WRITE");
  static_assert(static_cast<uint64_t>(AccessRights::All) == PROCESS_ALL_ACCESS, "AccessRights::All != PROCESS_ALL_ACCESS");
}

namespace winapi
{
  Process::Process(PID pid, winapi::AccessRights rights, bool inherit_handle) noexcept(false)
   : m_handle(::OpenProcess(static_cast<decltype(PROCESS_ALL_ACCESS)>(rights), inherit_handle, *pid), Cleanup::Auto)
  {
    if(not this->m_handle)
      throw winapi::windows_exception(winapi::last_error_string());
  }

  auto Process::current() noexcept -> Process {
    return Process(Handle(::GetCurrentProcess(), Cleanup::Auto));
  }

  Process::~Process() = default;

  Process::Process(Handle&& handle) noexcept
      : m_handle(std::move(handle))
  {}
} // namespace winapi