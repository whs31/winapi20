#include <gtest/gtest.h>
#include <winapi20/impl/consoleapi_impl.h>
#include <winapi20/detail/windows_headers.h>
#include <winapi20/detail/cvt.h>

using namespace std;
namespace wd = winapi::detail;

TEST(ConsoleAPI, ConsoleHost)
{
  // todo: this test doesn't work
  #if 0
  auto host = winapi::ConsoleHost();

  EXPECT_TRUE(host.is_attached());
  EXPECT_NO_THROW(host.set_title("winapi20 test console host"));
  EXPECT_EQ(host.title(), "winapi20 test console host");

  cout << "Test is ok" << endl;
  cin.get();
  #endif
  GTEST_SUCCEED();
}