#include "pch_hdr.hpp"

#include <v8/base/count_of.hpp>
#include <v8/base/string_util.hpp>
#include <v8/base/posix_utils.hpp>

#include "v8/base/debug_helpers_posix.hpp"

void v8::base::debug::output_debug_string(
    const char* file,
    int line,
    const char* fmt,
    ...
    ) {
    char buff_msg[2048];
    int chr_cnt = v8::base::snprintf(buff_msg, count_of_array(buff_msg),
                                     "\n[File %s, line %d]\n", file, line);

    if (chr_cnt > 0) {
        HANDLE_SYSCALL_EINTR(write(STDERR_FILENO, buff_msg,
                                   static_cast<size_t>(chr_cnt)));
    }

    va_list args_ptr;
    va_start(args_ptr, fmt);
    chr_cnt = v8::base::vsnprintf(buff_msg, count_of_array(buff_msg), fmt,
                                  args_ptr);
    va_end(args_ptr);

    if (chr_cnt > 0) {
        HANDLE_SYSCALL_EINTR(write(STDERR_FILENO, buff_msg,
                                   static_cast<size_t>(chr_cnt)));
    }
}

