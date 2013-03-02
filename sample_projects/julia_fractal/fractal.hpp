#pragma once

#include <complex>

#include <v8/v8.hpp>
#include <v8/event/fwd_event_types.hpp>
#include <v8/base/scoped_pointer.hpp>

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
        int width = 1024,
        int height = 1024,
        int iter = 256,
        float zoom = 1.0f
        );

    ~fractal();

    v8_bool_t initialize();

    //! @}

    //! \name Fractal parameters.
    //! @{

public :

    int get_width() const;

    int get_height() const;

    void set_width(int w);

    void set_height(int h);

    int get_iteration_count() const;

    void set_iteration_count(int iterations);

    float get_zoom_factor() const;

    void set_zoom_factor(float z);

    void set_x_offset(float off_x);

    float get_x_offset() const;

    void set_y_offset(float off_y);

    float get_y_offset() const;

    void set_constant(const complex_type& const_factor);

    complex_type get_constant() const;

    //! @}

public :

    //! \name Update and rendering
    //! @{

    void evaluate(float delta_ms);

    void draw();

    //! @}

public :

    //! \name Input events handling.
    //! @{

public :

    void on_input(const v8::input_event& ev_input);

    void on_resize(const v8::resize_event& ev_resize);

private :

    v8_bool_t mouse_wheel_event(
        v8_int_t rotations,
        v8_int_t xpos, 
        v8_int_t ypos
        );

    v8_bool_t key_press_event(
        v8_int_t key_code
        );

    v8_bool_t key_depress_event(
        v8_int_t key_code
        );

    //! @}

private :
    struct implementation;

    v8::base::scoped_ptr<implementation>                impl_;
};

extern fractal* g_fractal;