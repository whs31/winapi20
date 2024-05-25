#include <winapi20/impl/memoryapi_impl.h>

#include <winapi20/detail/cvt.h>
#include <winapi20/detail/exception.h>
#include <winapi20/detail/windows_headers.h>
#include <winapi20/impl/errhandlingapi_impl.h>

namespace winapi::checks
{
  using namespace memory;
  static_assert(static_cast<uint32_t>(PageAccess::Readonly) == PAGE_READONLY, "PageAccess::Readonly != PAGE_READONLY");
  static_assert(static_cast<uint32_t>(PageAccess::ReadWrite) == PAGE_READWRITE, "PageAccess::ReadWrite != PAGE_READWRITE");
  static_assert(static_cast<uint32_t>(PageAccess::NoAccess) == PAGE_NOACCESS, "PageAccess::NoAccess != PAGE_NOACCESS");
  static_assert(static_cast<uint32_t>(PageAccess::WriteCopy) == PAGE_WRITECOPY, "PageAccess::WriteCopy != PAGE_WRITECOPY");
  static_assert(static_cast<uint32_t>(PageAccess::Execute) == PAGE_EXECUTE, "PageAccess::Execute != PAGE_EXECUTE");
  static_assert(static_cast<uint32_t>(PageAccess::ExecuteRead) == PAGE_EXECUTE_READ, "PageAccess::ExecuteRead != PAGE_EXECUTE_READ");
  static_assert(static_cast<uint32_t>(PageAccess::ExecuteReadWrite) == PAGE_EXECUTE_READWRITE, "PageAccess::ExecuteReadWrite != PAGE_EXECUTE_READWRITE");
  static_assert(static_cast<uint32_t>(PageAccess::ExecuteWriteCopy) == PAGE_EXECUTE_WRITECOPY, "PageAccess::ExecuteWriteCopy != PAGE_EXECUTE_WRITECOPY");
  static_assert(static_cast<uint32_t>(PageAccess::Guard) == PAGE_GUARD, "PageAccess::Guard != PAGE_GUARD");
  static_assert(static_cast<uint32_t>(PageAccess::NoCache) == PAGE_NOCACHE, "PageAccess::NoCache != PAGE_NOCACHE");
  static_assert(static_cast<uint32_t>(PageAccess::WriteCombine) == PAGE_WRITECOMBINE, "PageAccess::WriteCombine != PAGE_WRITECOMBINE");
  static_assert(static_cast<uint32_t>(PageAccess::GraphicsNoAccess) == PAGE_GRAPHICS_NOACCESS, "PageAccess::GraphicsNoAccess != PAGE_GRAPHICS_NOACCESS");
  static_assert(static_cast<uint32_t>(PageAccess::GraphicsReadOnly) == PAGE_GRAPHICS_READONLY, "PageAccess::GraphicsReadOnly != PAGE_GRAPHICS_READONLY");
  static_assert(static_cast<uint32_t>(PageAccess::GraphicsReadWrite) == PAGE_GRAPHICS_READWRITE, "PageAccess::GraphicsReadWrite != PAGE_GRAPHICS_READWRITE");
  static_assert(static_cast<uint32_t>(PageAccess::GraphicsExecute) == PAGE_GRAPHICS_EXECUTE, "PageAccess::GraphicsExecute != PAGE_GRAPHICS_EXECUTE");
  static_assert(static_cast<uint32_t>(PageAccess::GraphicsExecuteRead) == PAGE_GRAPHICS_EXECUTE_READ, "PageAccess::GraphicsExecuteRead != PAGE_GRAPHICS_EXECUTE_READ");
  static_assert(static_cast<uint32_t>(PageAccess::GraphicsExecuteReadWrite) == PAGE_GRAPHICS_EXECUTE_READWRITE, "PageAccess::GraphicsExecuteReadWrite != PAGE_GRAPHICS_EXECUTE_READWRITE");
  static_assert(static_cast<uint32_t>(PageAccess::GraphicsCoherent) == PAGE_GRAPHICS_COHERENT, "PageAccess::GraphicsCoherent != PAGE_GRAPHICS_COHERENT");
  static_assert(static_cast<uint32_t>(PageAccess::GraphicsNocache) == PAGE_GRAPHICS_NOCACHE, "PageAccess::GraphicsNocache != PAGE_GRAPHICS_NOCACHE");
  static_assert(static_cast<uint32_t>(PageAccess::EnclaveThreadControl) == PAGE_ENCLAVE_THREAD_CONTROL, "PageAccess::EnclaveThreadControl != PAGE_ENCLAVE_THREAD_CONTROL");
  static_assert(static_cast<uint32_t>(PageAccess::RevertToFileMap) == PAGE_REVERT_TO_FILE_MAP, "PageAccess::RevertToFileMap != PAGE_REVERT_TO_FILE_MAP");
  static_assert(static_cast<uint32_t>(PageAccess::TargetsNoUpdate) == PAGE_TARGETS_NO_UPDATE, "PageAccess::TargetsNoUpdate != PAGE_TARGETS_NO_UPDATE");
  static_assert(static_cast<uint32_t>(PageAccess::TargetsInvalid) == PAGE_TARGETS_INVALID, "PageAccess::TargetsInvalid != PAGE_TARGETS_INVALID");
  static_assert(static_cast<uint32_t>(PageAccess::EnclaveUnvalidated) == PAGE_ENCLAVE_UNVALIDATED, "PageAccess::EnclaveUnvalidated != PAGE_ENCLAVE_UNVALIDATED");
  static_assert(static_cast<uint32_t>(PageAccess::EnclaveMask) == PAGE_ENCLAVE_MASK, "PageAccess::EnclaveMask != PAGE_ENCLAVE_MASK");
  static_assert(static_cast<uint32_t>(PageAccess::EnclaveDecommit) == PAGE_ENCLAVE_DECOMMIT, "PageAccess::EnclaveDecommit != PAGE_ENCLAVE_DECOMMIT");
  static_assert(static_cast<uint32_t>(PageAccess::EnclaveSsFirst) == PAGE_ENCLAVE_SS_FIRST, "PageAccess::EnclaveSsFirst != PAGE_ENCLAVE_SS_FIRST");
  static_assert(static_cast<uint32_t>(PageAccess::EnclaveSsRest) == PAGE_ENCLAVE_SS_REST, "PageAccess::EnclaveSsRest != PAGE_ENCLAVE_SS_REST");

  static_assert(static_cast<uint32_t>(MemoryOperation::Commit) == MEM_COMMIT, "MemoryOperation::Commit != MEM_COMMIT");
  static_assert(static_cast<uint32_t>(MemoryOperation::Reserve) == MEM_RESERVE, "MemoryOperation::Reserve != MEM_RESERVE");
  static_assert(static_cast<uint32_t>(MemoryOperation::ReplacePlaceholder) == MEM_REPLACE_PLACEHOLDER, "MemoryOperation::ReplacePlaceholder != MEM_REPLACE_PLACEHOLDER");
  static_assert(static_cast<uint32_t>(MemoryOperation::Decommit) == MEM_DECOMMIT, "MemoryOperation::Decommit != MEM_DECOMMIT");
  static_assert(static_cast<uint32_t>(MemoryOperation::Release) == MEM_RELEASE, "MemoryOperation::Release != MEM_RELEASE");
  static_assert(static_cast<uint32_t>(MemoryOperation::Free) == MEM_FREE, "MemoryOperation::Free != MEM_FREE");
  static_assert(static_cast<uint32_t>(MemoryOperation::Private) == MEM_PRIVATE, "MemoryOperation::Private != MEM_PRIVATE");
  static_assert(static_cast<uint32_t>(MemoryOperation::ReservePlaceholder) == MEM_RESERVE_PLACEHOLDER, "MemoryOperation::ReservePlaceholder != MEM_RESERVE_PLACEHOLDER");
  static_assert(static_cast<uint32_t>(MemoryOperation::Mapped) == MEM_MAPPED, "MemoryOperation::Mapped != MEM_MAPPED");
  static_assert(static_cast<uint32_t>(MemoryOperation::Reset) == MEM_RESET, "MemoryOperation::Reset != MEM_RESET");
  static_assert(static_cast<uint32_t>(MemoryOperation::TopDown) == MEM_TOP_DOWN, "MemoryOperation::TopDown != MEM_TOP_DOWN");
  static_assert(static_cast<uint32_t>(MemoryOperation::WriteWatch) == MEM_WRITE_WATCH, "MemoryOperation::WriteWatch != MEM_WRITE_WATCH");
  static_assert(static_cast<uint32_t>(MemoryOperation::Physical) == MEM_PHYSICAL, "MemoryOperation::Physical != MEM_PHYSICAL");
  static_assert(static_cast<uint32_t>(MemoryOperation::Rotate) == MEM_ROTATE, "MemoryOperation::Rotate != MEM_ROTATE");
  static_assert(static_cast<uint32_t>(MemoryOperation::DifferentImageBaseOk) == MEM_DIFFERENT_IMAGE_BASE_OK, "MemoryOperation::DifferentImageBaseOk != MEM_DIFFERENT_IMAGE_BASE_OK");
  static_assert(static_cast<uint32_t>(MemoryOperation::ResetUndo) == MEM_RESET_UNDO, "MemoryOperation::ResetUndo != MEM_RESET_UNDO");
  static_assert(static_cast<uint32_t>(MemoryOperation::LargePages) == MEM_LARGE_PAGES, "MemoryOperation::LargePages != MEM_LARGE_PAGES");
  static_assert(static_cast<uint32_t>(MemoryOperation::UnmapWithTransientBoost) == MEM_UNMAP_WITH_TRANSIENT_BOOST, "MemoryOperation::UnmapWithTransientBoost != MEM_UNMAP_WITH_TRANSIENT_BOOST");
  static_assert(static_cast<uint32_t>(MemoryOperation::CoalescePlaceholders) == MEM_COALESCE_PLACEHOLDERS, "MemoryOperation::CoalescePlaceholders != MEM_COALESCE_PLACEHOLDERS");
  static_assert(static_cast<uint32_t>(MemoryOperation::PreservePlaceholders) == MEM_PRESERVE_PLACEHOLDER, "MemoryOperation::PreservePlaceholders != MEM_PRESERVE_PLACEHOLDER");
}