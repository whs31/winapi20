#include <gtest/gtest.h>
#include <winapi20/impl/errhandlingapi_impl.h>
#include <winapi20/detail/windows_headers.h>
#include <winapi20/detail/cvt.h>

using namespace std;
namespace wd = winapi::detail;

TEST(ErrhandlingAPI, GetLastError)
{
  SetLastError(ERROR_CRC);

  EXPECT_EQ(winapi::last_error(), ERROR_CRC);
  EXPECT_FALSE(winapi::last_error_string().empty());
  EXPECT_TRUE(winapi::last_error_string().find('\n') != string::npos);
}

TEST(ErrhandlingAPI, FatalApplicationExit)
{
  #if 0
  // todo: this test must be disabled
  winapi::fatal_application_exit("Test message");
  EXPECT_EQ(winapi::last_error(), ERROR_FATAL_APP_EXIT);
  EXPECT_FALSE(winapi::last_error_string().empty());
  #else
  GTEST_SUCCESS_("Skipped");
  #endif
}