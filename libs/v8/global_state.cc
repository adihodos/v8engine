
#include "v8/gui/basic_window.hpp"
#include "v8/rendering/constants.hpp"
#include "v8/rendering/renderer.hpp"
#include "v8/rendering/render_assets_cache.hpp"
#include "v8/scene/scene_system.hpp"
#include "v8/io/filesystem.hpp"

#include "v8/global_state.hpp"

v8::application_state* v8::state = nullptr;

v8::application_state::application_state() 
{
}

v8::application_state::~application_state() {}

v8_bool_t v8::application_state::initialize() {
    assert((state == nullptr) && "Error : v8 state is already initialized!");
    state = this;
    return true;
}