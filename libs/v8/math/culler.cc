#include "pch_hdr.hpp"

#include <v8/math/camera.hpp>
#include <v8/math/containment/bounded_volume.hpp>
#include <v8/math/culling/culler.hpp>

inline uint32_t set_bit(uint32_t val, int bit, bool bit_val) {
    uint32_t high = val & (0xffffffff << (32 - bit - 1));
    uint32_t low = val & (0xffffffff >> (32 - bit));
    return low | high | ((uint32_t)(bit_val) << bit);
}

v8::math::culler::culler()
    : active_planes_(0), plane_count_(6) {}

bool v8::math::culler::cull(const bounded_volume* bound_object) const {
    for (uint32_t i = 0; i < plane_count_; ++i) {
        if (active_planes_ & (1 << i)) {
            if (bound_object->cull(cull_plane_stack_[i]))
                return true;
        }
    }
    return false;
}

void v8::math::culler::pop_plane() {
    if (plane_count_ > 6) {
        --plane_count_;
        active_planes_ = set_bit(active_planes_, plane_count_, false);
    }
}

void v8::math::culler::set_camera(const v8::math::camera* cam) {
    assert(cam);
    cam_ = cam;
    cam_->copy_frustrum_params(frustrum_params_);
    cam_->extract_frustrum_planes(cull_plane_stack_);
    active_planes_ |= 0x3F;
}
