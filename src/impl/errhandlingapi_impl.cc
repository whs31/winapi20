#include <winapi20/impl/errhandlingapi_impl.h>
#include <memory>
#include <winapi20/detail/windows_headers.h>
#include <utility/utility.hh>

using std::string;

namespace winapi
{
  auto last_error() noexcept -> uint32_t { return GetLastError(); }
  auto last_error_string() noexcept -> string
  {
    auto error = last_error();
    if(error == 0)
      return {};
    auto buf = std::make_unique<char[]>(256);
    if(!FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, error, 0, buf.get(), 256, nullptr))
      return {};
    return {buf.get()};
  }
}