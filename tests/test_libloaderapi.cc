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

  auto lib = dll::Library(dll::Library::View, "kernel32.dll", pid->pid);

  EXPECT_EQ(lib.file_path().filename().string(), "KERNEL32.DLL");
  fmt::print("{}\n", lib.file_path().string());

  auto fn = lib["MulDiv"];
  EXPECT_TRUE(fn.has_value());
  auto res = fn->call<int>(10, 2, 5);
  int res2 = (*fn).operator()<int>(10, 2, 5);
  EXPECT_EQ(res, ::MulDiv(10, 2, 5));
  EXPECT_EQ(res2, res);

  auto ntdll = dll::Library(dll::Library::View, "ntdll.dll", PID::current());
  auto ntdll_fn = ntdll["NtProtectVirtualMemory"];
  EXPECT_TRUE(ntdll_fn.has_value());
}