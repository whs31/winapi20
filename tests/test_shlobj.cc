#include <gtest/gtest.h>
#include <winapi20/impl/shlobj_core_impl.h>
#include <winapi20/impl/errhandlingapi_impl.h>
#include <winapi20/detail/windows_headers.h>

using namespace std;
namespace wd = winapi::detail;

TEST(Shlobj, SHGetKnownFolderPath)
{
  auto expected = PWSTR();
  SHGetKnownFolderPath(FOLDERID_Windows, 0, nullptr, &expected);

  auto got = winapi::shell::known_folder_path(winapi::shell::FolderID::Windows);

  EXPECT_EQ(got.string(), filesystem::path(expected).string());
  EXPECT_EQ(got.string(), "C:\\Windows");

  CoTaskMemFree(expected);
}