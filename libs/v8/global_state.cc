
//#include "framework/gui/basic_window.hpp"
#include "v8/rendering/renderer.hpp"
//#include "framework/rendering/resource_manager.hpp"
//#include "framework/scene/scene_manager.hpp"
#include "v8/io/filesystem.hpp"
#include "v8/global_state.hpp"

v8::application_state* v8::state = nullptr;

v8::application_state::application_state() {
    assert((state == nullptr) && "Error : v8 state is already initialized!");
    state = this;
}

v8::application_state::~application_state() {}