#include <winapi20/impl/winbase_impl.h>

#ifdef WINAPI20_ENABLED

#include <winapi20/impl/errhandlingapi_impl.h>
#include <winapi20/detail/windows_headers.h>

using std::wstring;

namespace winapi
{
  auto HWProfileInfo::get() noexcept(false) -> HWProfileInfo
  {
    auto info = HW_PROFILE_INFO();
    if(not GetCurrentHwProfileW(&info))
      throw windows_exception(last_error_string());
    auto res = HWProfileInfo();
    res.dock_info = static_cast<DockInfo>(info.dwDockInfo);
    res.guid = wstring(info.szHwProfileGuid);
    res.name = wstring(info.szHwProfileName);
    return res;
  }
}

#endif // WINAPI20_ENABLED