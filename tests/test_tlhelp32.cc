#include <gtest/gtest.h>
#include <winapi20/impl/tlhelp32_impl.h>
#include <winapi20/detail/windows_headers.h>
#include <winapi20/detail/cvt.h>

using namespace std;
namespace wd = winapi::detail;

TEST(TLHelp32, Snapshot)
{
  auto snapshot = winapi::th32::Snapshot(winapi::th32::Snapshot::IncludeFlags::Process, 0);

  EXPECT_TRUE(snapshot.valid());
}

TEST(TLHelp32, FindProcessByName)
{
  using enum winapi::th32::Snapshot::CaseSensitivity;

  auto snapshot = winapi::th32::Snapshot(winapi::th32::Snapshot::IncludeFlags::Process, 0);

  EXPECT_TRUE(snapshot.valid());
  auto pid = snapshot.process_by_name("ExPloRer.exe", CaseInsensitive);

  EXPECT_TRUE(pid.has_value());
}