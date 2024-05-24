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
# define UNICODE
#endif // WINAPI20_ENABLED