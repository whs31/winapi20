#pragma once

#if defined(_WIN32)
# ifndef _WIN32_IE // NOLINT(*-reserved-identifier)
#   define _WIN32_IE 0x0400 // NOLINT(*-reserved-identifier)
# endif

# ifndef _WIN32_WINNT // NOLINT(*-reserved-identifier)
#   define _WIN32_WINNT 0x0400 // NOLINT(*-reserved-identifier)
# endif

# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <commctrl.h>
# include <commdlg.h>
# include <shlobj.h>
# include <tchar.h>

# if defined(min)
#   undef min
# endif
# if defined(max)
#   undef max
# endif

extern HINSTANCE default_module_handle;
# ifndef __MODULE_HANDLE // NOLINT(*-reserved-identifier)
#   define __MODULE_HANDLE default_module_handle // NOLINT(*-reserved-identifier)
# endif
#endif