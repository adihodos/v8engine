#include <cassert>
#include "v8/scene/group_node.hpp"

void v8::scene::group_node::add_child(simple_node* node) {
    assert(node && "Child can not be null");
    //
    //  search node container for a free entry
    //  if free entry found
    //  then
    //      insert child in entry slot
    //  else
    //      push a new entry into the container
    //  endif
    //
    using namespace std;

    auto itr_entry = find_if(begin(children_), end(children_), 
                             [](simple_node* child_ptr) {
        return child_ptr == nullptr;
    });

    if (itr_entry != end(children_)) {
        *itr_entry = node;
    } else {
        children_.push_back(node);
    }
}

void v8::scene::group_node::remove_child(simple_node* node) {
    assert(node && "Child can not be null");
    using namespace std;
    children_.erase(remove(begin(children_), end(children_), node), 
                    end(children_));
}

void v8::scene::group_node::remove_all_children() {
    using namespace std;
    for_each(begin(children_), end(children_), [](simple_node* child_node) {
        node_destructor()(child_node);
    });
    children_.clear();
}

v8::scene::simple_node* 
v8::scene::group_node::release_child(v8_size_t pos) {
    assert((pos < children_.size()) && "Out of bounds child index!");

    auto ret_val = children_[pos];
    children_[pos] = nullptr;
    return ret_val;
}

v8_size_t v8::scene::group_node::get_child_count() const {
    return children_.size();
}

v8::scene::simple_node* 
v8::scene::group_node::get_child(v8_size_t pos) const {
    assert((pos < children_.size()) && "Out of bounds child index!");
    return children_[pos];
}