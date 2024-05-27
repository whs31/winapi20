#include <winapi20/wrappers/handle.h>
#include <winapi20/wrappers/memaddr.h>

#include <optional>
#include <winapi20/detail/cvt.h>
#include <winapi20/detail/windows_headers.h>

using std::optional;
using std::nullopt;

namespace winapi
{
  Handle::Handle(Handle::integer_type handle, Cleanup cleanup)
    : detail::PointerLike(handle)
    , m_cleanup(cleanup)
  {}

  Handle::~Handle() {
    if(this->Handle::valid() and this->m_cleanup == Cleanup::Auto)
      ::CloseHandle(this->m_);
  }

  auto MemoryAddress::permissions(memory::MemoryProtection const protection) const -> bool {
    using enum memory::MemoryProtection;
    using enum memory::MemoryState;

    auto const mem_info = memory::MemoryBasicInformation::try_query(this->as_integer());
    if(not mem_info.has_value())
      return false;
    if(not bool(mem_info->state & Commit))
      return false;
    if(not bool(mem_info->protection & protection))
      return false;
    if(not bool(mem_info->protection & (Guard | NoAccess)))
      return false;
    return true;
  }
}