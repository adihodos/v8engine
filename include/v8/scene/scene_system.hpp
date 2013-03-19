#pragma once

#include <algorithm>
#include <type_traits>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

#include <v8/v8.hpp>
#include <v8/base/count_of.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/base/sequence_container_veneer.hpp>
#include <v8/base/fixed_pod_vector.hpp>

#include <v8/math/camera.hpp>
#include <v8/math/light.hpp>
#include <v8/io/config_file_reader.hpp>
#include <v8/rendering/fwd_renderer.hpp>
#include <v8/scene/scene_entity.hpp>
#include <v8/scene/camera_controller.hpp>
#include <v8/scene/fwd_scene_loading_info.hpp>

namespace v8 { namespace scene {

///
/// \brief Handles management of objects.
class scene_system {
/// \name Constant attributes.
/// @{

public :

    ///< Maximum number of lights that we support.
    static const v8_int32_t k_max_lights_count = 8U;

/// @}

//! \name Construction/initialisation
//! @{

public :

    scene_system();

    virtual ~scene_system();

//! @}

//! \name Entity management.
//! @{

public :

    //! \brief Adds a new entity to the scene.
    //! \remarks The scene_system takes ownership of the entity.
    void add_entity(scene_entity* new_entity) {
        m_entity_list.push_back(new_entity);
    }

    //! \brief Removes the specified entity from the list. 
    //! The entity is destroyed.
    void remove_entity(scene_entity* entity) {
        auto itr_entity = std::find(std::begin(m_entity_list), 
                                    std::end(m_entity_list), entity);
        if (itr_entity != std::end(m_entity_list)) {
            delete entity;
            m_entity_list.erase(itr_entity);
        }
    }

//! @}

//! \name Lights management.
//! @{

    /// \brief adds another light to the list of lights.
    void push_light(const v8::math::light& light_source, v8_bool_t on = true) {
        assert(m_light_count < k_max_lights_count);
        m_scene_ligths[m_light_count++] = light_source;
        if (on) {
            set_light_on(m_light_count - 1);
        } else {
            set_light_off(m_light_count - 1);
        }
    }

    /// \brief returns a reference to the light with the specified index.
    v8::math::light& get_light(const v8_int32_t light_index) {
        assert(light_index < m_light_count);
        return m_scene_ligths[m_light_count];
    }

    const v8::math::light& get_light(const v8_int32_t light_index) const {
        assert(light_index < m_light_count);
        return m_scene_ligths[m_light_count];
    }

    /// \brief removes the last light pushed onto the light stack.
    void pop_light() {
        assert(m_light_count > 0);
        --m_light_count;
    }

    v8_int32_t get_lights_count() const {
        return m_light_count;
    }

    v8_int32_t get_active_ligths_count() const {
        return m_active_light_count;
    }

    void toggle_light(const v8_int_t k_index) {
        m_active_list = !(m_active_list & (1 << k_index)) 
                        | (m_active_list & ~(1 << k_index));
    }

    void set_light_on(const v8_int_t index) {
        m_active_list = (m_active_list & ~(1 << index)) | (1 << index);
    }

    void set_light_off(const v8_int_t index) {
        m_active_list = (m_active_list & ~(1 << index));
    }

    v8_bool_t is_light_on(const v8_int_t index) const {
        return m_active_list & (1 << index);
    }    

//! @}

//! \name Camera management.
//! @{

public :

    //! \brief Returns the active camera.
    v8::math::camera const* cam() const {
        return &m_camera;
    }

    //! \brief Returns the active camera.
    v8::math::camera* cam() {
        return &m_camera;
    }

    //! \brief Returns the camera controller.
    camera_controller* get_cam_controller() const {
        return v8::base::scoped_pointer_get(m_cam_controller);
    }

    //! \brief Sets the camera's controller.
    //! \remarks The scene_system takes ownership of the controller.
    void set_cam_controller(camera_controller* controller) {
        m_cam_controller = controller;
        m_cam_controller->set_camera(&m_camera);
    }

//! @}

//! \name State updating/drawing
//! @{

public :

    //! \brief Updates the state of all entities.
    virtual void update(float delta_ms);

    //! \brief Draws all (visible) entities in the scene.
    virtual void draw(v8::rendering::renderer*);

protected :

    //! \brief Called before anything is drawn. This is the place to perform
    //! frustrum culling.
    virtual void pre_draw(v8::rendering::renderer*);

    //! \brief Called after the drawing of all visible objects has completed.
    virtual void post_draw(v8::rendering::renderer*);

//! @}

//! \name Various entity manipulations
//! @{

protected :

    void depth_sort_all();

//! @}

//! \name Entity management structures.
//! @{

protected :

    struct entity_destructor {
        void operator()(scene_entity* se) const {
            delete se;
        }
    };

    typedef std::vector<scene_entity*>          entity_list_t;

    typedef v8::base::sequence_container_veneer<
        entity_list_t, 
        entity_destructor
    >                                           scoped_entity_list_t;

//! @}

//! \name Internal helper routines.
//!  @{

protected :

    bool check_valid() const {
        return m_cam_controller;
    }

//! @}

//! \name Data members.
//! @{

protected :

    //! List of existing entities, in the scene.
    scoped_entity_list_t                        m_entity_list;

    //! List of visible entities, from the camera's perspective.
    //! This list contains the objects that are actually drawn.
    std::vector<scene_entity*>                  m_visible_ent_list;

    //! Active camera.
    v8::math::camera                            m_camera;

    //! Active camera controller.
    v8::base::scoped_ptr<camera_controller>     m_cam_controller;

    //! Stores light information.
    v8::math::light                             m_scene_ligths[k_max_lights_count];
    v8::math::light                             m_active_lights[k_max_lights_count];

    //! Numbers of lights in the scene.
    v8_int32_t                                  m_light_count;
    v8_int32_t                                  m_active_light_count;
    v8_byte_t                                   m_active_list;
//! @}

//! \name Disabled operations.
//! @{

private :

    NO_CC_ASSIGN(scene_system);

//! @}
};

} // namespace scene
} // namespace v8
