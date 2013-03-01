#pragma once

#include <v8/v8.hpp>

namespace v8 { namespace rendering {

class fps_counter {
private :
    float           frames_per_second_;
    v8_uint32_t     current_framecount_;
    float           elapsed_time_;

public :
    fps_counter() 
        : frames_per_second_(0.0f), current_framecount_(0), elapsed_time_(0.0f) {}

    void frame_tick(float delta_elapsed) {
        ++current_framecount_;
        elapsed_time_ += delta_elapsed;

        if (elapsed_time_ > 1000.0f) {
            frames_per_second_ = (static_cast<float>(current_framecount_) / elapsed_time_) * 1000;
            current_framecount_ = 0;
            elapsed_time_ = 0.0f;
        }
    }

    float get_fps_count() const {
        return frames_per_second_;
    }
};

} // namespace rendering
} // namespace v8
