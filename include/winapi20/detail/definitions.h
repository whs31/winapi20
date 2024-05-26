#pragma once

#include <winapi20/detail/export.h>

namespace winapi
{
  enum class CaseSensitivity
  {
    CaseSensitive,
    CaseInsensitive
  };

  enum class Cleanup
  {
    Auto,
    Manual
  };

  enum class OwnershipMode
  {
    Own,
    View
  };
}