#include <vector>
#include "v8/base/string_util.hpp"
#include "v8/utility/string_ext.hpp"

void v8::string_vprintf(
    std::string* dst,
    const char* fmt,
    ...
    ) {
    std::vector<char> tmp_buff;
    tmp_buff.resize(256);
    va_list args_ptr;
    va_start(args_ptr, fmt);

    v8_int32_t count = -1;
    for (;;) {
        count = v8::base::vsnprintf(&tmp_buff[0], tmp_buff.size(), fmt, args_ptr);
        if (count < 0) {
            break;
        }

        if (static_cast<v8_uint32_t>(count) >= tmp_buff.size()) {
            tmp_buff.resize(count + 1);
            continue;
        }
        break;
    }
    if (count)
        *dst = static_cast<const char*>(&tmp_buff[0]);
}
