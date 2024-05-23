#pragma once

#if defined(_WIN32)
# define WINAPI20_ENABLED
#endif

#if defined(WINAPI20_ENABLED)
# if defined(WINAPI20_LIBRARY)
#  define WINAPI20_EXPORT __declspec(dllexport)
# elif defined(WINAPI20_STATIC_LIBRARY)
#  define WINAPI20_EXPORT
# else
#  define WINAPI20_EXPORT __declspec(dllimport)
# endif // WINAPI20_LIBRARY
#else
# define WINAPI20_EXPORT
#endif // WINAPI20_ENABLED

namespace winapi {}