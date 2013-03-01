#pragma once

#include <v8/v8.hpp>
#include <v8/base/com_shared_pointer.hpp>
#include <v8/base/operator_bool.hpp>

namespace v8 { namespace directx {

//!
//! \brief An opaque handle to a DirectX resource (shader, shader resource view,
//! texture 1D/2D/3D, etc). It <b>does not own</b> the actual underlying 
//! resource, being designed to abstract DirectX resources in an API independent
//! way.
//template<typename directx_resource_type>
//class directx_resource_handle {
//public :
//    typedef directx_resource_type                               value_type;
//    typedef directx_resource_type*                              handle_type;
//    typedef directx_resource_handle<directx_resource_type>      class_type;
//    
//    directx_resource_handle(handle_type res_handle = nullptr)
//        : handle_(res_handle) {}
//        
//    //! \name Sanity checking
//    //! @{
//        
//    bool operator!() const NOEXCEPT {
//        return handle_ == nullptr;
//    }
//    
//    operator int bool_helper::*() const NOEXCEPT {
//        return handle_ == nullptr ? nullptr : &bool_helper::a_member;
//    }
//    
//    //! @}
//    
//    //! \name Equality/inequality operators
//    //! @{
//    
//    bool operator==(const class_type& rhs) const NOEXCEPT {
//        return handle_ == rhs.handle_;
//    }
//    
//    bool operator!=(const class_type& rhs) const NOEXCEPT {
//        return !(*this == rhs);
//    }
//    
//    //! @}
//    
//private :
//
//    handle_type get_native_handle() const NOEXCEPT {
//        return handle_;
//    }
//
//    void set_native_handle(handle_type new_handle) NOEXCEPT {
//        handle_ = new_handle;
//    }
//    
//    handle_type handle_; //!< Handle to the native directx resource.
//};

template<typename directx_resource_type>
struct directx_resource_traits {
    typedef directx_resource_type                                   value_type;
    typedef directx_resource_type*                                  handle_type;                                   
};

} // namespace directx
} // namespace v8
