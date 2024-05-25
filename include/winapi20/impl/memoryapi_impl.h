#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <iosfwd>
#include <winapi20/detail/export.h>
#include <winapi20/detail/definitions.h>
#include <winapi20/detail/template_util.h>
#include <winapi20/impl/processthreadsapi_impl.h>

/// \brief Namespace for memory-related operations in the Windows API.
namespace winapi::memory
{
  /**
   * \brief Memory protection constants enum wrapper.
   * \details The following are the memory-protection options; you must specify one of the following values when
   * allocating or protecting a page in memory.
   * Protection attributes cannot be assigned to a portion of a page; they can only be assigned to a whole page.
   * \see https://learn.microsoft.com/en-us/windows/win32/memory/memory-protection-constants
   */
  enum class MemoryProtection : uint64_t
  {
    /**
     * \brief Enables read-only access to the committed region of pages.
     * \note If Data Execution Prevention is enabled, an attempt to execute code in the committed region
     * results in an access violation.
     * \warning An attempt to write to the committed region results in an access violation.
     * \see https://learn.microsoft.com/en-us/windows/win32/memory/data-execution-prevention
     */
    Readonly                  = 1 << 1,

    /**
     * \brief Enables read-only or read/write access to the committed region of pages.
     * \note If Data Execution Prevention is enabled, an attempt to execute code in the committed region
     * results in an access violation.
     * \see https://learn.microsoft.com/en-us/windows/win32/memory/data-execution-prevention
     */
    ReadWrite                 = 1 << 2,

    /**
     * \brief Disables all access to the committed region of pages.
     * \warning An attempt to read from, write to, or execute the committed region results in an access violation.
     */
    NoAccess                  = 1 << 0,

    /**
     * \brief Enables read-only or copy-on-write access to a mapped view of a file mapping object.
     * \note An attempt to write to a committed copy-on-write page results in a private copy of the page being
     * made for the process. The private page is marked as <tt>ReadWrite</tt>, and the change is written to the new page.
     * If Data Execution Prevention is enabled, attempting to execute code in the committed region results in an
     * access violation.
     * \note <b><i>This flag is not supported by the <tt>VirtualAlloc</tt> or <tt>VirtualAllocEx</tt> functions in original WinAPI.</i></b>
     */
    WriteCopy                 = 1 << 3,

    /**
     * \brief Enables execute access to the committed region of pages.
     * \warning  An attempt to write to the committed region results in an access violation.
     */
    Execute                   = 1 << 4,

    /**
     * \brief Enables execute or read-only access to the committed region of pages.
     * \warning  An attempt to write to the committed region results in an access violation.
     */
    ExecuteRead               = 1 << 5,

    /**
     * \brief Enables execute, read-only, or read/write access to the committed region of pages.
     */
    ExecuteReadWrite          = 1 << 6,

    /**
     * \brief Enables execute, read-only, or copy-on-write access to a mapped view of a file mapping object.
     * \note An attempt to write to a committed copy-on-write page results in a private copy of the page being made
     * for the process.
     * The private page is marked as ExecuteReadWrite, and the change is written to the new page.
     * \note <b><i>This flag is not supported by the <tt>VirtualAlloc</tt> or <tt>VirtualAllocEx</tt> functions in original WinAPI.</i></b>
     */
    ExecuteWriteCopy          = 1 << 7,

    /**
     * \brief Pages in the region become guard pages.
     * \details <b>Meant to be used with any of the flags above.</b>
     * Any attempt to access a guard page causes the system to raise a <tt>STATUS_GUARD_PAGE_VIOLATION</tt> exception
     * and turn off the guard page status. Guard pages thus act as a one-time access alarm.
     * When an access attempt leads the system to turn off guard page status, the underlying page protection takes over.
     * If a guard page exception occurs during a system service, the service typically returns a failure status indicator.
     * This value cannot be used with NoAccess.
     * \note <b><i>This flag is not supported by the <tt>CreateFileMapping</tt> functions in original WinAPI.</i></b>
     * \see https://learn.microsoft.com/en-us/windows/win32/memory/creating-guard-pages
     */
    Guard                     = 1 << 8,

    /**
     * \brief Sets all pages to be non-cachable.
     * \details See more information in <a href="https://learn.microsoft.com/en-us/windows/win32/memory/memory-protection-constants">Memory Protection Constants</a>.
     */
    NoCache                   = 1 << 9,

    /**
     * \brief Sets all pages to be write-combined.
     * \details See more information in <a href="https://learn.microsoft.com/en-us/windows/win32/memory/memory-protection-constants">Memory Protection Constants</a>.
     */
    WriteCombine              = 1 << 10,
    GraphicsNoAccess          = 1 << 11,
    GraphicsReadOnly          = 1 << 12,
    GraphicsReadWrite         = 1 << 13,
    GraphicsExecute           = 1 << 14,
    GraphicsExecuteRead       = 1 << 15,
    GraphicsExecuteReadWrite  = 1 << 16,
    GraphicsCoherent          = 1 << 17,
    GraphicsNocache           = 1 << 18,
    EnclaveThreadControl      = 0x80000000,
    RevertToFileMap           = EnclaveThreadControl,
    TargetsNoUpdate           = 0x40000000,

    /**
     * \brief Sets all locations in the pages as invalid targets for CFG.
     * \details Used along with any execute page protection like Execute, ExecuteRead, ExecuteReadWrite and ExecuteWriteCopy.
     * Any indirect call to locations in those pages will fail CFG checks and the process will be terminated.
     * The default behavior for executable pages allocated is to be marked valid call targets for CFG.
     * \note <b><i>This flag is not supported by the <tt>VirtualAlloc</tt> or <tt>VirtualAllocEx</tt> functions in original WinAPI.</i></b>
     */
    TargetsInvalid            = TargetsNoUpdate,
    EnclaveUnvalidated        = 0x20000000,
    EnclaveMask               = 0x10000000,
    EnclaveDecommit           = EnclaveMask,
    EnclaveSsFirst            = EnclaveMask | 1,
    EnclaveSsRest             = EnclaveMask | 2,
  };

  enum class MemoryState : uint64_t
  {
    /**
     * \brief Indicates committed pages where physical storage has been allocated.
     * \details Represents <b>Memory State</b>.
     */
    Commit                    = 1 << 12,

    /**
     * \brief Indicates reserved pages where a range of the process's virtual address space is reserved without any physical storage being allocated.
     * \details Represents <b>Memory State</b>. For reserved pages, the information in the Protect member is undefined.
     */
    Reserve                   = 1 << 13,

    /// \brief Alias for Decommit.
    ReplacePlaceholder        = 1 << 14,
    Decommit                  = ReplacePlaceholder,
    Release                   = 1 << 15,

    /**
     * \brief Indicates free pages not accessible to the calling process and available to be allocated.
     * \details Represents <b>Memory State</b>. For free pages, the information in the AllocationBase, AllocationProtect, Protect, and Type members is undefined.
     */
    Free                      = 1 << 16,

    /**
     * \brief Indicates that the memory pages within the region are private (that is, not shared by other processes).
     * \details Represents <b>Memory Type</b>.
     */
    Private                   = 1 << 17,

    /**
     * \brief Alias for Mapped.
     */
    ReservePlaceholder        = 1 << 18,

    /**
     * \brief Indicates that the memory pages within the region are mapped into the view of a section.
     * \details Represents <b>Memory Type</b>.
     */
    Mapped                    = ReservePlaceholder,
    Reset                     = 1 << 19,
    TopDown                   = 1 << 20,
    WriteWatch                = 1 << 21,
    Physical                  = 1 << 22,
    Rotate                    = 1 << 23,
    DifferentImageBaseOk      = Rotate,
    ResetUndo                 = 1 << 24,
    LargePages                = 0x20000000,
    UnmapWithTransientBoost   = 1 << 0,
    CoalescePlaceholders      = UnmapWithTransientBoost,
    PreservePlaceholders      = 1 << 1,

    /**
     * \brief Indicates that the memory pages within the region are mapped into the view of an image section.
     * \details Represents <b>Memory Type</b>.
     */
    Image                     = 0x1000000
  };

  struct WINAPI20_EXPORT MemoryBasicInformation
  {
    /// \brief A pointer to the base address of the region of pages.
    uintptr_t base_address;

    /// \brief A pointer to the base address of a range of pages allocated by the <tt>VirtualAlloc</tt> function.
    uintptr_t allocation_base;

    /**
     * \brief The memory protection option when the region was initially allocated.
     * \see https://learn.microsoft.com/en-us/windows/win32/memory/memory-protection-constants
     */
    MemoryProtection allocation_protection;

    /// \brief The size of the region beginning at the base address in which all pages have identical attributes, in bytes.
    size_t region_size;

    /**
     * \brief The state of the pages in the region.
     * \details This member can be one of the following values: <br>
     * - <tt>MemoryState::Commit</tt> <br>
     * - <tt>MemoryState::Reserve</tt> <br>
     * - <tt>MemoryState::Free</tt> <br>
     */
    MemoryState state;

    /**
     * \brief The access protection of the pages in the region.
     * \see https://learn.microsoft.com/en-us/windows/win32/memory/memory-protection-constants
     */
    MemoryProtection protection;

    /**
     * \brief The type of pages in the region.
     * \details This member can be one of the following values: <br>
     * - <tt>MemoryState::Image</tt> <br>
     * - <tt>MemoryState::Mapped</tt> <br>
     * - <tt>MemoryState::Private</tt> <br>
     */
    MemoryState type;

    /**
     * \brief Queries the memory state at the specified address in the address space of the calling process.
     * \details This function uses <tt>VirtualQuery</tt> to query the memory state at the specified address.
     * If you need to query memory state at an address that is not in the address space of the calling process, use <tt>MemoryBasicInformation::query(Process const&, uintptr_t)</tt>.
     * \warning If the query fails, the function throws a <tt>winapi::windows_exception</tt>.
     * \param address The address to query.
     * \throws winapi::windows_exception If the query fails.
     * \return The memory state at the specified address.
     * \see https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualquery
     */
    [[nodiscard]] static auto query(uintptr_t address) -> MemoryBasicInformation;

    /**
     * \brief Queries the memory state at the specified address in the address space of the specified process.
     * \details This function uses <tt>VirtualQueryEx</tt> to query the memory state at the specified address.
     * \warning If the query fails, the function throws a <tt>winapi::windows_exception</tt>.
     * \param process The process to query.
     * \param address The address to query.
     * \throws winapi::windows_exception If the query fails.
     * \return The memory state at the specified address.
     * \see https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualqueryex
     */
    [[nodiscard]] static auto query(Process const& process, uintptr_t address) -> MemoryBasicInformation;
  };
}