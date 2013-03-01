#include <utility>
#include <windows.h>
#include "v8/base/count_of.hpp"
#include "v8/base/string_util.hpp"
#include "v8/meta_programming/param_select.hpp"

#include "v8/utility/string_ext.hpp"
#include "v8/utility/win_util.hpp"

void v8::utility::formatted_msg_box(
    const char* title,
    v8_uint_t buttons,
    const char* format,
    ...
    ) {
    char buff_msg[2048];
    va_list args_p;
    va_start(args_p, format);
    v8::base::vsnprintf(buff_msg, dimension_of(buff_msg), format, args_p);
    va_end(args_p);

    MessageBoxA(nullptr, buff_msg, title, buttons);
}

v8::utility::win32::scoped_wide_string_t 
v8::utility::win32::multibyte_string_to_wide_string(const char* mb_string) {
    assert(mb_string);
    scoped_wide_string_t output_str;

    auto char_count = MultiByteToWideChar(CP_UTF8, 0, mb_string, -1, nullptr, 0);
    if (char_count) {
        output_str = new wchar_t[char_count + 1];
        char_count = MultiByteToWideChar(
            CP_UTF8, 0, mb_string, -1, v8::base::scoped_pointer_get(output_str),
            static_cast<int>(char_count + 1));
    }
    return std::move(output_str);
}