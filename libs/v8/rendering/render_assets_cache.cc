#include "v8/rendering/effect.hpp"
#include "v8/rendering/renderer.hpp"
#include "v8/rendering/render_assets_cache.hpp"

struct v8::rendering::render_assets_cache::implementation_details {
    rendering::renderer*    render_sys;
};

v8::rendering::render_assets_cache::render_assets_cache(renderer* rsys)
    : pimpl_(new implementation_details())
{
    pimpl_->render_sys = rsys;
}

v8::rendering::render_assets_cache::~render_assets_cache() {}
