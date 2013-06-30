#include "v8/base/debug_helpers.hpp"
#include "v8/scene/camera_controller.hpp"
#include "v8/scene/scene_entity.hpp"

#include "v8/scene/scene_system.hpp"

v8::scene::scene_system::scene_system()
    :       m_light_count(0)
        ,   m_active_light_count(0)
        ,   m_active_list(0)
{}

v8::scene::scene_system::~scene_system() {}

void 
v8::scene::scene_system::update(float delta_ms) {
    assert(check_valid());

    m_cam_controller->update(delta_ms);

    using namespace std;
    for_each(begin(m_entity_list), end(m_entity_list), [delta_ms](scene_entity* s_ent) {
        s_ent->update(delta_ms);
    });
}

void 
v8::scene::scene_system::pre_draw(v8::rendering::renderer*) {
    m_active_light_count = 0;
    for (v8_int_t light_idx = 0; light_idx < m_light_count; ++light_idx) {
        if (is_light_on(light_idx)) {
            m_active_lights[m_active_light_count++] = m_scene_ligths[light_idx];
        }
    }
}

void 
v8::scene::scene_system::post_draw(v8::rendering::renderer*) {}

void 
v8::scene::scene_system::draw(v8::rendering::renderer* render_sys) {
    assert(check_valid());

    pre_draw(render_sys);

    using namespace std;
    scene_system* scene_sys = this;
    for_each(
        begin(m_entity_list),
        end(m_entity_list),
        [scene_sys, render_sys](scene_entity* s_ent) {
            s_ent->pre_draw(scene_sys, render_sys);
            s_ent->draw(scene_sys, render_sys);
            s_ent->post_draw(scene_sys, render_sys);
    });

    post_draw(render_sys);
}

void 
v8::scene::scene_system::depth_sort_all() {
    assert(check_valid());
}
