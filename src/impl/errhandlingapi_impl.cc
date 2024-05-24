#include <winapi20/impl/errhandlingapi_impl.h>

#ifdef WINAPI20_ENABLED

#include <array>
#include <string>
#include <winapi20/detail/windows_headers.h>
#include <winapi20/detail/cvt.h>

using std::array;
using std::string;
using std::string_view;
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

  auto fatal_application_exit(string_view message) noexcept -> void {
    FatalAppExitW(0, detail::into<wstring>(message).data());
  }
}

#endif // WINAPI20_ENABLED