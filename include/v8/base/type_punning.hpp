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

#include <v8/v8.hpp>
#include <v8/meta_programming/constraints.hpp>

namespace v8 { namespace base {

//!
//! \brief Allows casting from a POD type to another POD type, via a union
//! (aka type punning).
//! \param from_type    Source type.
//! \param to_type      Destination type.
//! \remarks Both <b>from_type</b> and <b>to_type</b> must be POD types.
template
<
    typename from_type,
    typename to_type
>
union type_punning {
public :
    //! \name Typedefs.
    //! @{

    //! Type of an object of this union.
    typedef type_punning<from_type, to_type>                class_type;

    //! @}

public :

    //! \name Constructors/destructors.
    //! @{

    //! \brief Default constructor. Leaves members uninitialized.
    type_punning() {}

    //! \brief Construct from an existing value
    //! \param from_val Value to cast from.
    type_punning(const from_type& val_from)
        : in_value(val_from)
    {}

    ~type_punning() {
        constraint_must_be_pod<from_type>();
        constraint_must_be_pod<to_type>();
    }

    //! @}

public :

    //! \name Data members.
    //! @{

    from_type    in_value; //!< Value expressed as the source type.
    to_type      out_value;   //!< Value expressed as the destination type.

    //! @}

};

//!
//! \brief Helper function to convert a value of a POD type to the same value
//! but expressed as another type, via type punning.
//! \param source_val Input value, of type input_type.
//! \returns The value of the source_val parameter,
//! represented as an output_type value.
template<typename output_type, typename input_type>
inline output_type punning_cast(const input_type& source_val) {
    return type_punning<input_type, output_type>(source_val).out_value;
}

} // namespace base
} // namespace v8
