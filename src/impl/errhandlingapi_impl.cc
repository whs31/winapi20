#include <winapi20/impl/errhandlingapi_impl.h>

#ifdef WINAPI20_ENABLED

#include <memory>
#include <winapi20/detail/windows_headers.h>
#include <utility/utility.hh>

using std::wstring;

namespace winapi
{
  auto last_error() noexcept -> uint32_t { return GetLastError(); }
  auto last_error_string() noexcept -> wstring
  {
    auto error = last_error();
    if(error == 0)
      return {};
    wchar_t buf[256];
    if(not FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, (sizeof(buf) / sizeof(wchar_t)), nullptr))
      return {};
    return {buf};
  }
}

#endif // WINAPI20_ENABLED