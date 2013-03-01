#pragma once

#include <v8/v8.hpp>

namespace v8 { namespace input {

/**
 * \brief System independent key constants.
 */
struct Key_Sym_t {
    enum {
        Unknown,

        /**
         *\name Special keys
         */

         /*@{*/
        Cancel,
        Backspace,
        Tab,
        Clear,
        Enter,
        Shift,
        Control,
        Alt,
        Pause,
        CapsLock,
        Escape,
        Space,
        PageUp,
        PageDown,
        End,
        Home,
        Left,
        Up,
        Right,
        Down,
        Select,
        PrintScreen,
        Insert,
        Delete,
        Separator,
        LeftWin,
        RightWin,
        NumLock,
        ScrolLock,
        LeftShift,
        RightShift,
        RightControl,
        LeftMenu,
        RightMenu,
        /*@}*/

        /**
         *\name Aplha numeric keys
         */

         /*@{*/
        Key_0,
        Key_1,
        Key_2,
        Key_3,
        Key_4,
        Key_5,
        Key_6,
        Key_7,
        Key_8,
        Key_9,
        Key_A,
        Key_B,
        Key_C,
        Key_D,
        Key_E,
        Key_F,
        Key_G,
        Key_H,
        Key_I,
        Key_J,
        Key_K,
        Key_L,
        Key_M,
        Key_N,
        Key_O,
        Key_P,
        Key_Q,
        Key_R,
        Key_S,
        Key_T,
        Key_U,
        Key_V,
        Key_W,
        Key_X,
        Key_Y,
        Key_Z,
        /*@}*/

        /**
         *\name Numpad keys
         */

         /*@{*/
        KP0,
        KP1,
        KP2,
        KP3,
        KP4,
        KP5,
        KP6,
        KP7,
        KP8,
        KP9,
        KP_Multiply,
        KP_Add,
        KP_Minus,
        KP_Decimal,
        KP_Divide,
        /*@}*/

        /**
         *\name Function keys
         */

         /*@{*/
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        /*@}*/
        Last
    };
};

extern const v8_int32_t* kScanCodeToKeyMap;

inline v8_int32_t os_key_scan_to_app_key(v8_int32_t os_key) {
    return kScanCodeToKeyMap[os_key];
}

} // namespace input
} // namespace v8
