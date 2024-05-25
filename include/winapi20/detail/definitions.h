#pragma once

#include <winapi20/detail/export.h>

namespace winapi
{
  enum class CaseSensitivity
  {
    CaseSensitive,
    CaseInsensitive
  };

  /// \brief Syntax sugar for specifying the current process.
  enum PID : uint32_t { CurrentProcess = 0 };

  enum class Cleanup
  {
    Auto,
    Manual
  };
}