#pragma once

#include <string>

namespace v8 {

void string_vprintf(
    std::string* dst,
    const char* fmt,
    ...
    );

}