#pragma once

#include <string>
#include <tuple>
#include <windows.h>

#include <v8/v8.hpp>
#include <v8/base/timers.hpp>
#include <v8/base/cpu_counter.hpp>

#include <v8/event/event_delegate.hpp>
#include <v8/event/input_event.hpp>
#include <v8/event/window_event.hpp>
#include <v8/rendering/fps_counter.hpp>
#include <v8/input/key_syms.hpp>

namespace v8 { namespace gui {

///
/// \brief Parameters that control window initialization.
struct window_init_params_t {
    ///< Window width, in pixels.
    v8_int32_t      width;
    ///< Window height in pixels.
    v8_int32_t      height;
    ///< Window title.
    const char*     title;
};    

///
/// \brief Displays the output from the renderer aand provides input events.
class basic_window {
//! \name Construction/initialisation
//! @{

public :

    basic_window() 
    {}

    virtual ~basic_window() {}

    virtual v8_bool_t initialize(const window_init_params_t& init_params);

//! @}

//! \name Event delegates.
//! @{

    /// \brief List of delegates for input events.
    event_handler_list1<const input_event&>         Subscribers_InputEvents;

    /// \brief List of delegates for window resize events.
    event_handler_list1<const resize_event&>        Subscribers_ResizeEvent;

    /// \brief List of delegates for the update event. Any object that must
    /// update its state must register for this event.
    event_handler_list1<float>                      Delegates_UpdateEvent;

    /// \brief List of delegates for a drawing call. 
    /// Any object that needs to draw itself must register for this event.
    event_handler_list0<>                           Delegates_DrawEvent;

//! @}

public :

//! \name Attributes.
//! @{

    HWND get_handle() const {
        return m_windata.win;
    }

    v8_int32_t get_width() const {
        return m_windata.width;
    }

    v8_int32_t get_height() const {
        return m_windata.height;
    }

    /// \brief Returns the aspect ratio of the window (Width/Height).
    float get_aspect_ratio() const {
        return static_cast<float>(m_windata.width) 
               / static_cast<float>(m_windata.height);
    }

//! @}

//! \name Main loop.
//! @{

public :

    /// \brief Pumps messages from the system and fires the update and draw
    /// events.
    virtual void message_loop();

protected :

    virtual void app_main_loop();

    virtual void app_do_frame();

    virtual void app_frame_tick();

    virtual void app_frame_draw();

    virtual void on_app_idle() {
        Sleep(1000);
    }

//! @}

//! \name Message handler functions.
//! @{

protected :

    virtual void handle_wm_close() {
        DestroyWindow(m_windata.win);
    }

    virtual void handle_wm_destroy() { 
        PostQuitMessage(0); 
    }

    virtual void handle_wm_enter_sizemove() { 
        m_windata.resizing = true; 
    }

    virtual void handle_wm_exit_sizemove() { 
        m_windata.resizing = false; 
    }

    virtual void handle_wm_size(
        WPARAM sizing_request, 
        v8_int32_t newWidth, 
        v8_int32_t newHeight
        );

    virtual v8_bool_t handle_wm_key_down(
        v8_int_t key_code, 
        LPARAM l_param
        );

    virtual v8_bool_t handle_wm_key_up(
        v8_int_t key_code, 
        LPARAM l_param
        );

    virtual v8_bool_t handle_wm_syscommand(
        WPARAM w_param, 
        LPARAM l_param
        );

    virtual v8_bool_t handle_wm_mousewheel(
        WPARAM w_param, 
        LPARAM l_param
        );

    virtual v8_bool_t handle_wm_leftbutton_down(
        WPARAM w_param, 
        LPARAM l_param
        );

    virtual v8_bool_t handle_wm_leftbutton_up(
        WPARAM w_param, 
        LPARAM l_param
        );

    virtual v8_bool_t handle_wm_mousemove(
        WPARAM w_param, 
        LPARAM l_param
        );

    virtual void handle_wm_getminmaxinfo(MINMAXINFO* mmInfo) {
        mmInfo->ptMinTrackSize.x = 320;
        mmInfo->ptMinTrackSize.y = 240;
    }

    virtual void handle_wm_activate(
        WPARAM w_param
        );

    virtual LRESULT window_procedure(
        UINT msg, 
        WPARAM w_param, 
        LPARAM l_param
        );

//! @}

//! \name Internal types and routines.
//! @{

protected :

    //!
    //! Stores window state information.
    struct window_stats_t {
        HINSTANCE           instance;
        HWND                win;
        v8_int32_t          width;
        v8_int32_t          height;
        v8_bool_t           initialized;
        v8_bool_t           fullscreen;
        v8_bool_t           minimized;
        v8_bool_t           active;
        v8_bool_t           resizing;
        v8_bool_t           mousecaptured;
        v8_bool_t           quitflag;
        v8_bool_t           occluded;
        v8_uint32_t         style;

        window_stats_t() {
            memset(this, 0, sizeof(*this));
        }
    };

    //!
    //! Stores timer statistics.
    struct running_stats_t {
        v8::base::high_resolution_timer<float>      timer;
        v8::base::cpu_counter                       cpucount;
        rendering::fps_counter                      fpscount;
    };

    //! Window stats.
    window_stats_t                                  m_windata;

    //! Timing stats.
    running_stats_t                                 m_runstats;

private :

    static LRESULT WINAPI window_procedure_stub(
        HWND window, 
        UINT msg, 
        WPARAM w_param, 
        LPARAM l_param
    );

//! @}

//! \name Disabled functions.
//! @{

private :

    NO_CC_ASSIGN(basic_window);

//! @}
};

} // namespace gui
} // namespace v8
