#include <winapi20/impl/errhandlingapi_impl.h>

#ifdef WINAPI20_ENABLED

#include <array>
#include <winapi20/detail/windows_headers.h>
#include <winapi20/detail/cvt.h>
#include <utility/utility.hh>

using std::array;
using std::string;
using std::wstring;

namespace winapi
{
  auto last_error() noexcept -> uint32_t { return GetLastError(); }
  auto last_error_string() noexcept -> string
  {
    auto error = last_error();
    if(error == 0)
      return {};
    auto buf = array<wchar_t, 256>();
    if(not FormatMessageW(
      FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr,
      error,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      buf.data(),
      (sizeof(buf) / sizeof(wchar_t)),
      nullptr
    ))
      return {};
    return detail::into<string>(wstring(buf.data()));
  }
}

#endif // WINAPI20_ENABLED