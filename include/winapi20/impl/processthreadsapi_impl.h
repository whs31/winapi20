#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <iosfwd>
#include <winapi20/detail/export.h>
#include <winapi20/detail/definitions.h>
#include <winapi20/detail/template_util.h>
#include <winapi20/wrappers/handle.h>
#include <winapi20/wrappers/pid.h>

namespace winapi
{
  /**
   * \brief Specifies the access rights for a process.
   * \details The Microsoft Windows security model enables you to control access to process objects.
   * For more information about security, see Access-Control Model.
   *
   * When a user logs in, the system collects a set of data that uniquely identifies the user during the
   * authentication process, and stores it in an access token. This access token describes the security context
   * of all processes associated with the user. The security context of a process is the set of credentials given
   * to the process or the user account that created the process.
   * \see https://learn.microsoft.com/en-us/windows/win32/procthread/process-security-and-access-rights
   * \warning A process that has some of the access rights noted here can use them to gain other access rights.
   * For example, if process A has a handle to process B with DuplicateHandle access, it can duplicate the pseudo
   * handle for process B. This creates a handle that has maximum access to process B.
   */
  enum class AccessRights : uint64_t
  {
    /// \brief Required to delete the object.
    Delete                  = 0x00010000L,

    /**
     * \brief Required to read information in the security descriptor for the object, not including the information in the SACL.
     * \note To read or write the SACL, you must request the ACCESS_SYSTEM_SECURITY access right.
     * For more information, see SACL Access Rights.
     * \see https://learn.microsoft.com/en-us/windows/win32/secauthz/sacl-access-right
     */
    ReadControl             = 0x00020000L,

    /**
     * \brief The right to use the object for synchronization.
     * \details This enables a thread to wait until the object is in the signaled state.
     */
    Synchronize             = 0x00100000L,

    /// \brief Required to modify the DACL in the security descriptor for the object.
    WriteDAC                = 0x00040000L,

    /// \brief Required to change the owner in the security descriptor for the object.
    WriteOwner              = 0x00080000L,

    /// \brief Required to use this process as the parent process.
    CreateProcess           = 0x0080,

    /// \brief Required to create a thread in the process.
    CreateThread            = 0x0002,

    /// \brief Required to duplicate a handle to the process.
    DuplicateHandle         = 0x0040,

    /// \brief Required to retrieve certain information about a process, such as its token, exit code, and priority class.
    QueryInformation        = 0x0400,

    /// \brief Required to retrieve certain information about a process
    QueryLimitedInformation = 0x1000,

    /// \brief Required to set certain information about a process, such as its priority class.
    SetInformation          = 0x0200,

    /// \brief Required to set memory limits using SetProcessWorkingSetSize.
    SetQuota                = 0x0100,

    /// \brief Required to suspend or resume a process.
    SuspendResume           = 0x0800,

    /// \brief Required to terminate a process using TerminateProcess.
    Terminate               = 0x0001,

    /// \brief Required to perform an operation on the address space of a process (see VirtualProtectEx and WriteProcessMemory).
    VirtualMemoryOperation  = 0x0008,

    /// \brief Required to read or write virtual memory in a process using ReadProcessMemory and WriteProcessMemory.
    VirtualMemoryRead       = 0x0010,

    /// \brief Required to read or write virtual memory in a process using ReadProcessMemory and WriteProcessMemory.
    VirtualMemoryWrite      = 0x0020,

    All                     = 0x000F0000L | Synchronize | 0xFFFF
  };

  class WINAPI20_EXPORT Process
  {
    public:
      explicit Process(PID pid, AccessRights rights, bool inherit_handle = false) noexcept(false);
      virtual ~Process();

      [[nodiscard]] inline auto handle() const noexcept -> Handle const& { return this->m_handle; }

    public:
      [[nodiscard]] static auto current() noexcept -> Process;

    private:
      Process() = default;
      explicit Process(Handle&& handle) noexcept;

    private:
      Handle m_handle;
  };
} // namespace winapi