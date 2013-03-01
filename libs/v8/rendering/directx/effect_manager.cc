#include <vector>

#include <D3Dcompiler.h>
#include <d3dx11effect.h>

#include <v8/v8.hpp>
#include <v8/base/associative_container_veneer.hpp>
#include <v8/base/auto_buffer.hpp>
#include <v8/base/com_exclusive_pointer.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/fixed_pod_vector.hpp>
#include <v8/base/fixed_pod_stack.hpp>
#include <v8/base/scoped_pointer.hpp>

#include <third_party/stlsoft/platformstl/filesystem/path.hpp>
#include <third_party/stlsoft/platformstl/filesystem/memory_mapped_file.hpp>
#include <third_party/stlsoft/rangelib/sequence_range.hpp>

#include "framework/app_dirs.hpp"
#include "framework/global_state.hpp"
#include "framework/rendering/effect.hpp"
#include "framework/rendering/effect_info.hpp"
#include "framework/rendering/renderer.hpp"

#include "framework/directx/internal/directx_debug_helpers.hpp"
#include "framework/directx/internal/directx_include_handler.hpp"
#include "framework/directx/internal/directx_compile_flags_translator.hpp"
#include "framework/directx/internal/directx_utilities.hpp"

#include "framework/rendering/effect_manager.hpp"

namespace {

const char* const kFXTargetStrings[] = {
    "fx_4_0",
    "fx_4_1",
    "fx_5_0"
};

} // anonymous namespace

struct v8_framework::render_engine::effect_manager::implementation_details {
/// \name Types.
/// @{

public :

    ///< Data structure to store compiled effects.
    typedef std::unordered_map
    <
        v8_framework::render_engine::effect_info_t, 
        v8_framework::render_engine::effect*
    >                                                   compiled_effects_pool_t;

    ///< Compiled effect and associated information.
    typedef compiled_effects_pool_t::value_type             effect_pool_item_t;

    struct effect_dtor {
        void operator()(effect_pool_item_t& eff_data) const {
            delete eff_data.second;
        }
    };

/// @}

public :
    implementation_details(v8_framework::render_engine::renderer* rptr)
        :   r_ptr_(rptr)
    {}

/// \name Data members.
/// @{

public :

    ///< Stores compiled effects, along with the compilation settings used.
    v8::base::associative_container_veneer<
        compiled_effects_pool_t,
        effect_dtor
    >                                                         compiled_effects_;

    ///< This is needed for effect creation.
    v8_framework::render_engine::renderer*                               r_ptr_;

/// @}
};


v8_framework::render_engine::effect_manager::effect_manager(
    v8_framework::render_engine::renderer* rptr
    )
    :   pimpl_(new implementation_details(rptr))
{}

v8_framework::render_engine::effect_manager::~effect_manager()
{}

v8_framework::render_engine::effect*
v8_framework::render_engine::effect_manager::create_effect(
    const char* effect_name,
    const char* compile_macros,
    const v8_uint32_t compile_flags,
    const FX_Model::Type target_profile,
    const char* eff_file_path
    ) {

    //
    // Build effect file path.
    platformstl::path effect_file_path;

    if (eff_file_path) {
        effect_file_path.push(eff_file_path);
    } else {
        //
        // No path specified, so derive the path using the effect's name.
        effect_file_path.push(
            global::state->Filesys->get_dir_path(app_dirs::Dir::Shaders).c_str());
        effect_file_path.push(effect_name);
        const char* const kEffFileExtension = "fx";
        effect_file_path.push_ext(kEffFileExtension);
    }

    //
    // Collect defined compilation macros.
    std::vector<directx::user_defined_macro_t> user_macros;
    if (compile_macros) {
        if (!directx::parse_macro_string(compile_macros, &user_macros)) {
            return nullptr;
        }
    }

    v8::base::fixed_pod_vector<D3D10_SHADER_MACRO> api_macro_list;
    for (rangelib::sequence_range<const std::vector<directx::user_defined_macro_t>> r(user_macros);
         r; ++r) {

        D3D10_SHADER_MACRO macro = { (*r).define.c_str(), (*r).value.c_str() };
        api_macro_list.push_back(macro);
    }

    D3D10_SHADER_MACRO list_terminator = { nullptr, nullptr };
    api_macro_list.push_back(list_terminator);

    //
    // Load the effect file, compile the code and create the effect.
    platformstl::memory_mapped_file mmfile(effect_file_path.c_str());

    v8::base::com_exclusive_pointer<ID3D10Blob>::type   compiled_bytecode;
    v8::base::com_exclusive_pointer<ID3D10Blob>::type   error_message;
    directx::internal::dxcompile_include_handler        include_handler;
    HRESULT                                             ret_code;

    CHECK_D3D(
        &ret_code,
        D3DCompile(mmfile.memory(), mmfile.size(), nullptr,
                   &api_macro_list[0], &include_handler, nullptr,
                   kFXTargetStrings[target_profile], 
                   directx::internal::translate_compiler_options_to_directx_value(compile_flags),
                   0U,
                   v8::base::scoped_pointer_get_impl(compiled_bytecode),
                   v8::base::scoped_pointer_get_impl(error_message)));

    
    if (FAILED(ret_code)) {
        OUTPUT_DBG_MSGA("Failed to compile effect [%s]", effect_file_path.c_str());
        OUTPUT_DBG_MSGA("Compiler error [%s]", error_message->GetBufferPointer() ?
            static_cast<const char*>(error_message->GetBufferPointer()) : "not available"
            );
        return nullptr;
    }

    ID3DX11Effect* effect_interface = nullptr;
    CHECK_D3D(
        &ret_code,
        D3DX11CreateEffectFromMemory(compiled_bytecode->GetBufferPointer(),
                                     compiled_bytecode->GetBufferSize(),
                                     0U, pimpl_->r_ptr_->internal_np_get_device(),
                                     &effect_interface));

    if (SUCCEEDED(ret_code)) {
        v8::base::scoped_ptr<effect> new_effect(new effect());
        new_effect->internal_np_set_handle(effect_interface);

        if (new_effect->is_valid()) {
            return v8::base::scoped_pointer_release(new_effect);
        }
    }

    return nullptr;
}

v8_framework::render_engine::effect* 
v8_framework::render_engine::effect_manager::get_effect(
    const effect_info_t& effect_info
    ) {

    auto itr_effects = pimpl_->compiled_effects_.find(effect_info);
    if (itr_effects != std::end(pimpl_->compiled_effects_)) {
        return itr_effects->second;
    }

    v8::base::scoped_ptr<effect> compiled_effect(create_effect(
        effect_info.eff_name, effect_info.eff_compile_macros,
        effect_info.eff_compile_flags, effect_info.eff_target));

    if (!compiled_effect) {
        return nullptr;
    }

    pimpl_->compiled_effects_.insert(std::make_pair(
        effect_info, v8::base::scoped_pointer_get(compiled_effect)));

    return v8::base::scoped_pointer_release(compiled_effect);
}
