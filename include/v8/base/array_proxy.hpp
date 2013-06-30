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

/**
 * \file array_proxy.hpp
 */

#include <iterator>
#include <v8/v8.hpp>
#include <v8/meta_programming/constraints.hpp>

namespace v8 { namespace base {

/**
 * \class array_proxy
 * \brief Light-weight wrapper around built-in arrays, providing helper and
 *        convenience functions for safe array manipulation. Mostly
 *        inspired by <b>Imperfect C++</b>, section 14.4.
 * \code
 *  int array[100];
 *  array_proxy<int> ap(array);
 *
 *  for (auto i = 0; i < ap.lenght(); ++i) {
 *      *(ap.base() + i) = 0;
 *      ap[i] = i;
 *  }
 * \code
 */
template<typename T>
class array_proxy {
public :
    //! \name Typedefs
    //! @{
    
    //! Type of elements stored in the array.
    typedef T                                       value_type;
    
    //! Type of pointer to element.
    typedef T*                                      pointer;
    
    //! Type of const pointer to element.
    typedef T*                                      const_pointer;
    
    //! Type of reference to element.
    typedef T&                                      reference;
    
    //! Type of const reference to element.
    typedef T&                                      const_reference;
    
    //! Type of iterator.
    typedef T*                                      iterator;
    
    //! Type of const iterator.
    typedef T*                                      const_iterator;
    
    //! Type of reversed iterator.
    typedef std::reverse_iterator<iterator>         reverse_iterator;
    
    //! Type of const reverse iterator.
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;
    
    //! Type used for indexing and size.
    typedef v8_size_t                               size_type;

    //! Fully qualified type of this class.
    typedef array_proxy<T>                          class_type;
    
    //! @}

public :

    /** \name Constructors
     * @{
     */

    /**
     * \brief Construct from pointers to a range of elements.
     */
    array_proxy(T* beg, T* end) : begin_(beg), end_(end) {}

    /**
     * \brief Construct with a pointer and an element count.
     */
    array_proxy(T* beg, const v8_size_t num_elements)
        : begin_(beg), end_(begin_ + num_elements) {}

    /**
     * \brief Construct from an existing array.
     */
    template<v8_size_t N>
    array_proxy(T (&arr)[N]) : begin_(&arr[0]), end_(&arr[0] + N) {}

    /**
     * \brief Construct from pointers to a range of objects with the same
     *        size.
     */
    template<typename U>
    array_proxy(U* beg, U* end) : begin_(beg), end_(end) {
        must_be_same_size<T, U>();
    }

    /**
     * \brief Construct from pointer and element count of objects with the
     *        same size.
     */
    template<typename U>
    array_proxy(U* beg, const v8_size_t num_elements)
        : begin_(beg), end_(begin_ + num_elements) {
        must_be_same_size<T, U>();
    }

    /**
     * \brief Construct from an existing array of objects with the same size.
     */
    template<typename U, v8_size_t N>
    array_proxy(U (&arr)[N]) : begin_(&arr[0]), end_(&arr[0] + N) {
        must_be_same_size<T, U>();
    }

    /**
     * \brief   Construct from temporary.
     */
    array_proxy(class_type&& rval)
        : begin_(rval.begin_), end_(rval.end_)
    {}

    /**
     * \brief   Construct from temporary that holds objects of the same size.
     */
    template<typename U>
    array_proxy(array_proxy<U>&& rval)
        : begin_(rval.begin()), end_(rval.end()) {
        must_be_same_size<T, U>();
    }

    /**
     * \brief   Assign from temporary.
     */
    class_type& operator=(class_type&& rval) {
        begin_ = rval.begin_;
        end_ = rval.end_;
        return *this;
    }

    /**
     * \brief   Assign from temporary that holds objects of the same size.
     */
    template<typename U>
    class_type& operator=(array_proxy<U>& rval) {
        must_be_same_size<T, U>();

        begin_ = rval.begin();
        end_ = rval.end();
        return *this;
    }

    /** @} */

public :
    /** \name Properties
     * @{
     */

    /**
     * \brief Returns the array's start address.
     */
    pointer base() {
        return begin_;
    }

    /**
     * \brief Returns the array's start address.
     */
    const_pointer base() const {
        return begin_;
    }

    /**
     * \brief Returns the number of elements in the array.
     */
    size_type length() const {
        return end_ - begin_;
    }

    /**
     * \brief Tests if the array is empty.
     */
    v8_bool_t empty() const {
        return length() == 0;
    }

    /** @} */

public :

    /** \name Iteration
     * @{
     */

    /**
     * \brief Returns an iterator to the start of the sequence.
     */
    iterator begin() {
        return begin_;
    }

    /**
     * \brief Returns a const iterator to the start of the sequence.
     */
    const_iterator begin() const {
        return begin_;
    }

    /**
     * \brief Returns an iterator to the end of the sequence.
     */
    iterator end() {
        return end_;
    }

    /**
     * \brief Returns a const iterator to the end of the sequence.
     */
    const_iterator end() const {
        return end_;
    }

    reverse_iterator rbegin() {
        return reverse_iterator(begin());
    }
    
    reverse_iterator rend() {
        return reverse_iterator(end());
    }
    
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(rbegin());
    }
    
    const_reverse_iterator rend() const {
        return const_reverse_iterator(rend());
    }

    /** @} */

    /** \name Subscripting
     * @{
     */

public :

    /**
     * \brief Returns a reference to an element.
     * \param idx Index of the desired element.
     */
    reference operator[](v8_size_t idx) {
        assert((idx < length()) && "Out of bounds index");
        return *(begin_ + idx);
    }

    /**
     * \brief Returns a const reference to an element.
     * \param idx Index of the desired element.
     */
    const_reference operator[](v8_size_t idx) const {
        assert((idx < length()) && "Out of bounds index");
        return *(begin_ + idx);
    }

    /** @ */

    /** \name Data members.
     *  @{
     */
    
private :

    T* const    begin_; //! Pointer to the first element.
    T* const    end_; //! Pointer to one past the end element.

private :
    NO_CC_ASSIGN(array_proxy);

    /** @} */
};

} // namespace base
} // namespace v8
