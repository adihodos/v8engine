#include <cassert>
#include <windowsx.h>
#include <v8/base/debug_helpers.hpp>

#include "v8/rendering/renderer.hpp"
#include "v8/rendering/constants.hpp"
#include "v8/gui/basic_window.hpp"
#include "v8/input/key_syms.hpp"

v8_bool_t v8::gui::basic_window::initialize(const window_init_params_t& init_params) {
    if (m_windata.initialized) {
        return true;
    }

    m_windata.instance = GetModuleHandle(nullptr);
    m_windata.width = init_params.width;
    m_windata.height = init_params.height;
    m_windata.style = WS_OVERLAPPEDWINDOW;

    const char* const kWindowClassName = "__##@@D3DAPPWindowCLass@@##__$$__";

    WNDCLASS wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = &basic_window::window_procedure_stub; 
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = m_windata.instance;
    wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
    wc.lpszMenuName  = 0;
    wc.lpszClassName = kWindowClassName;

    if (!RegisterClass(&wc))
        return false;

    RECT windowRect = { 
        0, 0, static_cast<LONG>(m_windata.width), 
        static_cast<LONG>(m_windata.height) 
    };

    const BOOL ret_code = AdjustWindowRect(&windowRect, m_windata.style, false);
    assert(ret_code && "Failed to adjust window rectangle");

    m_windata.win = CreateWindow(
        kWindowClassName, 
        init_params.title, 
        m_windata.style, 
        CW_USEDEFAULT, 
        CW_USEDEFAULT, 
        windowRect.right - windowRect.left, 
        windowRect.bottom - windowRect.top,
        nullptr, nullptr, m_windata.instance, 
        this
        );

    if (!m_windata.win) {
        return false;
    }

    m_windata.initialized = true;
    m_windata.active = true;

    ShowWindow(m_windata.win, SW_SHOWNORMAL);
    UpdateWindow(m_windata.win);

    return true;
}

void v8::gui::basic_window::message_loop(
    v8::rendering::renderer* graphics_context
    ) {
    MSG msg = { 0 };
    
    while (msg.message != WM_QUIT && !m_windata.quitflag) {
        while (!PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
            app_main_loop(graphics_context);
        }

        do {
            v8_int_t ret_code = GetMessage(&msg, nullptr, 0, 0);
            
            if (ret_code == -1) {
                OUTPUT_DBG_MSGA("GetMessage() error, %d", GetLastError());
                m_windata.quitflag = true;
                break;
            }
            
            if (ret_code == 0) {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } while (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE));
    }
}

void v8::gui::basic_window::app_main_loop(
    v8::rendering::renderer* graphics_context
    ) {
    if (!m_windata.initialized)
        return;
    
    if (m_windata.minimized) {
        on_app_idle();
        return;
    }

    if (m_windata.active) {
        app_do_frame(graphics_context);
        Sleep(2);
        return;
    } else {
        on_app_idle();
    }

    if (m_windata.quitflag) {
        PostMessage(m_windata.win, WM_CLOSE, 0, 0);
    }
}

void 
v8::gui::basic_window::app_do_frame(v8::rendering::renderer* graphics_context) {
    app_frame_tick();
    app_frame_draw(graphics_context);
}

void v8::gui::basic_window::app_frame_tick() {
    const float delta_tm = m_runstats.timer.tick();
    m_runstats.fpscount.frame_tick(delta_tm);
    Delegates_UpdateEvent.call_delegates(delta_tm);
}

LRESULT 
WINAPI 
v8::gui::basic_window::window_procedure_stub(
    HWND window, UINT msg, WPARAM w_param, LPARAM l_param
    ) {
    if (msg == WM_CREATE) {
        basic_window* objPtr = static_cast<basic_window*>(
            reinterpret_cast<CREATESTRUCT*>(l_param)->lpCreateParams);
        SetWindowLongPtr(window, GWLP_USERDATA, 
                         reinterpret_cast<LONG_PTR>(objPtr));
        return true;
    }

    basic_window* objPtr = reinterpret_cast<basic_window*>(
        GetWindowLongPtr(window, GWLP_USERDATA));

    return objPtr ? objPtr->window_procedure(msg, w_param, l_param) :
        DefWindowProc(window, msg, w_param, l_param);
}

void v8::gui::basic_window::handle_wm_size(
    WPARAM sizing_request, 
    v8_int32_t newWidth, 
    v8_int32_t newHeight
    ) {
    m_windata.width = newWidth;
    m_windata.height = newHeight;

    switch (sizing_request) {
    case SIZE_MINIMIZED :
        m_windata.minimized = true;
        return;
        break;

    case SIZE_MAXIMIZED : case SIZE_RESTORED :
        m_windata.minimized = false;
        break;

    default :
        break;
    }

    if ((newWidth > 0) && (newHeight > 0)) {
        resize_event re = { newWidth, newHeight };
        Subscribers_ResizeEvent.call_delegates(re);
    }
}

v8_bool_t v8::gui::basic_window::handle_wm_key_down(
    v8_int_t key_code, 
    LPARAM /*l_param*/
    ) {

    input_event new_event;
    new_event.type = InputEventType::Key;
    new_event.key_ev.down = true;
    new_event.key_ev.key = input::os_key_scan_to_app_key(key_code);
    Subscribers_InputEvents.call_delegates(new_event);
    return true;
}

v8_bool_t v8::gui::basic_window::handle_wm_key_up(
    v8_int_t key_code, 
    LPARAM /*l_param*/
    ) {
    
    input_event new_event;
    new_event.type = InputEventType::Key;
    new_event.key_ev.down = false;
    new_event.key_ev.key = input::os_key_scan_to_app_key(key_code);
    Subscribers_InputEvents.call_delegates(new_event);
    return true;
}

v8_bool_t v8::gui::basic_window::handle_wm_syscommand(
    WPARAM /*w_param*/, 
    LPARAM /*l_param*/
    ) {
    return false;
}

v8_bool_t v8::gui::basic_window::handle_wm_mousewheel(
    WPARAM w_param, 
    LPARAM l_param
    ) {
    input_event ie;
    ie.type = InputEventType::Mouse_Wheel;
    ie.mouse_wheel_ev.delta = GET_WHEEL_DELTA_WPARAM(w_param);
    ie.mouse_wheel_ev.x_pos = GET_X_LPARAM(l_param);
    ie.mouse_wheel_ev.y_pos = GET_Y_LPARAM(l_param);
    Subscribers_InputEvents.call_delegates(ie);

    return true;
}

v8_bool_t v8::gui::basic_window::handle_wm_leftbutton_down(
    WPARAM /*w_param*/, 
    LPARAM l_param
    ) {
    SetCapture(m_windata.win);
    m_windata.mousecaptured = true;

    input_event ie;
    ie.type = InputEventType::Mouse_Button;
    ie.mouse_button_ev.id = MouseButton::Left;
    ie.mouse_button_ev.down = true;
    ie.mouse_button_ev.x_pos = GET_X_LPARAM(l_param);
    ie.mouse_button_ev.y_pos = GET_Y_LPARAM(l_param);
    Subscribers_InputEvents.call_delegates(ie);

    return true;
}

v8_bool_t v8::gui::basic_window::handle_wm_leftbutton_up(
    WPARAM /*w_param*/, 
    LPARAM l_param
    ) {
    ReleaseCapture();
    m_windata.mousecaptured = false;

    input_event ie;
    ie.type = InputEventType::Mouse_Button;
    ie.mouse_button_ev.id = MouseButton::Left;
    ie.mouse_button_ev.down = false;
    ie.mouse_button_ev.x_pos = GET_X_LPARAM(l_param);
    ie.mouse_button_ev.y_pos = GET_Y_LPARAM(l_param);
    Subscribers_InputEvents.call_delegates(ie);

    return true;
}

v8_bool_t v8::gui::basic_window::handle_wm_mousemove(
    WPARAM /*w_param*/, 
    LPARAM l_param
    ) {
    if (!m_windata.mousecaptured) {
        return false;
    }

    input_event ie;
    ie.type = InputEventType::Mouse_Movement;
    ie.mouse_move_ev.x_pos = GET_X_LPARAM(l_param);
    ie.mouse_move_ev.y_pos = GET_Y_LPARAM(l_param);
    Subscribers_InputEvents.call_delegates(ie);

    return true;
}

void v8::gui::basic_window::handle_wm_activate(
    WPARAM w_param
    ) {
    const v8_int_t status = LOWORD(w_param);
    m_windata.active = (status == WA_ACTIVE) || (status == WA_CLICKACTIVE);
}

LRESULT 
v8::gui::basic_window::window_procedure(
    UINT msg, 
    WPARAM w_param, 
    LPARAM l_param
    ) {

    using namespace std;
    tuple<v8_bool_t, LRESULT> message_status(make_tuple(false, 0L));
    
    switch (msg) {
    case WM_CLOSE :
        handle_wm_close();
        return 0L;
        break;

    case WM_DESTROY :
        handle_wm_destroy();
        return 0L;
        break;

    case WM_ENTERSIZEMOVE :
        handle_wm_enter_sizemove();
        return 0L;
        break;

    case WM_EXITSIZEMOVE :
        handle_wm_exit_sizemove();
        return 0L;
        break;

    case WM_SIZE :
        handle_wm_size(w_param, LOWORD(l_param), HIWORD(l_param));
        return 0L;
        break;

    case WM_KEYDOWN :
        message_status = make_tuple(
            handle_wm_key_down((int) w_param, l_param), 0L);
        break;

    case WM_KEYUP :
        message_status = make_tuple(
            handle_wm_key_up((int) w_param, l_param), 0L);
        break;

    case WM_GETMINMAXINFO :
        handle_wm_getminmaxinfo(reinterpret_cast<MINMAXINFO*>(l_param));
        return 0L;
        break;

    case WM_SYSCOMMAND :
        message_status = make_tuple(
            handle_wm_syscommand(w_param, l_param), 0L);
        break;

    case WM_MOUSEWHEEL :
        message_status = make_tuple(
            handle_wm_mousewheel(w_param, l_param), 0);
        break;

    case WM_MOUSEMOVE :
        message_status = make_tuple(
            handle_wm_mousemove(w_param, l_param), 0);
        break;

    case WM_LBUTTONDOWN :
        message_status = make_tuple(
            handle_wm_leftbutton_down(w_param, l_param), 0);
        break;

    case WM_LBUTTONUP :
        message_status = make_tuple(
            handle_wm_leftbutton_up(w_param, l_param), 0);
        break;

    case WM_ACTIVATE :
        message_status = make_tuple(true, 0L);
        handle_wm_activate(w_param);
        break;

    default :
        break;
    }

    if (get<0>(message_status)) {
        return get<1>(message_status);
    }
    
    return DefWindowProc(m_windata.win, msg, w_param, l_param);
}

void 
v8::gui::basic_window::app_frame_draw(v8::rendering::renderer* graphics_context) {
    v8::rendering::FramePresent::Type present_flag = rendering::FramePresent::All;

    //
    // Don't draw anything if occluded
    if (m_windata.occluded) {
        on_app_idle();
        present_flag = rendering::FramePresent::Test;
    } else {
        //frame_draw_impl(graphics_context);
        Delegates_DrawEvent.call_delegates();
    }

    auto ret_code = graphics_context->present_frame(present_flag);

    if (ret_code == rendering::FramePresentResult::Ok) {
        m_windata.occluded = false;
    } else if (ret_code == rendering::FramePresentResult::WindowOccluded) {
        m_windata.occluded = true;
    } else {
        OUTPUT_DBG_MSGA("Frame present error");
        m_windata.quitflag = true;
    }
}