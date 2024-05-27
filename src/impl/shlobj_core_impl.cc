#include <winapi20/impl/shlobj_core_impl.h>

#ifdef WINAPI20_ENABLED

#if defined(_MSC_VER)
# if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#   define _AMD64_
# elif defined(i386) || defined(__i386) || defined(__i386__) || defined(__i386__) || defined(_M_IX86)
#   define _X86_
# elif defined(__arm__) || defined(_M_ARM) || defined(_M_ARMT)
#   define _ARM_
# endif
# include <Shlobj.h>
#endif // _MSC_VER

#include <winapi20/impl/errhandlingapi_impl.h>
#include <winapi20/detail/windows_headers.h>
#include <winapi20/detail/exception.h>

namespace fs = std::filesystem;

namespace
{
  [[nodiscard]] auto folder_id_to_winapi_folder_id(winapi::shell::FolderID id) noexcept -> KNOWNFOLDERID
  {
    switch(id) {
      case winapi::shell::FolderID::AccountPictures: return FOLDERID_AccountPictures;
      case winapi::shell::FolderID::AddNewPrograms: return FOLDERID_AddNewPrograms;
      case winapi::shell::FolderID::AdminTools: return FOLDERID_AdminTools;
      case winapi::shell::FolderID::AppDataDesktop: return FOLDERID_Desktop;
      case winapi::shell::FolderID::AppDataDocuments: return FOLDERID_Documents;
      case winapi::shell::FolderID::AppDataFavorites: return FOLDERID_Favorites;
      case winapi::shell::FolderID::AppDataProgramData: return FOLDERID_ProgramData;
      case winapi::shell::FolderID::ApplicationShortcuts: return FOLDERID_ApplicationShortcuts;
      case winapi::shell::FolderID::AppsFolder: return FOLDERID_AppsFolder;
      case winapi::shell::FolderID::AppUpdates: return FOLDERID_AppUpdates;
      case winapi::shell::FolderID::CameraRoll: return FOLDERID_CameraRoll;
      case winapi::shell::FolderID::CDBurning: return FOLDERID_CDBurning;
      case winapi::shell::FolderID::ChangeRemovePrograms: return FOLDERID_ChangeRemovePrograms;
      case winapi::shell::FolderID::CommonAdminTools: return FOLDERID_AdminTools;
      case winapi::shell::FolderID::CommonOEMLinks: return FOLDERID_CommonOEMLinks;
      case winapi::shell::FolderID::CommonPrograms: return FOLDERID_CommonPrograms;
      case winapi::shell::FolderID::CommonStartMenu: return FOLDERID_CommonStartMenu;
      case winapi::shell::FolderID::CommonStartup: return FOLDERID_CommonStartup;
      case winapi::shell::FolderID::CommonTemplates: return FOLDERID_CommonTemplates;
      case winapi::shell::FolderID::ComputerFolder: return FOLDERID_ComputerFolder;
      case winapi::shell::FolderID::ConflictFolder: return FOLDERID_ConflictFolder;
      case winapi::shell::FolderID::ConnectionsFolder: return FOLDERID_ConnectionsFolder;
      case winapi::shell::FolderID::Contacts: return FOLDERID_Contacts;
      case winapi::shell::FolderID::ControlPanelFolder: return FOLDERID_ControlPanelFolder;
      case winapi::shell::FolderID::Cookies: return FOLDERID_Cookies;
      case winapi::shell::FolderID::Desktop: return FOLDERID_Desktop;
      case winapi::shell::FolderID::DeviceMetadataStore: return FOLDERID_DeviceMetadataStore;
      case winapi::shell::FolderID::Documents: return FOLDERID_Documents;
      case winapi::shell::FolderID::DocumentsLibrary: return FOLDERID_DocumentsLibrary;
      case winapi::shell::FolderID::Downloads: return FOLDERID_Downloads;
      case winapi::shell::FolderID::Favorites: return FOLDERID_Favorites;
      case winapi::shell::FolderID::Fonts: return FOLDERID_Fonts;
      case winapi::shell::FolderID::Games: return FOLDERID_Games;
      case winapi::shell::FolderID::GameTasks: return FOLDERID_GameTasks;
      case winapi::shell::FolderID::History: return FOLDERID_History;
      case winapi::shell::FolderID::HomeGroup: return FOLDERID_HomeGroup;
      case winapi::shell::FolderID::HomeGroupCurrentUser: return FOLDERID_HomeGroupCurrentUser;
      case winapi::shell::FolderID::ImplicitAppShortcuts: return FOLDERID_ImplicitAppShortcuts;
      case winapi::shell::FolderID::InternetCache: return FOLDERID_InternetCache;
      case winapi::shell::FolderID::InternetFolder: return FOLDERID_InternetFolder;
      case winapi::shell::FolderID::Libraries: return FOLDERID_Libraries;
      case winapi::shell::FolderID::Links: return FOLDERID_Links;
      case winapi::shell::FolderID::LocalAppData: return FOLDERID_LocalAppData;
      case winapi::shell::FolderID::LocalAppDataLow: return FOLDERID_LocalAppDataLow;
      case winapi::shell::FolderID::LocalizedResourcesDir: return FOLDERID_LocalizedResourcesDir;
      case winapi::shell::FolderID::Music: return FOLDERID_Music;
      case winapi::shell::FolderID::MusicLibrary: return FOLDERID_MusicLibrary;
      case winapi::shell::FolderID::NetHood: return FOLDERID_NetHood;
      case winapi::shell::FolderID::NetworkFolder: return FOLDERID_NetworkFolder;
      case winapi::shell::FolderID::Objects3D: return FOLDERID_Objects3D;
      case winapi::shell::FolderID::OriginalImages: return FOLDERID_OriginalImages;
      case winapi::shell::FolderID::PhotoAlbums: return FOLDERID_PhotoAlbums;
      case winapi::shell::FolderID::PicturesLibrary: return FOLDERID_PicturesLibrary;
      case winapi::shell::FolderID::Pictures: return FOLDERID_Pictures;
      case winapi::shell::FolderID::Playlists: return FOLDERID_Playlists;
      case winapi::shell::FolderID::PrintersFolder: return FOLDERID_PrintersFolder;
      case winapi::shell::FolderID::PrintHood: return FOLDERID_PrintHood;
      case winapi::shell::FolderID::Profile: return FOLDERID_Profile;
      case winapi::shell::FolderID::ProgramData: return FOLDERID_ProgramData;
      case winapi::shell::FolderID::ProgramFiles: return FOLDERID_ProgramFiles;
      case winapi::shell::FolderID::ProgramFilesX64: return FOLDERID_ProgramFilesX64;
      case winapi::shell::FolderID::ProgramFilesX86: return FOLDERID_ProgramFilesX86;
      case winapi::shell::FolderID::ProgramFilesCommon: return FOLDERID_ProgramFilesCommon;
      case winapi::shell::FolderID::ProgramFilesCommonX64: return FOLDERID_ProgramFilesCommonX64;
      case winapi::shell::FolderID::ProgramFilesCommonX86: return FOLDERID_ProgramFilesCommonX86;
      case winapi::shell::FolderID::Programs: return FOLDERID_Programs;
      case winapi::shell::FolderID::Public: return FOLDERID_Public;
      case winapi::shell::FolderID::PublicDesktop: return FOLDERID_PublicDesktop;
      case winapi::shell::FolderID::PublicDocuments: return FOLDERID_PublicDocuments;
      case winapi::shell::FolderID::PublicDownloads: return FOLDERID_PublicDownloads;
      case winapi::shell::FolderID::PublicGameTasks: return FOLDERID_PublicGameTasks;
      case winapi::shell::FolderID::PublicLibraries: return FOLDERID_PublicLibraries;
      case winapi::shell::FolderID::PublicMusic: return FOLDERID_PublicMusic;
      case winapi::shell::FolderID::PublicPictures: return FOLDERID_PublicPictures;
      case winapi::shell::FolderID::PublicRingtones: return FOLDERID_PublicRingtones;
      case winapi::shell::FolderID::PublicUserTiles: return FOLDERID_PublicUserTiles;
      case winapi::shell::FolderID::PublicVideos: return FOLDERID_PublicVideos;
      case winapi::shell::FolderID::QuickLaunch: return FOLDERID_QuickLaunch;
      case winapi::shell::FolderID::Recent: return FOLDERID_Recent;
      case winapi::shell::FolderID::RecordedTV: return FOLDERTYPEID_RecordedTV;
      case winapi::shell::FolderID::RecordedTVLibrary: return FOLDERID_RecordedTVLibrary;
      case winapi::shell::FolderID::RecycleBinFolder: return FOLDERID_RecycleBinFolder;
      case winapi::shell::FolderID::ResourceDir: return FOLDERID_ResourceDir;
      case winapi::shell::FolderID::Ringtones: return FOLDERID_Ringtones;
      case winapi::shell::FolderID::RoamingAppData: return FOLDERID_RoamingAppData;
      case winapi::shell::FolderID::RoamedTileImages: return FOLDERID_RoamedTileImages;
      case winapi::shell::FolderID::RoamingTiles: return FOLDERID_RoamingTiles;
      case winapi::shell::FolderID::SampleMusic: return FOLDERID_SampleMusic;
      case winapi::shell::FolderID::SamplePictures: return FOLDERID_SamplePictures;
      case winapi::shell::FolderID::SamplePlaylists: return FOLDERID_SamplePlaylists;
      case winapi::shell::FolderID::SampleVideos: return FOLDERID_SampleVideos;
      case winapi::shell::FolderID::SavedGames: return FOLDERID_SavedGames;
      case winapi::shell::FolderID::SavedPictures: return FOLDERID_SavedPictures;
      case winapi::shell::FolderID::SavedPicturesLibrary: return FOLDERID_SavedPicturesLibrary;
      case winapi::shell::FolderID::SavedSearches: return FOLDERID_SavedSearches;
      case winapi::shell::FolderID::Screenshots: return FOLDERID_Screenshots;
      case winapi::shell::FolderID::Searches: return FOLDERID_SavedSearches; // todo
      case winapi::shell::FolderID::SearchHistory: return FOLDERID_SearchHistory;
      case winapi::shell::FolderID::SearchHome: return FOLDERID_SearchHome;
      case winapi::shell::FolderID::SearchTemplates: return FOLDERID_SearchTemplates;
      case winapi::shell::FolderID::SendTo: return FOLDERID_SendTo;
      case winapi::shell::FolderID::SidebarDefaultParts: return FOLDERID_SidebarDefaultParts;
      case winapi::shell::FolderID::SidebarParts: return FOLDERID_SidebarParts;
      case winapi::shell::FolderID::SkyDrive: return FOLDERID_SkyDrive;
      case winapi::shell::FolderID::SkyDriveCameraRoll: return FOLDERID_SkyDriveCameraRoll;
      case winapi::shell::FolderID::SkyDriveDocuments: return FOLDERID_SkyDriveDocuments;
      case winapi::shell::FolderID::SkyDrivePicturesLibrary: return FOLDERID_SkyDrivePictures;
      case winapi::shell::FolderID::StartMenu: return FOLDERID_StartMenu;
      case winapi::shell::FolderID::Startup: return FOLDERID_Startup;
      case winapi::shell::FolderID::SyncManagerFolder: return FOLDERID_SyncManagerFolder;
      case winapi::shell::FolderID::SyncResultsFolder: return FOLDERID_SyncResultsFolder;
      case winapi::shell::FolderID::SyncSetupFolder: return FOLDERID_SyncSetupFolder;
      case winapi::shell::FolderID::System: return FOLDERID_System;
      case winapi::shell::FolderID::SystemX86: return FOLDERID_SystemX86;
      case winapi::shell::FolderID::Templates: return FOLDERID_Templates;
      case winapi::shell::FolderID::TreeProperties: return {}; // todo
      case winapi::shell::FolderID::UserPinned: return FOLDERID_UserPinned;
      case winapi::shell::FolderID::UserProfiles: return FOLDERID_UserProfiles;
      case winapi::shell::FolderID::UserProgramFiles: return FOLDERID_UserProgramFiles;
      case winapi::shell::FolderID::UserProgramFilesCommon: return FOLDERID_UserProgramFilesCommon;
      case winapi::shell::FolderID::UsersFiles: return FOLDERID_UsersFiles;
      case winapi::shell::FolderID::UsersLibraries: return FOLDERID_UsersLibraries;
      case winapi::shell::FolderID::Videos: return FOLDERID_Videos;
      case winapi::shell::FolderID::VideosLibrary: return FOLDERID_VideosLibrary;
      case winapi::shell::FolderID::Windows: return FOLDERID_Windows;
      default: return {};
    }
  }
}

namespace winapi::shell
{
  auto known_folder_path(
    FolderID id,
    KnownFolderFlag flags,
    void* user_handle
  ) noexcept(false) -> fs::path
  {
    auto buf = PWSTR();
    auto const res = SHGetKnownFolderPath(
      ::folder_id_to_winapi_folder_id(id),
      static_cast<DWORD>(flags),
      user_handle,
      &buf
    );
    if(res != S_OK) {
      CoTaskMemFree(buf);
      throw winapi::windows_exception("SHGetKnownFolderPath returned error");
    }
    auto const path = fs::path(buf);
    CoTaskMemFree(buf);
    return path;
  }
} // namespace winapi::shell

#endif // WINAPI20_ENABLED