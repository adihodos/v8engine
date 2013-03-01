#pragma once

#include <utility>
#include <v8/v8.hpp>
#include <v8/base/com_exclusive_pointer.hpp>
#include <v8/base/operator_bool.hpp>
#include <v8/rendering/directx/fwd_renderer.hpp>

namespace v8 { namespace directx {

template<typename state_traits>
class api_state {
public :
    typedef typename state_traits::handle_type                  handle_type;
    typedef typename state_traits::descriptor_type              descriptor_type;

    typedef api_state<state_traits>                             class_type;

    api_state(class_type&& rval)
        : state_handle_(std::move(rval.state_handle_))
    {}

    class_type& operator=(class_type&& rval) {
        state_handle_ = std::move(rval.state_handle_);
        return *this;
    }

    handle_type* internal_np_get_handle() const {
        return v8::base::scoped_pointer_get(state_handle_);
    }

    operator int v8::base::operator_bool::*() const {
        return state_handle_ == nullptr ? nullptr : &v8::base::operator_bool::a_member;
    }

    v8_bool_t initialize(const descriptor_type& state_desc, renderer* rsys) {
        if (!state_handle_) {
            state_handle_ = state_traits::create_state(state_desc, rsys);
        }
        return state_handle_ != nullptr;
    }

private :
    typename v8::base::com_exclusive_pointer<handle_type>::type   state_handle_;

private :
    NO_CC_ASSIGN(api_state);
};

} // namespace directx
} // namespace v8
