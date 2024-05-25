#pragma once

#include <winapi20/detail/export.h>

#if defined(WINAPI20_ENABLED)
# include <winapi20/wrappers/handle.h>
# include <winapi20/impl/winbase_impl.h>
# include <winapi20/impl/errhandlingapi_impl.h>
# include <winapi20/impl/shlobj_core_impl.h>
# include <winapi20/impl/consoleapi_impl.h>
# include <winapi20/impl/tlhelp32_impl.h>
# include <winapi20/impl/processthreadsapi_impl.h>
# include <winapi20/impl/libloaderapi_impl.h>
#endif