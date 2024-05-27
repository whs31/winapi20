#include <winapi20/impl/memoryapi_impl.h>

#include <fmt/format.h>
#include <winapi20/detail/cvt.h>
#include <winapi20/detail/exception.h>
#include <winapi20/detail/windows_headers.h>
#include <winapi20/impl/errhandlingapi_impl.h>

using std::optional;
using std::nullopt;

namespace winapi::checks
{
  using namespace memory;
  static_assert(static_cast<uint32_t>(MemoryProtection::Readonly) == PAGE_READONLY, "MemoryProtection::Readonly != PAGE_READONLY");
  static_assert(static_cast<uint32_t>(MemoryProtection::ReadWrite) == PAGE_READWRITE, "MemoryProtection::ReadWrite != PAGE_READWRITE");
  static_assert(static_cast<uint32_t>(MemoryProtection::NoAccess) == PAGE_NOACCESS, "MemoryProtection::NoAccess != PAGE_NOACCESS");
  static_assert(static_cast<uint32_t>(MemoryProtection::WriteCopy) == PAGE_WRITECOPY, "MemoryProtection::WriteCopy != PAGE_WRITECOPY");
  static_assert(static_cast<uint32_t>(MemoryProtection::Execute) == PAGE_EXECUTE, "MemoryProtection::Execute != PAGE_EXECUTE");
  static_assert(static_cast<uint32_t>(MemoryProtection::ExecuteRead) == PAGE_EXECUTE_READ, "MemoryProtection::ExecuteRead != PAGE_EXECUTE_READ");
  static_assert(static_cast<uint32_t>(MemoryProtection::ExecuteReadWrite) == PAGE_EXECUTE_READWRITE, "MemoryProtection::ExecuteReadWrite != PAGE_EXECUTE_READWRITE");
  static_assert(static_cast<uint32_t>(MemoryProtection::ExecuteWriteCopy) == PAGE_EXECUTE_WRITECOPY, "MemoryProtection::ExecuteWriteCopy != PAGE_EXECUTE_WRITECOPY");
  static_assert(static_cast<uint32_t>(MemoryProtection::Guard) == PAGE_GUARD, "MemoryProtection::Guard != PAGE_GUARD");
  static_assert(static_cast<uint32_t>(MemoryProtection::NoCache) == PAGE_NOCACHE, "MemoryProtection::NoCache != PAGE_NOCACHE");
  static_assert(static_cast<uint32_t>(MemoryProtection::WriteCombine) == PAGE_WRITECOMBINE, "MemoryProtection::WriteCombine != PAGE_WRITECOMBINE");
  static_assert(static_cast<uint32_t>(MemoryProtection::GraphicsNoAccess) == PAGE_GRAPHICS_NOACCESS, "MemoryProtection::GraphicsNoAccess != PAGE_GRAPHICS_NOACCESS");
  static_assert(static_cast<uint32_t>(MemoryProtection::GraphicsReadOnly) == PAGE_GRAPHICS_READONLY, "MemoryProtection::GraphicsReadOnly != PAGE_GRAPHICS_READONLY");
  static_assert(static_cast<uint32_t>(MemoryProtection::GraphicsReadWrite) == PAGE_GRAPHICS_READWRITE, "MemoryProtection::GraphicsReadWrite != PAGE_GRAPHICS_READWRITE");
  static_assert(static_cast<uint32_t>(MemoryProtection::GraphicsExecute) == PAGE_GRAPHICS_EXECUTE, "MemoryProtection::GraphicsExecute != PAGE_GRAPHICS_EXECUTE");
  static_assert(static_cast<uint32_t>(MemoryProtection::GraphicsExecuteRead) == PAGE_GRAPHICS_EXECUTE_READ, "MemoryProtection::GraphicsExecuteRead != PAGE_GRAPHICS_EXECUTE_READ");
  static_assert(static_cast<uint32_t>(MemoryProtection::GraphicsExecuteReadWrite) == PAGE_GRAPHICS_EXECUTE_READWRITE, "MemoryProtection::GraphicsExecuteReadWrite != PAGE_GRAPHICS_EXECUTE_READWRITE");
  static_assert(static_cast<uint32_t>(MemoryProtection::GraphicsCoherent) == PAGE_GRAPHICS_COHERENT, "MemoryProtection::GraphicsCoherent != PAGE_GRAPHICS_COHERENT");
  static_assert(static_cast<uint32_t>(MemoryProtection::GraphicsNocache) == PAGE_GRAPHICS_NOCACHE, "MemoryProtection::GraphicsNocache != PAGE_GRAPHICS_NOCACHE");
  static_assert(static_cast<uint32_t>(MemoryProtection::EnclaveThreadControl) == PAGE_ENCLAVE_THREAD_CONTROL, "MemoryProtection::EnclaveThreadControl != PAGE_ENCLAVE_THREAD_CONTROL");
  static_assert(static_cast<uint32_t>(MemoryProtection::RevertToFileMap) == PAGE_REVERT_TO_FILE_MAP, "MemoryProtection::RevertToFileMap != PAGE_REVERT_TO_FILE_MAP");
  static_assert(static_cast<uint32_t>(MemoryProtection::TargetsNoUpdate) == PAGE_TARGETS_NO_UPDATE, "MemoryProtection::TargetsNoUpdate != PAGE_TARGETS_NO_UPDATE");
  static_assert(static_cast<uint32_t>(MemoryProtection::TargetsInvalid) == PAGE_TARGETS_INVALID, "MemoryProtection::TargetsInvalid != PAGE_TARGETS_INVALID");
  static_assert(static_cast<uint32_t>(MemoryProtection::EnclaveUnvalidated) == PAGE_ENCLAVE_UNVALIDATED, "MemoryProtection::EnclaveUnvalidated != PAGE_ENCLAVE_UNVALIDATED");
  static_assert(static_cast<uint32_t>(MemoryProtection::EnclaveMask) == PAGE_ENCLAVE_MASK, "MemoryProtection::EnclaveMask != PAGE_ENCLAVE_MASK");
  static_assert(static_cast<uint32_t>(MemoryProtection::EnclaveDecommit) == PAGE_ENCLAVE_DECOMMIT, "MemoryProtection::EnclaveDecommit != PAGE_ENCLAVE_DECOMMIT");
  static_assert(static_cast<uint32_t>(MemoryProtection::EnclaveSsFirst) == PAGE_ENCLAVE_SS_FIRST, "MemoryProtection::EnclaveSsFirst != PAGE_ENCLAVE_SS_FIRST");
  static_assert(static_cast<uint32_t>(MemoryProtection::EnclaveSsRest) == PAGE_ENCLAVE_SS_REST, "MemoryProtection::EnclaveSsRest != PAGE_ENCLAVE_SS_REST");

  static_assert(static_cast<uint32_t>(MemoryState::Commit) == MEM_COMMIT, "MemoryState::Commit != MEM_COMMIT");
  static_assert(static_cast<uint32_t>(MemoryState::Reserve) == MEM_RESERVE, "MemoryState::Reserve != MEM_RESERVE");
  static_assert(static_cast<uint32_t>(MemoryState::ReplacePlaceholder) == MEM_REPLACE_PLACEHOLDER, "MemoryState::ReplacePlaceholder != MEM_REPLACE_PLACEHOLDER");
  static_assert(static_cast<uint32_t>(MemoryState::Decommit) == MEM_DECOMMIT, "MemoryState::Decommit != MEM_DECOMMIT");
  static_assert(static_cast<uint32_t>(MemoryState::Release) == MEM_RELEASE, "MemoryState::Release != MEM_RELEASE");
  static_assert(static_cast<uint32_t>(MemoryState::Free) == MEM_FREE, "MemoryState::Free != MEM_FREE");
  static_assert(static_cast<uint32_t>(MemoryState::Private) == MEM_PRIVATE, "MemoryState::Private != MEM_PRIVATE");
  static_assert(static_cast<uint32_t>(MemoryState::ReservePlaceholder) == MEM_RESERVE_PLACEHOLDER, "MemoryState::ReservePlaceholder != MEM_RESERVE_PLACEHOLDER");
  static_assert(static_cast<uint32_t>(MemoryState::Mapped) == MEM_MAPPED, "MemoryState::Mapped != MEM_MAPPED");
  static_assert(static_cast<uint32_t>(MemoryState::Reset) == MEM_RESET, "MemoryState::Reset != MEM_RESET");
  static_assert(static_cast<uint32_t>(MemoryState::TopDown) == MEM_TOP_DOWN, "MemoryState::TopDown != MEM_TOP_DOWN");
  static_assert(static_cast<uint32_t>(MemoryState::WriteWatch) == MEM_WRITE_WATCH, "MemoryState::WriteWatch != MEM_WRITE_WATCH");
  static_assert(static_cast<uint32_t>(MemoryState::Physical) == MEM_PHYSICAL, "MemoryState::Physical != MEM_PHYSICAL");
  static_assert(static_cast<uint32_t>(MemoryState::Rotate) == MEM_ROTATE, "MemoryState::Rotate != MEM_ROTATE");
  static_assert(static_cast<uint32_t>(MemoryState::DifferentImageBaseOk) == MEM_DIFFERENT_IMAGE_BASE_OK, "MemoryState::DifferentImageBaseOk != MEM_DIFFERENT_IMAGE_BASE_OK");
  static_assert(static_cast<uint32_t>(MemoryState::ResetUndo) == MEM_RESET_UNDO, "MemoryState::ResetUndo != MEM_RESET_UNDO");
  static_assert(static_cast<uint32_t>(MemoryState::LargePages) == MEM_LARGE_PAGES, "MemoryState::LargePages != MEM_LARGE_PAGES");
  static_assert(static_cast<uint32_t>(MemoryState::UnmapWithTransientBoost) == MEM_UNMAP_WITH_TRANSIENT_BOOST, "MemoryState::UnmapWithTransientBoost != MEM_UNMAP_WITH_TRANSIENT_BOOST");
  static_assert(static_cast<uint32_t>(MemoryState::CoalescePlaceholders) == MEM_COALESCE_PLACEHOLDERS, "MemoryState::CoalescePlaceholders != MEM_COALESCE_PLACEHOLDERS");
  static_assert(static_cast<uint32_t>(MemoryState::PreservePlaceholders) == MEM_PRESERVE_PLACEHOLDER, "MemoryState::PreservePlaceholders != MEM_PRESERVE_PLACEHOLDER");
  static_assert(static_cast<uint32_t>(MemoryState::Image) == MEM_IMAGE, "MemoryState::Image != MEM_IMAGE");
}

namespace winapi::memory
{
  auto MemoryBasicInformation::query(uintptr_t address) noexcept(false) -> winapi::memory::MemoryBasicInformation
  {
    MEMORY_BASIC_INFORMATION buf = {};
    if(not ::VirtualQuery(reinterpret_cast<LPCVOID>(address), &buf, sizeof(buf)))
      throw windows_exception(fmt::format("virtual query failed, reason: {}", last_error_string()));
    return MemoryBasicInformation {
      .base_address = reinterpret_cast<uintptr_t>(buf.BaseAddress),
      .allocation_base = reinterpret_cast<uintptr_t>(buf.AllocationBase),
      .allocation_protection = static_cast<MemoryProtection>(buf.AllocationProtect),
      .region_size = buf.RegionSize,
      .state = static_cast<MemoryState>(buf.State),
      .protection = static_cast<MemoryProtection>(buf.Protect),
      .type = static_cast<MemoryState>(buf.Type),
    };
  }

  auto MemoryBasicInformation::query(Process const& process, uintptr_t address) -> MemoryBasicInformation
  {
    MEMORY_BASIC_INFORMATION buf = {};
    if(not ::VirtualQueryEx(*process.handle(), reinterpret_cast<LPCVOID>(address), &buf, sizeof(buf)))
      throw windows_exception(fmt::format("virtual query failed, reason: {}", last_error_string()));
    return MemoryBasicInformation {
      .base_address = reinterpret_cast<uintptr_t>(buf.BaseAddress),
      .allocation_base = reinterpret_cast<uintptr_t>(buf.AllocationBase),
      .allocation_protection = static_cast<MemoryProtection>(buf.AllocationProtect),
      .region_size = buf.RegionSize,
      .state = static_cast<MemoryState>(buf.State),
      .protection = static_cast<MemoryProtection>(buf.Protect),
      .type = static_cast<MemoryState>(buf.Type),
    };
  }

  auto MemoryBasicInformation::try_query(uintptr_t address) -> optional<MemoryBasicInformation> {
    try {
      return MemoryBasicInformation::query(address);
    } catch(...) {
      return nullopt;
    }
  }

  auto MemoryBasicInformation::try_query(Process const& process, uintptr_t address) -> optional<MemoryBasicInformation> {
    try {
      return MemoryBasicInformation::query(process, address);
    } catch(...) {
      return nullopt;
    }
  }
} // namespace winapi::memory