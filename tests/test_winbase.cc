#include <gtest/gtest.h>
#include <winapi20/impl/winbase_impl.h>
#include <winapi20/impl/errhandlingapi_impl.h>
#include <winapi20/detail/windows_headers.h>

using namespace std;
namespace wd = winapi::detail;

TEST(Winbase, GetCurrentHwProfile)
{
  auto expected = HW_PROFILE_INFO();
  GetCurrentHwProfileW(&expected);

  auto got = winapi::HWProfileInfo::get();
  EXPECT_EQ(got.name, wd::into_utf8(expected.szHwProfileName));
  EXPECT_EQ(got.dock_info, winapi::HWProfileInfo::DockInfo(expected.dwDockInfo));
  EXPECT_EQ(got.guid, wd::into_utf8(expected.szHwProfileGuid));
}