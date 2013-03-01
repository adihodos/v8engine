#include <v8/base/debug_helpers.hpp>
#include <v8/math/vector3.hpp>

#include "v8/io/config_file_reader.hpp"

v8::io::V8ConfigFile::V8ConfigFile(
    const char* const k_file_path
    )
    try
        :       is_valid_(false)
            ,   mmfile_(k_file_path)
            ,   config_file_()
    {
        config_file_.Parse<0>(static_cast<const char*>(mmfile_.memory()));
        is_valid_ = !config_file_.HasParseError();
        if (!is_valid_) {
            OUTPUT_DBG_MSGA("Config file [%s], parse error [%s]",
                            k_file_path, config_file_.GetParseError());
        }
    }
    catch (const std::exception&) {
        OUTPUT_DBG_MSGA("Failed to open config file [%s]", k_file_path);
    }

v8::io::V8ConfigFile_ReadOnlySection
v8::io::V8ConfigFile::GetSection(
    const char**        k_section_hierarchy,
    const v8_size_t     k_hier_entries
    ) const {
    assert(CheckIfValid());
    
    if (!k_section_hierarchy) {
        return V8ConfigFile_ReadOnlySection(&config_file_);
    }

    const rapidjson::Value* section_ptr = &config_file_;
    for (v8_size_t idx = 0; idx < k_hier_entries; ++idx) {
        if (!section_ptr->HasMember(k_section_hierarchy[idx])) {
            section_ptr = nullptr;
            break;
        }

        section_ptr = &(*section_ptr)[k_section_hierarchy[idx]];
    }
    return V8ConfigFile_ReadOnlySection(section_ptr);
}