#include <utility>
#include <cstdlib>

#include <v8/v8.hpp>
#include <v8/base/count_of.hpp>
#include <third_party/stlsoft/platformstl/filesystem/path.hpp>

#include "v8/io/config_file_reader.hpp"
#include "v8/utility/string_ext.hpp"
#include "v8/utility/hash_fnv1a.hpp"

#include "v8/io/filesystem.hpp"

namespace {

const char* const C_Path_Sep = "\\";

const v8::filesys::Dir::Type C_default_hierarchy[] = {
    v8::filesys::Dir::Last, // no parent
    v8::filesys::Dir::Last, // no parent
    v8::filesys::Dir::FSRoot, // parent of gamedata
    v8::filesys::Dir::Gamedata, // par of shaders
    v8::filesys::Dir::Gamedata, // par of models
    v8::filesys::Dir::Gamedata, // par of textures
    v8::filesys::Dir::Gamedata, // par of config
    v8::filesys::Dir::Gamedata // par of vertex decl
};

const char* const C_default_names[] = {
    "",
    "",
    "gamedata",
    "shaders",
    "models",
    "textures",
    "config",
    "vertex_decl",
    ""
};

v8_int_t parent_string_to_parent_id(
    const char* parent_string
    ) {
    using v8::utility::FNV1AHash_t;

    if (FNV1AHash_t::compile_time_hash("$fs_root$")
        == FNV1AHash_t::hash(parent_string, strlen(parent_string) + 1)) {
        return v8::filesys::Dir::FSRoot;
    }

    if (FNV1AHash_t::compile_time_hash("$gamedata$")
        == FNV1AHash_t::hash(parent_string, strlen(parent_string) + 1)) {
        return v8::filesys::Dir::Gamedata;
    }

    return v8::filesys::Dir::Last;
}

}

v8::filesys::filesys() : initialized_(false) {}

void v8::filesys::initialize(const char* main_dir_path) {
    if (initialized_)
        return;

    //
    // Set default values. This way, even if the config file does not exist,
    // we're using sane values.
    set_defaults();

    platformstl::path main_settings_file_path;

    if (main_dir_path) {
        main_settings_file_path.push(main_dir_path);
    } else {
        main_settings_file_path.push(get_dir_path(Dir::ExeModule).c_str());
    }
                   
    main_settings_file_path.push("fsgame.ltx");

    //
    // Try to load the user defined config file, if one exists.
    const io::V8ConfigFile config_file(main_settings_file_path.c_str());
    if (!config_file.CheckIfValid()) {
        return;
    }
    
    const io::V8ConfigFile_ReadOnlySection conf_settings(config_file.GetSection());
    const char* k_fsroot = nullptr;
    if (conf_settings.GetValue("$fs_root$", k_fsroot)) {
        platformstl::path fsroot_path(k_fsroot);

        //
        // $fs_root$ must be an absolute path.
        if (!fsroot_path.empty() && fsroot_path.is_absolute()) {
            //
            // Make sure path has a trailing separator.
            if (!fsroot_path.has_sep()) {
                fsroot_path.push_sep();
            }

            dirs_[Dir::FSRoot].full_path.assign(fsroot_path.c_str());
        }
    }

    //
    // Walk each predefined entry and if we find one, we use the values
    // from the config file.
    const struct dir_var_data_t {
        const char*     var_name;
        v8_int_t        var_id;
    } predefined_dir_vars[] = {
        { "$gamedata$", Dir::Gamedata },
        { "$shaders$", Dir::Shaders },
        { "$models$", Dir::Models },
        { "$textures$", Dir::Textures },
        { "$config$", Dir::Config },
        { "$vertex_decl$", Dir::VertexDecl }
    };

    for (v8_size_t i = 0; i < dimension_of(predefined_dir_vars); ++i) {
        const char* k_dirs_pair[] = { nullptr, nullptr };
        //
        // Config does not define this variable, so skip it.
        if (!conf_settings.GetValue(predefined_dir_vars[i].var_name, k_dirs_pair)) {
            continue;
        }

        //
        // Map a parent string in the form of $XXXXX$ to a known integer id.
        const int read_parent_id = parent_string_to_parent_id(k_dirs_pair[0]);

        //
        // Check if values are sane.
        if (!(read_parent_id >= Dir::FSRoot && read_parent_id < Dir::Last)) {
            continue;
        }

        platformstl::path var_path(k_dirs_pair[1]);
        if (var_path.empty() || var_path.is_absolute()) {
            continue;
        }

        //
        // Values are sane, so replace the defaults.
        dirs_[predefined_dir_vars[i].var_id].parent = static_cast<Dir::Type>(read_parent_id);
        dirs_[predefined_dir_vars[i].var_id].name.assign(var_path.c_str());
    }
    
    for (v8_int_t i = Dir::Gamedata; i < Dir::Last; ++i) {
        dirs_[i].full_path = build_path_for_var(static_cast<Dir::Type>(i));
    }
    initialized_ = true;
}

void v8::filesys::set_defaults() {
    char* exe_module_path = nullptr;
    //
    // TODO : non portable api call.
    _get_pgmptr(&exe_module_path);

    dirs_[Dir::ExeModule].name = "";
    string_vprintf(&dirs_[Dir::ExeModule].full_path, "%s", exe_module_path);

    //
    // By default, $fs_root$ directory is defined to be the directory where 
    // the executable module resides.
    dirs_[Dir::FSRoot].name.clear();
    dirs_[Dir::FSRoot].full_path = dirs_[Dir::ExeModule].full_path;

    auto lpos = dirs_[Dir::FSRoot].full_path.find_last_of(C_Path_Sep);

    if (lpos != std::string::npos) {
        dirs_[Dir::FSRoot].full_path.erase(lpos + 1U);
    }

    for (int i = Dir::Gamedata; i < Dir::Last; ++i) {
        dirs_[i].parent = C_default_hierarchy[i];
        dirs_[i].name = C_default_names[i];
    }
}

void v8::filesys::set_dir_name(
    const v8_int_t var, 
    const char* dir_name
    ) {
    assert(var != Dir::ExeModule);
    assert(dir_name);

    dirs_[var].name = dir_name;
    
    for (int i = Dir::Gamedata; i < Dir::Last; ++i) {
        dirs_[i].full_path = build_path_for_var(static_cast<Dir::Type>(i));
    }
}

std::string v8::filesys::build_path_for_var(const v8_int_t var) {
    assert(var != Dir::ExeModule);
    assert(var != Dir::Last);

    if (var == Dir::FSRoot) {
        assert(!dirs_[Dir::FSRoot].full_path.empty());
        return dirs_[Dir::FSRoot].full_path;
    }

    return build_path_for_var(dirs_[var].parent) + dirs_[var].name + "\\";
}

std::string v8::filesys::make_full_path(
    const v8_int32_t var, const char* const file_name, const char* const ext
    ) const {
    assert(file_name);
    assert(is_valid());

    platformstl::path full_path(dirs_[var].full_path.c_str());
    full_path.push(file_name);
    if (ext) {
        full_path.push_ext(ext);
    }
    return full_path.c_str();
}