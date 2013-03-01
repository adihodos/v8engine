//
// Copyright (c) 2011, 2012, Adrian Hodos
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the author nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR THE CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

//! \file external_property.hpp
//! \brief Simple, but somewhat ugly and non portable way to emulate
//! properties for C++ classes.

#include <cstddef>
#include <v8/v8.hpp>

namespace v8 { namespace base {

//! \addtogroup v8_base_lib
//! @{

//!
//! \brief Implements a read-only property. The class will actually call
//! a delegate function that is passed as a nontype template parameter, to
//! return the property's value to the caller. It is merely "syntatic sugar"
//! for a call to a class method.
template
<
    typename class_ty,
    typename prop_return_type,
    prop_return_type (class_ty::*prop_get_func)() const,
    ptrdiff_t (*prop_get_offset_func)()
>
class external_property_get {
public :
    typedef external_property_get
    <
        class_ty,
        prop_return_type,
        prop_get_func,
        prop_get_offset_func
    >                                                       class_type;

    operator prop_return_type() const {
        class_ty const* obj =
            (class_ty const*) ((char*) this - prop_get_offset_func());
        return (obj->*prop_get_func)();
    }
};

//!
//! \brief Implements a write-only property. The class will actually call
//! a delegate function that is passed as a nontype template parameter, to
//! return the property's value to the caller. It is merely "syntatic sugar"
//! for a call to a class method.
template
<
    typename class_ty,
    typename prop_arg_type,
    void (class_ty::*prop_set_func)(prop_arg_type),
    ptrdiff_t (*prop_get_offset_func)()
>
class external_property_set {
public :
    typedef external_property_set
    <
        class_ty,
        prop_arg_type,
        prop_set_func,
        prop_get_offset_func
    >                                                       class_type;

    class_type& operator=(prop_arg_type value) {
        class_ty* obj = (class_ty*) ((char*) this - prop_get_offset_func());
        (obj->*prop_set_func)(value);
        return *this;
    }
};

//!
//! Implements a read-write property.
template
<
    typename class_ty,
    typename prop_return_type,
    prop_return_type (class_ty::*prop_get_func)() const,
    typename prop_arg_type,
    void (class_ty::*prop_set_func)(prop_arg_type),
    ptrdiff_t (*prop_get_offset_func)()
>
class external_property_get_set {
public :

    typedef external_property_get_set
    <
        class_ty,
        prop_return_type,
        prop_get_func,
        prop_arg_type,
        prop_set_func,
        prop_get_offset_func
    >                                                       class_type;

    operator prop_return_type() const {
        class_ty const* obj =
            (class_ty const*) ((char*) this - prop_get_offset_func());
        return (obj->*prop_get_func)();
    }

    class_type& operator=(prop_arg_type value) {
        class_ty* obj = (class_ty*) ((char*) this - prop_get_offset_func());
        (obj->*prop_set_func)(value);
        return *this;
    }
};

} // namespace base
} // namespace v8

//!
//! \def MK_PROPERTY_OFFSET_FUNC
//! \brief Convenience macro to define a static function that will return
//! the offset of the property member in the given class.
//!
//! \param class_name Name of the class that contains the property.
//! \param property_name Name of the property.
//!
//! \note Because this function uses the offsetof() macro to compute the
//! offset of the property, some compiler will give warnings about it
//! (GCC especially). Furthermore, code using the macro,
//! <b> is not portable </b>.
//! Given a class T, for each property U of T, there must be
//! a MK_PROPERTY_OFFSET_FUNC(T, U) preceding the property declaration macro.
//! It is recommended to put these macros in the private part of the class.
#ifndef MK_PROPERTY_OFFSET_FUNC
#define MK_PROPERTY_OFFSET_FUNC(class_name, property_name)  \
    static ptrdiff_t get_prop_offset_ ## property_name() {  \
        return offsetof(class_name, property_name);         \
    }
#endif

//!
//! \def EXTERNAL_PROPERTY_GET
//! \brief Convenience macro to define a read only property of a class.
//!
//! \param class_name Name of the class containing the property.
//! \param prop_return_type Type returned by the property getter function.
//! \param prop_get_func Name of the class method that implements retrieval
//! of the property's value.
//! \param prop_name Name of the property.
//!
//! \code
//! class vector2 {
//! public :
//!   vector2()  {}
//!
//!   vector2(float x, float y) {
//!       data_[0] = x;
//!       data_[1] = y;
//!   }
//! private :
//!    MK_PROPERTY_OFFSET_FUNC(vector2, LengthSquared);
//!    MK_PROPERTY_OFFSET_FUNC(vector2, Length);
//! private :
//!   float data_[2];
//!
//!   float length_squared() const {
//!       return data_[0] * data_[0] + data_[1] * data_[1];
//!   }
//!
//!   float length() const {
//!       return sqrt(length_squared());
//!   }
//! public :
//!   BEGIN_CLASS_PROPERTY_DEFS
//!
//!       EXTERNAL_PROPERTY_GET(vector2, float, length_squared, LengthSquared);
//!       EXTERNAL_PROPERTY_GET(vector2, float, length, Length);
//!
//!   END_CLASS_PROPERTY_DEFS
//!
//! };
//!
//! //
//! // In code you can simply write :
//! vector2 v0(-3.0f, 4.0f);
//! do_computation(v0.Length);
//! do_another_computation(v0.LengthSquared);
//! \endcode
#ifndef EXTERNAL_PROPERTY_GET
#define EXTERNAL_PROPERTY_GET(  \
    class_name, prop_return_type, prop_get_func, prop_name  \
    )   \
    friend v8::base::external_property_get \
    <   \
        class_name, prop_return_type, &class_name::prop_get_func, \
        &class_name::get_prop_offset_ ## prop_name \
    >;  \
    v8::base::external_property_get \
    < \
        class_name, prop_return_type, &class_name::prop_get_func, \
        &class_name::get_prop_offset_ ## prop_name \
    > prop_name;
#endif

//!
//! \def EXTERNAL_PROPERTY_SET
//! \brief Convenience macro to define a write-only property for a class.
//!
//! \param class_name Name of the class containing the property.
//! \param prop_arg_type Type of argument to the property setter function.
//! \param prop_set_func Name of the class method that implements
//! write access to the property.
//! \param prop_name Name of the property.
#ifndef EXTERNAL_PROPERTY_SET
#define EXTERNAL_PROPERTY_SET(  \
    class_name, prop_arg_type, prop_set_func, prop_name  \
    )   \
    friend v8::base::external_property_set \
    <   \
        class_name, prop_arg_type, &class_name::prop_set_func, \
        &class_name::get_prop_offset_ ## prop_name \
    >;  \
    v8::base::external_property_set \
    < \
        class_name, prop_arg_type, &class_name::prop_set_func, \
        &class_name::get_prop_offset_ ## prop_name \
    > prop_name;
#endif

//!
//! \def EXTERNAL_PROPERTY_GET_SET
//! \brief Convenience macro to define a read-write property for a class.
//!
//! \param class_name Name of the class containing the property.
//! \param prop_return_type Type returned by the property getter function.
//! \param prop_get_func Name of the class method that implements retrieval
//! of the property's value.
//! \param prop_arg_type Type of argument to the property setter function.
//! \param prop_set_func Name of the class method that implements
//! write access to the property.
//! \param prop_name Name of the property.
//!
//! \code
//! class vector2 {
//! public :
//!   vector2()  {}
//!
//!   vector2(float x, float y) {
//!       data_[0] = x;
//!       data_[1] = y;
//!   }
//! private :
//!    MK_PROPERTY_OFFSET_FUNC(vector2, X);
//!    MK_PROPERTY_OFFSET_FUNC(vector2, Y);
//! private :
//!   float data_[2];
//!
//!   float length_squared() const {
//!       return data_[0] * data_[0] + data_[1] * data_[1];
//!   }
//!
//!   float length() const {
//!       return sqrt(length_squared());
//!   }
//! public :
//!   BEGIN_CLASS_PROPERTY_DEFS
//!
//!       EXTERNAL_PROPERTY_GET_SET(vector2, float, get_x, float, set_x, X);
//!       EXTERNAL_PROPERTY_GET_SET(vector2, float, get_y, float, set_y, Y);
//!
//!   END_CLASS_PROPERTY_DEFS
//!
//! };
//!
//! //
//! // some code
//! vector2 v0;
//! v0.X = -3.0f;
//! v0.Y = +4.0f;
//! normalize(v0);
//!
//! \endcode
#ifndef EXTERNAL_PROPERTY_GET_SET
#define EXTERNAL_PROPERTY_GET_SET(                  \
    class_name, prop_return_type, prop_get_func,    \
    prop_arg_type, prop_set_func,                   \
    prop_name                                       \
    )                                               \
    friend v8::base::external_property_get_set      \
    <                                               \
        class_name, prop_return_type,               \
        &class_name::prop_get_func,                 \
        prop_arg_type,                              \
        &class_name::prop_set_func,                 \
        &class_name::get_prop_offset_ ## prop_name  \
    >;                                              \
    v8::base::external_property_get_set             \
    <                                               \
        class_name, prop_return_type,               \
        &class_name::prop_get_func,                 \
        prop_arg_type,                              \
        &class_name::prop_set_func,                 \
        &class_name::get_prop_offset_ ## prop_name  \
    > prop_name;
#endif

//!
//! \def BEGIN_CLASS_PROPERTY_DEFS
//! \brief The purpose of this macro is to ensure that only a small size penalty
//! is incured when using properties. Even though the objects implementing
//! properties do not have any members and simply call the
//! specified delegate functions, each property defined in a class would
//! increase the size of the class by a small amount (one byte usually).
//! This macro simply begins the definition of an anonymous union. If all property
//! definitions are placed after this macro, then the size penalty
//! remains the same, irrespective of the number of defined properties (since
//! they're all members of the same union). Any list of property
//! definitions stated with this macro, must end with the
//! END_CLASS_PROPERTY_DEFS macro.
//!
//! \note Properties must be the last defined members in a class, otherwise
//! the compiler complains about the use of incomplete types.
//!
//! \code
//! class vector2 {
//! ...
//! public :
//! BEGIN_CLASS_PROPERTY_DEFS
//!     EXTERNAL_PROPERTY_GET_SET(vector2, float, get_x, float, set_x, X);
//!     EXTERNAL_PROPERTY_GET_SET(vector2, float, get_y, float, set_y, Y);
//!     EXTERNAL_PROPERTY_GET(vector2, float, length_squared, LengthSquared);
//!     EXTERNAL_PROPERTY_GET(vector2, float, length, Length);
//! END_CLASS_PROPERTY_DEFS
//!
//! };
//! \endcode
#ifndef BEGIN_CLASS_PROPERTY_DEFS
#define BEGIN_CLASS_PROPERTY_DEFS union {
#endif

//!
//! \def END_CLASS_PROPERTY_DEFS
//! \brief Ends a member property definition list started with the
//! BEGIN_CLASS_PROPERTY_DEFS macro.
#ifndef END_CLASS_PROPERTY_DEFS
#define END_CLASS_PROPERTY_DEFS };
#endif

//! @}
