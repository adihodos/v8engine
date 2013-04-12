#pragma once

#include <v8/v8.hpp>
#include <v8/base/scoped_pointer.hpp>

#include <v8/rendering/fwd_renderer.hpp>
#include <v8/rendering/fwd_effect.hpp>

struct draw_context_t;

class animated_aircraft {
public :
    
    animated_aircraft();

    ~animated_aircraft();

public :
    
    v8_bool_t initialize(
        const char* const model_config, 
        v8::rendering::renderer* r_sys
        );

    void update(const float delta_tm);

    void draw(const draw_context_t& draw_con, void* vs, void* ps);

private :
    v8_bool_t load_mesh(
        const char* file_path, v8::rendering::renderer* rsys
        );

    v8_bool_t load_effect();    

private :
    struct implementation_details;
    v8::base::scoped_ptr<implementation_details>    pimpl_;

private :
    NO_CC_ASSIGN(animated_aircraft);    
};