#include <gtest/gtest.h>
#include <winapi20/impl/tlhelp32_impl.h>
#include <winapi20/detail/windows_headers.h>
#include <winapi20/detail/cvt.h>

using namespace std;
using namespace winapi;
using enum CaseSensitivity;
using enum th32::Snapshot::IncludeFlags;

TEST(TLHelp32, Snapshot)
{
  auto snapshot = th32::Snapshot(winapi::th32::Snapshot::IncludeFlags::Process, PID::current());
  EXPECT_TRUE(snapshot.valid());
}

TEST(TLHelp32, FindProcessByName)
{
  auto snapshot = th32::Snapshot(Process, PID::current());
  auto pid = snapshot.find_first_by_name<th32::ProcessEntry>("ExPloRer.exe");
  EXPECT_TRUE(snapshot.valid());
  EXPECT_TRUE(pid.has_value());
  EXPECT_FALSE(pid->pid.is_current());

  EXPECT_FALSE(snapshot.find_last_by_name<th32::ProcessEntry>("ExPloRer.exe", CaseSensitive).has_value());
  EXPECT_TRUE(snapshot.find_by_name<th32::ProcessEntry>("ExPloRer.exe", CaseSensitive).empty());
}

TEST(TLHelp32, FindModuleByName)
{
  auto snapshot = th32::Snapshot(Process, PID::current());
  auto pid = snapshot.find_first_by_name<th32::ProcessEntry>("explorer.exe");
  EXPECT_TRUE(snapshot.valid());
  EXPECT_TRUE(pid.has_value());
  EXPECT_FALSE(pid->pid.is_current());

  auto snapshot2 = th32::Snapshot(Module | Process, pid->pid);
  auto module = snapshot2.find_first_by_name<th32::ModuleEntry>("kernel32.dll");
  auto modules = snapshot2.modules();
  EXPECT_TRUE(module.has_value());
  EXPECT_TRUE(module->base_address > 0);
  EXPECT_FALSE(modules.empty());
  for(auto& m : modules) {
    EXPECT_TRUE(m.base_address > 0);
    EXPECT_TRUE(m.size > 0);
    EXPECT_TRUE(m.pid == pid->pid);
    EXPECT_FALSE(m.name.empty());
    EXPECT_FALSE(m.path.empty());
    EXPECT_TRUE(m.handle);
  }
}