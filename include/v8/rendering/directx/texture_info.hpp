#pragma once

#include <string>
#include <v8/v8.hpp>

namespace v8 { namespace directx {
    struct texture_info_t {
        std::string     tex_filename;
        v8_uint32_t     tex_bindflags;

        texture_info_t() : tex_bindflags(0) {}

        texture_info_t(const char* file_name, const v8_uint32_t flags)
            :       tex_filename(file_name)
                ,   tex_bindflags(flags)
        {}
    };
} // namespace directx
} // namespace v8
