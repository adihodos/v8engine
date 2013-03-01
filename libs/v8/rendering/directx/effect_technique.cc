#include <v8/v8.hpp>
#include <v8/base/debug_helpers.hpp>

#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/effect_technique.hpp"
#include "v8/rendering/directx/effect_pass.hpp"

v8::directx::effect_technique::~effect_technique() {}

void v8::directx::effect_technique::on_technique_loaded() {
    assert(is_valid());

    reset_data();

    HRESULT ret_code;
    CHECK_D3D(&ret_code, technique_handle_->GetDesc(&technique_info_));

    if (FAILED(ret_code)) {
        technique_handle_ = nullptr;
        return;
    }

    for (v8_uint_t i = 0; i < technique_info_.Passes; ++i)  {
        ID3DX11EffectPass* pass_handle = technique_handle_->GetPassByIndex(i);

        v8::base::scoped_ptr<effect_pass> new_pass(new effect_pass());
        new_pass->internal_np_set_handle(pass_handle);

        if (!new_pass->is_valid()) {
            OUTPUT_DBG_MSGA("Warning : failed to create pass @ index %u",
                            i);
            continue;
        }

        passes_by_name_.insert(std::make_pair(
            new_pass->get_name(), v8::base::scoped_pointer_get(new_pass)));

        passes_by_index_.push_back(std::move(new_pass));
    }
}