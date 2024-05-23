#pragma once

#include <winapi20/detail/export.h>

#if defined(WINAPI20_ENABLED)
# ifndef _WIN32_IE // NOLINT(*-reserved-identifier)
#   define _WIN32_IE 0x0400 // NOLINT(*-reserved-identifier)
# endif // _WIN32_IE

# ifndef _WIN32_WINNT // NOLINT(*-reserved-identifier)
#   define _WIN32_WINNT 0x0400 // NOLINT(*-reserved-identifier)
# endif // _WIN32_WINNT

# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <commctrl.h>
# include <commdlg.h>
# include <shlobj.h>
# include <tchar.h>

# if defined(min)
#   undef min
# endif // min
# if defined(max)
#   undef max
# endif // max

extern HINSTANCE default_module_handle;
# ifndef __MODULE_HANDLE // NOLINT(*-reserved-identifier)
#   define __MODULE_HANDLE default_module_handle // NOLINT(*-reserved-identifier)
# endif // __MODULE_HANDLE
#endif // WINAPI20_ENABLED