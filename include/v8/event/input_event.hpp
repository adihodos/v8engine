#pragma once

#include <v8/v8.hpp>

namespace v8 {

///
/// \brief Identifies the buttons of a mouse input device.
struct MouseButton {
    enum Value {
        Left,
        Right,
        XButton1,
        XButton2
    };
};

///
/// \brief Identifies the source of the input event.
struct InputEventType {
    enum Value {
        Key,
        Mouse_Button,
        Mouse_Wheel,
        Mouse_Movement,
        Joystick
    };
};

///
/// \brief Data for a key event (press, release).
struct key_event_t {
    ///< Identifies the pressed key.
    v8_int32_t      key;

    ///< If true, the key was pressed, false if depressed.
    v8_bool_t       down;
};

///
/// \brief Mouse button event information.
struct mouse_button_event_t {
    ///< The button that generated the event.
    MouseButton::Value  id;
    ///< X coordinate of the mouse.
    v8_int32_t          x_pos;
    ///< Y coordinate of the mouse.
    v8_int32_t          y_pos;
    ///< True if button was pressed, false if it was depressed.
    v8_bool_t           down;
};

///
/// \brief Mouse wheel event data.
struct wheel_event_t {
    ///< Number of revolutions.
    v8_int32_t          delta;
    ///< X coordinate of the mouse.
    v8_int32_t          x_pos;
    ///< Y coordinate of the mouse.
    v8_int32_t          y_pos;
};

///
/// \brief Mouse movement data.
struct mouse_move_event_t {
    ///< X coordinate of the mouse.
    v8_int32_t          x_pos;
    ///< Y coordinate of the mouse.
    v8_int32_t          y_pos;
};

///
/// \brief Contains information about an input event (key press/release,
//  mouse button press/release, etc).
struct input_event {

    ///< Event data.
    union {
        key_event_t                 key_ev;
        mouse_button_event_t        mouse_button_ev;
        wheel_event_t               mouse_wheel_ev;
        mouse_move_event_t          mouse_move_ev;
    };

    ///< Type of event.
    InputEventType::Value       type;
};

} // namespace v8
