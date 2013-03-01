#pragma once

#include <v8/v8.hpp>

#include <cassert>
#include <string>

namespace v8 {

///
/// \brief The purpose of this class is to define a hierachy for organizing
/// the application's assets directories (shaders, mmeshes, scripts, etc) 
/// and to offer a consistent and uniform way of locating these directories
/// across the various subsystems of the application.
/// For these purposes, there are a number of predefined variables 
/// (encoded by the Dir::Type enum), that map to a specific asset directory.
/// For example, Dir::Shaders refers to the directory where the shaders are
/// stored. The values of these variables can be configured via a configuration 
/// file called fsgame.ltx, or they can be set at runtime.
/// 
class filesys {
public :

//! \name Construction and initialization.
//! @{

public :

    filesys();

    void initialize(const char* config_file_path = nullptr);

private :

    v8_bool_t read_config_file();

    void set_defaults();

//! @}

//! \name Defined types
//! @{

public :

    //! Predefined variable values that identify asset directories.
    struct Dir {
        enum Type {

            //! Directory where the executable module is located
            ExeModule,

            //! Main root of the hierarchy. If not set in the config file, 
            //! it defaults to the directory of the executable module dir.
            //! Wether specified via the config file or at runtime, it must be
            //! a fully qualified path. All other entries must be relative to
            //! this path.
            FSRoot,

            //! Stores assets. Defaults to FSRoot\\gamedata.
            Gamedata,

            //! Stores shaders. Defaults to gamedata\\shaders.
            Shaders,

            //! Stores meshes. Defaults to gamedata\\models.
            Models,

            //! Stores textures. Defaults to gamedata\\textures.
            Textures,

            //! Stores config files. Defaults to gamedata\\config.
            Config,

            //! Stores vertex layout declarations
            VertexDecl,

            //! Last value. Any new members must be added before it.
            Last
        };
    };

private :

    //! Stores values of a directory variable.
    struct Dir_Var_t {

        //!< Directory name (e.g "shaders")
        std::string         name;

        //!< Cached full path (e.g. D:\\game\\gamedata\\shaders\\)
        //!< The full path is obtained withe the following formula :
        //!< FSRoot + parent + path_sep + name + path_sep
        std::string         full_path;

        //!< Parent directory of this directory (e.g. gamedata)
        v8_int_t            parent;

        Dir_Var_t() : parent(Dir::Last) {}

        Dir_Var_t(const std::string& dir_name, const v8_int_t tp)
            : name(dir_name), parent(tp) {}

        Dir_Var_t(const char* dir_name, const v8_int_t tp)
            : name(dir_name), parent(tp) {}
    };

//! @}

//! \name Retrieval of configured values.
//! @{

public :

    //!
    //! \brief Returns the short name of the directory variable (e.g. "shaders").
    std::string get_dir_name(const v8_int_t dir_type) const {
        assert(is_valid());
        return dirs_[dir_type].name;
    }

    //!
    //! \brief Returns the full configured path of the specified directory 
    //! variable (e.g. "D:\\games\\gamedata\\shaders\\").
    std::string get_dir_path(const v8_int_t dir_type) const {
        assert(is_valid());
        return dirs_[dir_type].full_path;
    }

//! @}

//! \name Directory name manipulation.
//! @{

public :

    //!
    //! \brief Sets the short name of the directory variable.
    void set_dir_name(const v8_int_t var, const char* dir_name);

private :

    std::string build_path_for_var(const v8_int_t var);

//! @}

//! \name Data members and sanity checking.
//! @{

private :

    v8_bool_t is_valid() const {
        return initialized_;
    }

    //!< Cached data for the application's directories.
    Dir_Var_t                                   dirs_[Dir::Last];

    //!< Keeps track if object has been initialized.
    v8_bool_t                                   initialized_;

//! @}

};

} // namespace v8
