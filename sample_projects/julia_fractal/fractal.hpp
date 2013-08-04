#pragma once

#include <complex>

#include <v8/v8.hpp>
#include <v8/event/fwd_event_types.hpp>
#include <v8/event/input_event.hpp>
#include <v8/rendering/fwd_renderer.hpp>
#include <v8/base/scoped_pointer.hpp>
#include "application_context.hpp"

class fractal {

public :

    //! \name Defined types.
    //! @{

    typedef std::complex<float>                             complex_type;

    //! @}

    //! \name Construction/destruction
    //! @{

public :
    fractal(
        const v8_int_t width = 1024,
        const v8_int_t height = 1024,
        const v8_int_t iter = 256,
        const float zoom = 1.0f
        );

    ~fractal();

    v8_bool_t initialize(fractal_app_context& app_context);

private :
    v8_bool_t create_color_table(fractal_app_context& app_context);

    //! @}

    //! \name Fractal parameters.
    //! @{

public :

    v8_int_t get_width() const;

    v8_int_t get_height() const;

    void set_width(const v8_int_t w);

    void set_height(const v8_int_t h);

    v8_int_t get_iteration_count() const;

    void set_iteration_count(const v8_int_t iterations);

    float get_zoom_factor() const;

    void set_zoom_factor(const float z);

    void set_x_offset(const float off_x);

    float get_x_offset() const;

    void set_y_offset(const float off_y);

    float get_y_offset() const;

    void set_constant(const complex_type& const_factor);

    complex_type get_constant() const;

    //! @}

public :

    //! \name Update and rendering
    //! @{

    void evaluate(const float delta_ms);

    void draw(v8::rendering::renderer* draw_context);

    //! @}

public :

    //! \name Input events handling.
    //! @{

public :

    void on_input(const v8::input_event& ev_input);

    void on_resize(const v8::resize_event& ev_resize);

private :

    v8_bool_t mouse_wheel_event(
        const v8_int_t rotations,
        const v8_int_t xpos, 
        const v8_int_t ypos
        );

    void mouse_button_event(const v8::mouse_button_event_t& evt);

    v8_bool_t key_press_event(const v8_int_t key_code);

    v8_bool_t key_depress_event(const v8_int_t key_code);

    //! @}

private :
    struct implementation;
    v8::base::scoped_ptr<implementation>                impl_;

private :
    NO_CC_ASSIGN(fractal);
};
