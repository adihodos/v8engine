#include "pch_hdr.hpp"

#include <v8/base/count_of.hpp>
#include <v8/base/string_util.hpp>
#include <v8/base/win32_utils.hpp>

using namespace v8::base;

void v8::base::win32::varargs_msgbox(
    UINT buttons,
    const wchar_t* title,
    const wchar_t* fmt,
    ...
    ) {
    va_list args_ptr;
    va_start(args_ptr, fmt);

    wchar_t buff_msg[2048];
    vsnwprintf(buff_msg, count_of_array(buff_msg), fmt, args_ptr);
    va_end(args_ptr);
    ::MessageBoxW(nullptr, buff_msg, title, buttons);
}
