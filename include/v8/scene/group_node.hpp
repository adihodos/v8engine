#pragma once

#include <vector>

#include <v8/v8.hpp>
#include <v8/base/sequence_container_veneer.hpp>
#include <v8/scene/simple_node.hpp>

namespace v8 { namespace scene {

class group_node : public simple_node {
public :

    //! \name Constructors
    //! @{

    group_node(simple_node* parent = nullptr)
        : simple_node(parent) {}

    ~group_node() {}

    //! @}


    //! \name Child nodes management.
    //! @{

public :

    void add_child(simple_node* node);

    void remove_child(simple_node* node);

    void remove_all_children();

    simple_node* release_child(v8_size_t pos);

    v8_size_t get_child_count() const;

    simple_node* get_child(v8_size_t pos) const;

private :

    struct node_destructor {
        void operator()(simple_node* node_ptr) {
            delete node_ptr;
        }
    };

    typedef std::vector<simple_node*>   node_container_type;

    typedef v8::base::sequence_container_veneer
    <
        node_container_type, 
        node_destructor
    > child_container_type;

    //! Container with child nodes rooted at this node. The root node owns
    //! the pointers to the children.
    child_container_type                children_;

    //! @}

private :

    //! \name Disabled operations
    //! @{

    NO_CC_ASSIGN(group_node);

    //! @}
};

} // namespace scene
} // namespace v8
