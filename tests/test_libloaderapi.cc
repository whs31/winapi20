#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <winapi20/impl/libloaderapi_impl.h>
#include <winapi20/impl/tlhelp32_impl.h>
#include <winapi20/impl/errhandlingapi_impl.h>
#include <winapi20/detail/windows_headers.h>

using namespace std;
using namespace ::testing;
using namespace winapi;
using namespace winapi;
using namespace winapi::th32;

TEST(Library, View)
{
  using IncludeFlags = Snapshot::IncludeFlags;

  auto snapshot = th32::Snapshot(IncludeFlags::Process, PID::current());
  auto pid = snapshot.find_first_by_name<th32::ProcessEntry>("explorer.exe");
  EXPECT_TRUE(snapshot.valid());
  EXPECT_TRUE(pid.has_value());
  EXPECT_FALSE(pid->pid.is_current());

  auto snapshot2 = th32::Snapshot(IncludeFlags::Module | IncludeFlags::Process, pid->pid);
  auto module = snapshot2.find_first_by_name<th32::ModuleEntry>("kernel32.dll");
  EXPECT_TRUE(snapshot2.valid());
  EXPECT_TRUE(module.has_value());
  EXPECT_TRUE(module->base_address > 0);
  EXPECT_TRUE(module->pid == pid->pid);
  EXPECT_FALSE(module->name.empty());
  EXPECT_FALSE(module->path.empty());
  EXPECT_TRUE(module->handle);

  auto lib = dll::Library::view("kernel32.dll", pid->pid);
  EXPECT_TRUE(lib.has_value());
  auto fn = (*lib)["MulDiv"];
  EXPECT_TRUE(fn.has_value());
  auto res = fn->operator()<int>(10, 2, 5);
  EXPECT_EQ(res, ::MulDiv(10, 2, 5));
}