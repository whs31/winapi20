#pragma once

#include <string_view>
#include <sstream>
#include <fmt/format.h>

namespace winapi::utility
{
  template <typename C>
  struct basic_ostream_formatter : fmt::formatter<std::basic_string_view<C>, C>
  {
    template <typename T, typename OutputIt>
    inline auto format(T const& value, fmt::basic_format_context<OutputIt, C>& ctx) const -> OutputIt {
      auto ss = std::basic_stringstream<C>();
      ss << value;
      return fmt::formatter<std::basic_string_view<C>, C>::format(ss.str(), ctx);
    }
  };

  using ostream_formatter = basic_ostream_formatter<char>;
  using wostream_formatter = basic_ostream_formatter<wchar_t>;
}