#include <winapi20/wrappers/handle.h>
#include <winapi20/detail/windows_headers.h>

namespace winapi
{
  Handle::Handle(Handle::integer_type handle, Cleanup cleanup)
    : detail::PointerLike(handle)
    , m_cleanup(cleanup)
  {}

  Handle::~Handle() {
    if(this->valid() and this->m_cleanup == Cleanup::Auto)
      ::CloseHandle(this->m_);
  }
}