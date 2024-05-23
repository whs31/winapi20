#pragma once

#if defined(_WIN32)
# if defined(WINAPI20_LIBRARY)
#  define WINAPI20_EXPORT __declspec(dllexport)
# elif defined(WINAPI20_STATIC_LIBRARY)
#  define WINAPI20_EXPORT
# else
#  define WINAPI20_EXPORT __declspec(dllimport)
# endif
#else
# define WINAPI20_EXPORT
#endif

namespace winapi {}