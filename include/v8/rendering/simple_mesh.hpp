#pragma once

#include <v8/v8.hpp>
#include <v8/base/operator_bool.hpp>
#include <v8/rendering/fwd_renderer.hpp>
#include <v8/rendering/vertex_buffer.hpp>
#include <v8/rendering/index_buffer.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/utility/hash_string.hpp>

namespace v8 { namespace rendering {

// class simple_mesh {
// /// \name Construction/init
// /// @{

// public :    
//     simple_mesh();

//     ~simple_mesh();

//     simple_mesh(const hash_string& mesh_name, renderer* render_sys);
        
//     v8_bool_t initialize(const hash_string& mesh_name, renderer* render_sys);

// /// @}    

// /// \name Sanity checking
// /// @{

// public :

//     v8_bool_t is_valid() const {
//         return valid_;
//     }    

//     operator int base::operator_bool::*() const {
//         return is_valid() ? &base::operator_bool::a_member : nullptr;
//     }

// /// @}   

// /// \name Attributes.
// /// @{

// public :

//     vertex_buffer* get_vertex_buffer() {
//         return &vertex_buffer_;
//     }

//     index_buffer* get_index_buffer() {
//         return &index_buffer_;
//     }

//     PrimitiveTopology::Type get_topology() const {
//         return topology_;
//     }

// /// @}    

// private :    

//     vertex_buffer                       vertex_buffer_;
//     index_buffer                        index_buffer_;
//     PrimitiveTopology::Type             topology_;
//     v8_bool_t                           valid_;

// private :
//     NO_CC_ASSIGN(simple_mesh);    
// };

} // namespace rendering
} // namespace v8
