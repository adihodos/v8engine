#pragma once

#include <algorithm>
#include <vector>
#include <cstring>

#include <v8/v8.hpp>
#include <v8/math/light.hpp>

namespace v8 { namespace scene {

class simple_node {
public :
    //! \name Constructors
    //! @{

    simple_node(simple_node* parent = nullptr)
        : parent_(parent) {
    }

    virtual ~simple_node() {}

    //! @}

public :

    //! \name Lighting operations.
    //! @{

    inline void add_light(const v8::math::light& light);

    inline void remove_light(const v8::math::light& light);

    inline void remove_light(v8_size_t index);

    inline void remove_all_lights();

    inline v8::math::light* get_light(v8_size_t index);

    inline v8_size_t get_lights_count() const;

    //! @}

public :

    //! \name Tree operations.
    //! @{


    simple_node* get_parent() const {
        return parent_;
    }

    void set_parent(simple_node* parent) {
        parent_ = parent;
    }

    //! @}

protected :

    //! \name Data members
    //! @{

    //! Stores lights attached to this node. Lights attached to a node
    //! affect all children rooted at that node.
    std::vector<v8::math::light>    attached_lights_;

    //! Parent of this node (null if the node is the root node).
    simple_node*    parent_;

    //! @}

private :

    //! \name Disabled operations.
    //! @{

    NO_CC_ASSIGN(simple_node);

    //! @}
};

inline void simple_node::add_light(const v8::math::light& light) {
    attached_lights_.push_back(light);
}

inline void simple_node::remove_light(const v8::math::light& light) {
    using namespace std;

    attached_lights_.erase(
        remove_if(begin(attached_lights_), end(attached_lights_),
                  [light](const v8::math::light& l0) {
        return !memcmp(&l0, &light, sizeof(l0));
    }));
}

inline void simple_node::remove_all_lights() {
    attached_lights_.clear();
}

inline v8::math::light* simple_node::get_light(v8_size_t index) {
    return index < attached_lights_.size() ? &attached_lights_[index] : nullptr;
}

inline v8_size_t simple_node::get_lights_count() const {
    return attached_lights_.size();
}

} // namespace scene
} // namespace v8
