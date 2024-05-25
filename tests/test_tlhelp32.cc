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
  using enum winapi::th32::Snapshot::IncludeFlags;

  auto snapshot = winapi::th32::Snapshot(Process, 0);
  auto pid = snapshot.find_by_name<winapi::th32::ProcessEntry>("ExPloRer.exe", CaseInsensitive);
  EXPECT_TRUE(snapshot.valid());
  EXPECT_TRUE(pid.has_value());
}

TEST(TLHelp32, FindModuleByName)
{
  using enum winapi::th32::Snapshot::CaseSensitivity;
  using enum winapi::th32::Snapshot::IncludeFlags;
  using namespace winapi::enums;

  auto snapshot = winapi::th32::Snapshot(Process, 0);
  auto pid = snapshot.find_by_name<winapi::th32::ProcessEntry>("explorer.exe", CaseInsensitive);
  EXPECT_TRUE(snapshot.valid());
  EXPECT_TRUE(pid.has_value());

  auto snapshot2 = winapi::th32::Snapshot(Module | Process, pid->pid);
  auto module = snapshot2.find_by_name<winapi::th32::ModuleEntry>("kernel32.dll", CaseInsensitive);
  EXPECT_TRUE(module.has_value());
  EXPECT_TRUE(module->base_address > 0);
}