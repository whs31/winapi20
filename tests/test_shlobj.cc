#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <winapi20/impl/shlobj_core_impl.h>
#include <winapi20/impl/errhandlingapi_impl.h>
#include <winapi20/detail/windows_headers.h>

using winapi::enums::operator|;
using namespace std;
using namespace ::testing;
namespace wd = winapi::detail;

TEST(Shlobj, SHGetKnownFolderPath)
{
  auto expected = PWSTR();
  SHGetKnownFolderPath(FOLDERID_Windows, 0, nullptr, &expected);

  auto got = winapi::shell::known_folder_path(
    winapi::shell::FolderID::Windows,
    winapi::shell::KnownFolderFlag::DefaultPath | winapi::shell::KnownFolderFlag::NoAlias
  );

  EXPECT_EQ(got.string(), filesystem::path(expected).string());
  EXPECT_THAT(got.string(), AnyOf("C:\\Windows", "C:\\windows"));

  CoTaskMemFree(expected);
}