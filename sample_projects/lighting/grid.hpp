#pragma once

#include <v8/v8.hpp>
#include <v8/rendering/fwd_renderer.hpp>
#include <v8/scene/custom_entity.hpp>

class simple_grid : public v8::scene::custom_entity {
public :
    typedef v8::scene::custom_entity                  base_class;

    simple_grid() 
        : base_class()
    {}

    struct Height_Function {
        enum Value {
            Sin_X2Z2
        };
    };

    v8_bool_t initialize(const char* config_file);

    void pre_draw(v8::scene::scene_system*, v8::rendering::renderer*);

    void post_draw(v8::scene::scene_system*, v8::rendering::renderer*);

    void update(float delta);

private :
    struct grid_parms_t {
        v8_int_t    num_rows;
        v8_int_t    num_columns;
        float       width;
        float       height;
        v8_int_t    height_fn;

        grid_parms_t() {
            set_defaults();
        }

        void set_defaults() {
            num_rows    = 128;
            num_columns = 128;
            width       = height = 8.0f;
            height_fn   = Height_Function::Sin_X2Z2;
        }
    };

    grid_parms_t                                                grid_config_;

private :
    NO_CC_ASSIGN(simple_grid);
};
