#include <gtest/gtest.h>
#include <winapi20/impl/consoleapi_impl.h>
#include <winapi20/detail/windows_headers.h>
#include <winapi20/detail/cvt.h>

using namespace std;
namespace wd = winapi::detail;

TEST(ConsoleAPI, ConsoleHost)
{
  for(auto i = 0; i < 2; ++i) {
    try {
      auto host = winapi::ConsoleHost(winapi::ConsoleHost::Mode::All);

      EXPECT_TRUE(host.is_attached());
      EXPECT_NO_THROW(host.set_title("winapi20 test console host"));
      EXPECT_EQ(host.title(), "winapi20 test console host");

      cout << "Test is ok" << i << endl;
      cin.get();
    } catch(winapi::windows_exception const& e) {
      GTEST_FATAL_FAILURE_(e.what());
    }
  }
  GTEST_SUCCEED();
}