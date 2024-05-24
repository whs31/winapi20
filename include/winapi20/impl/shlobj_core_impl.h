#pragma once

#include <cstdint>
#include <filesystem>
#include <winapi20/detail/export.h>
#include <winapi20/detail/cvt.h>

namespace winapi::shell
{
  enum class FolderID
  {
    AccountPictures,
    AddNewPrograms,
    AdminTools,
    AppDataDesktop,
    AppDataDocuments,
    AppDataFavorites,
    AppDataProgramData,
    ApplicationShortcuts,
    AppsFolder,
    AppUpdates,
    CameraRoll,
    CDBurning,
    ChangeRemovePrograms,
    CommonAdminTools,
    CommonOEMLinks,
    CommonPrograms,
    CommonStartMenu,
    CommonStartup,
    CommonTemplates,
    ComputerFolder,
    ConflictFolder,
    ConnectionsFolder,
    Contacts,
    ControlPanelFolder,
    Cookies,
    Desktop,
    DeviceMetadataStore,
    Documents,
    DocumentsLibrary,
    Downloads,
    Favorites,
    Fonts,
    Games,
    GameTasks,
    History,
    HomeGroup,
    HomeGroupCurrentUser,
    ImplicitAppShortcuts,
    InternetCache,
    InternetFolder,
    Libraries,
    Links,
    LocalAppData,
    LocalAppDataLow,
    LocalizedResourcesDir,
    Music,
    MusicLibrary,
    NetHood,
    NetworkFolder,
    Objects3D,
    OriginalImages,
    PhotoAlbums,
    PicturesLibrary,
    Pictures,
    Playlists,
    PrintersFolder,
    PrintHood,
    Profile,
    ProgramData,
    ProgramFiles,
    ProgramFilesX64,
    ProgramFilesX86,
    ProgramFilesCommon,
    ProgramFilesCommonX64,
    ProgramFilesCommonX86,
    Programs,
    Public,
    PublicDesktop,
    PublicDocuments,
    PublicDownloads,
    PublicGameTasks,
    PublicLibraries,
    PublicMusic,
    PublicPictures,
    PublicRingtones,
    PublicUserTiles,
    PublicVideos,
    QuickLaunch,
    Recent,
    RecordedTV,
    RecordedTVLibrary,
    RecycleBinFolder,
    ResourceDir,
    Ringtones,
    RoamingAppData,
    RoamedTileImages,
    RoamingTiles,
    SampleMusic,
    SamplePictures,
    SamplePlaylists,
    SampleVideos,
    SavedGames,
    SavedPictures,
    SavedPicturesLibrary,
    SavedSearches,
    Screenshots,
    Searches,
    SearchHistory,
    SearchHome,
    SearchTemplates,
    SendTo,
    SidebarDefaultParts,
    SidebarParts,
    SkyDrive,
    SkyDriveCameraRoll,
    SkyDriveDocuments,
    SkyDrivePicturesLibrary,
    StartMenu,
    Startup,
    SyncManagerFolder,
    SyncResultsFolder,
    SyncSetupFolder,
    System,
    SystemX86,
    Templates,
    TreeProperties,
    UserPinned,
    UserProfiles,
    UserProgramFiles,
    UserProgramFilesCommon,
    UsersFiles,
    UsersLibraries,
    Videos,
    VideosLibrary,
    Windows
  };

  /**
   * \brief Defines constants that specify special retrieval options for known folders.
   * \details Defines constants that specify special retrieval options for known folders
   * (for example, for use when calling the SHGetKnownFolderIDList function to retrieve the path
   * of a known folder). These values supersede CSIDL values, which have parallel meanings.
   * \see https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/ne-shlobj_core-known_folder_flag
   */
  enum class KnownFolderFlag : uint32_t
  {
    Default                       = 0,
    ForceAppdataRedirection       = 0x00080000,
    ReturnFilterRedirectionTarget = 0x00040000,
    ForcePackageRedirection       = 0x00020000,
    NoPackageRedirection          = 0x00010000,
    ForceAppcontainerRedirection  = 0x00020000,
    NoAppcontainerRedirection     = 0x00010000,
    Create                        = 0x00008000,
    DontVerify                    = 0x00004000,
    DontUnexpand                  = 0x00002000,
    NoAlias                       = 0x00001000,
    Init                          = 0x00000800,
    DefaultPath                   = 0x00000400,
    NotParentRelative             = 0x00000200,
    SimpleIdlist                  = 0x00000100,
    AliasOnly                     = 0x80000000
  };

  namespace detail
  {
    [[maybe_unused]] void consteval enable_bitand(KnownFolderFlag);
    [[maybe_unused]] void consteval enable_bitor(KnownFolderFlag);
  }

  using enums::operator&;
  using enums::operator|;

  [[nodiscard]] WINAPI20_EXPORT auto known_folder_path(
      FolderID id,
      KnownFolderFlag flags = KnownFolderFlag::Default,
      void* user_handle = nullptr
  ) noexcept(false) -> std::filesystem::path;
} // namespace winapi::shell