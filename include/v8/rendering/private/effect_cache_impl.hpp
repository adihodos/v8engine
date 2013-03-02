#pragma once

#include <v8/v8.hpp>
#include <v8/base/scoped_pointer.hpp>

#include <v8/rendering/effect.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/constants.hpp>

namespace v8_framework { namespace render_engine {

///
/// \brief Effect management class.
class effect_manager {
public :

/// \name Constructors.
/// @{

    effect_manager(renderer* r_ptr);

    ~effect_manager();

/// @}

/// \name Effect creation and retrieval.
/// @{

public :

    ///
    /// \brief Creates an instance of an effect class, and returns it to the
    /// user. The user is responsible for the lifetime management of the 
    /// efect instance.
    /// \param[in] effect_name The name of the effect.
    /// \param[in] compile_macros Null terminated string, containing
    /// defined macros that will be used when compiling the effect.
    /// \param[in] compile_flags Additional flags that control compilation.
    /// \param[in] target_profile The profile to use when compiling the effect.
    /// \param[in] effect_file_path A fully qualified path to an effect file.
    /// If nothing is specified, the path to the effect fill will be composed
    /// in this manner : full_path_to_shader_directory\effect_name.fx
    ///
    /// \note The syntax for the string containing compilation macros is :
    /// [macro_name(macro_value)]. Each macro must be enclosed between [].
    /// The value is optional. If no value is specified, the round parenthesis 
    /// can be omitted too.
    /// \code
    /// const char* valid_macros = "[LIGHT_COUNT(4)][USE_PHONG_ILLUM]";
    /// const char* invalid_macros = "[LIGHT_COUNT 4]";
    /// \endcode
    effect* create_effect(
        const char* effect_name,
        const char* compile_macros,
        const v8_uint32_t compile_flags,
        const FX_Model::Type target_profile = FX_Model::FX_4_1,
        const char* effect_file_path = nullptr
        );

    ///
    /// \brief Retrieves an instance of a compiled effect, or creates one if
    /// it did not exist, and returns a pointer to the user. The class has
    /// ownership of the effect instance.
    /// \param[in] effect_info Information identifying the effect.
    /// \returns Pointer to effect instance if found or if it was successfully
    /// created, null on failure (effect file not found, compile error, etc).
    effect* get_effect(
        const effect_info_t& effect_info
        );

/// @}

/// \name Defined types.
/// @{

private :

    struct implementation_details;
    v8::base::scoped_ptr<implementation_details>                    pimpl_;

/// @}

/// \name Disabled functions.
/// @{

private :
    NO_CC_ASSIGN(effect_manager);

/// @}
};

} // namespace direct
} // namespace v8_framework
