#include <winapi20/impl/winbase_impl.h>
#include <winapi20/impl/errhandlingapi_impl.h>
#include <winapi20/detail/windows_headers.h>

using std::string;

namespace winapi
{
  auto HWProfileInfo::get() noexcept(false) -> HWProfileInfo
  {
    auto info = HW_PROFILE_INFO();
    if(not GetCurrentHwProfileA(&info))
      throw windows_exception(last_error_string());
    auto res = HWProfileInfo();
    res.dock_info = static_cast<DockInfo>(info.dwDockInfo);
    res.guid = string(info.szHwProfileGuid);
    res.name = string(info.szHwProfileName);
    return res;
  }
}