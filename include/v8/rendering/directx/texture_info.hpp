#pragma once

#include <string>
#include <v8/v8.hpp>

namespace v8 { namespace directx {

///
/// \brief Stores information needed to load a texture.
struct texture_info_t {

    ///< Fully qualified path of the file containing the texture's data.
    std::string     tex_filename;

    ///< Binding flags, indicating usage scenario 
    ///< (e.g. shader resource, render target).
    v8_uint32_t     tex_bindflags;

    texture_info_t() : tex_bindflags(0) {}

    texture_info_t(const char* file_name, const v8_uint32_t flags)
        :       tex_filename(file_name)
            ,   tex_bindflags(flags)
    {}

    texture_info_t(const std::string& file_name, const v8_uint32_t flags)
        :       tex_filename(file_name)
            ,   tex_bindflags(flags)
    {}
};

} // namespace directx
} // namespace v8
