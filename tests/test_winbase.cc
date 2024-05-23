#include <gtest/gtest.h>
#include <winapi20/impl/winbase_impl.h>
#include <winapi20/impl/errhandlingapi_impl.h>
#include <winapi20/detail/windows_headers.h>

TEST(Winbase, GetCurrentHwProfile)
{
  auto expected = HW_PROFILE_INFO();
  GetCurrentHwProfileW(nullptr);

  auto got = winapi::HWProfileInfo::get();
  EXPECT_EQ(got.name, std::wstring(expected.szHwProfileName));
  EXPECT_EQ(got.dock_info, winapi::HWProfileInfo::DockInfo(expected.dwDockInfo));
  EXPECT_EQ(got.guid, std::wstring(expected.szHwProfileGuid));
  throw winapi::windows_exception(winapi::last_error_string());
}