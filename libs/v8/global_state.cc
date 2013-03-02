
#include "v8/gui/basic_window.hpp"
#include "v8/rendering/constants.hpp"
#include "v8/rendering/renderer.hpp"
#include "v8/rendering/render_assets_cache.hpp"
//#include "framework/scene/scene_manager.hpp"
#include "v8/io/filesystem.hpp"

#include "v8/global_state.hpp"

v8::application_state* v8::state = nullptr;

v8::application_state::application_state() 
{
}

v8::application_state::~application_state() {}

v8_bool_t v8::application_state::initialize(
    window_creation_fn win_create_fn
    ) {
    assert((state == nullptr) && "Error : v8 state is already initialized!");
    state = this;

    window_ = win_create_fn();
    if (!window_) {
        return false;
    }

    render_sys_ = new rendering::renderer();
    if (!render_sys_->initialize(window_->get_handle(), window_->get_width(), 
                                 window_->get_height(), 
                                 v8::rendering::ElementType::Unorm8, 4, 24, 8)) {
        return false;
    }

    asset_cache_ = new v8::rendering::render_assets_cache(render_sys());
    file_sys_ = new v8::filesys();
    file_sys_->initialize();

    return true;
}