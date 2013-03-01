#pragma once

#include <v8/v8.hpp>

namespace v8 {

///
/// \brief This event is set after the window was resized.
struct resize_event {
    int width;      ///< New width of the window.
    int height;     ///< New height of the window.
};

}