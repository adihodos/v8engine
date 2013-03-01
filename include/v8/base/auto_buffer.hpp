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

#include <cassert>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <utility>

#include <v8/v8.hpp>
#include <v8/meta_programming/constraints.hpp>

namespace v8 { namespace base {

//! \addtogroup v8_base_lib
//! @{

//! 
//! \brief A POD buffer class. Largely based on the auto_buffer class,
//! described in <b>Imperfect C++</b>. When possible, the class will use 
//! an internal statically allocated buffer (the dimensions can be specified 
//! at compile time by the Size template parameter) to store the elements. 
//! If, however, the user  needs to store more elements, it will allocate and 
//! use a dynamic buffer.
//! \param T Type of elements stored in the buffer. <b> Must be a POD type. </b>
//! \param Size Initial buffer size.
//! \param Allocator The allocator is used to obtain memory to store the elements,
//! when the static storage buffer is too small.
template
<
     typename T,
     v8_size_t Size = 512U,
     typename Allocator = std::allocator<T>
>
class auto_buffer : protected Allocator {

public :

    //! \name   Typedefs
    //! @{

    //! Type of elements stored in the buffer.
    typedef T                                           value_type;

    //! Type of reference to an element.
    typedef T&                                          reference;

    //! Type of reference to a const element.
    typedef T const&                                    const_reference;

    //! Full type of this class.
    typedef auto_buffer<T, Size, Allocator>             class_type;

    //! Type of an iterator.
    typedef T*                                          iterator;

    //! Type of a const iterator.
    typedef T const*                                    const_iterator;

    //! Type of a reverse iterator.
    typedef std::reverse_iterator<iterator>             reverse_iterator;

    //! Type of a const reverse iterator.
    typedef std::reverse_iterator<const_iterator>       const_reverse_iterator;

    //! @}

public :

    //! \name Constructors.
    //! @{

    //! Default constructor. Will set the number of items equal to the Size
    //! template parameter.
    auto_buffer() 
        : store_ptr_(&static_store_[0])
          , items_(Size) {}

    //! Construct with enough space to store the specified number of items.
    //! \param[in] items Number of items to reserve space for.
    //! \note If items > Size, then a buffer will be allocated, otherwise
    //! the internal static buffer is used to store the elements.
    auto_buffer(v8_size_t items)
        : store_ptr_(items > Size ? alloc_no_copy(items) : &static_store_[0])
          , items_(items) {
    }

    //! Construct with enough space to store the specified number of items and
    //! initialize all elements to the specified value.
    //! \param[in] items Number of items to reserve space for.
    //! \param[in] initial_value Initial value for the elements.
    auto_buffer(v8_size_t items, T const& initial_value)
        : store_ptr_(items > Size ? alloc_no_copy(items) : &static_store_[0])
          , items_(items) {
        std::fill_n(store_ptr_, items, initial_value);
    }

    //! Copy construct from an existing object of this class.
    auto_buffer(const class_type& rhs)
        : Allocator(rhs)
          , store_ptr_(rhs.size() > Size ?
                           alloc_no_copy(rhs.size()) : &static_store_[0])
          , items_(rhs.size()) {
        memcpy(store_ptr_, rhs.store_ptr_, rhs.size() * sizeof(T));
    }

    ~auto_buffer() {
        constraint_must_be_pod<T>();

        if (!has_static_storage()) {
            this->deallocate(store_ptr_, size());
        }
    }

    //! @}

public :

    //! \name Assignment operators.
    //! @{

    class_type& operator=(const class_type& rhs) {
        class_type tmp_autobuff(rhs);
        this->swap(tmp_autobuff);
        return *this;
    }

    //! @}

public :

    //! \name Operations
    //! @{

    //! Exchange contents with another object of this class.
    void swap(class_type& rhs);

    //! Resizes the buffer.
    //! \param[in] new_size Number of items that the buffer will have.
    //! \note The semantic of resize is as follows :
    //! \par
    //!     if the buffer is using the static store
    //!         if new_size > Size
    //!             allocate a dynamic buffer and move existing elements
    //!         endif
    //! 
    //!         set the number of elements to new_size
    //!     else
    //!         if new_size <= Size
    //!             move elements from dynamic buffer to static buffer
    //!             release dynamic buffer
    //!         elif new_size <= size()
    //!             set the number of elements to new_size
    //!         else
    //!             allocate a new buffer and move elements from old buffer
    //!             release old buffer
    //!         endif
    //!         
    //!         set the number of elements to new_size
    //!     endif
    void resize(v8_size_t new_size);

    //! @}

public :

    //! \name Element access
    //! @{

    //! Returns a reference to an element.
    //! \param[in] index Index of requested element.
    reference operator[](v8_size_t index) {
        assert(index < size());
        return store_ptr_[index];
    }

    //! Returns a const reference to an element.
    //! \param[in] index Index of requested element.
    const_reference operator[](v8_size_t index) const {
        assert(index < size());
        return store_ptr_[index];
    }

    //!
    //! Returns a pointer to the beginning of the storage area.
    T* get_buffer() {
        return store_ptr_;
    }

    //!
    //! Returns a pointer to the beginning of the storage area (const).
    const T* get_buffer() const {
        return store_ptr_;
    }

    //! @}

public :

    //! \name Iteration.
    //! @{

    iterator begin() NOEXCEPT {
        return store_ptr_;
    }

    iterator end() NOEXCEPT {
        return store_ptr_ + size();
    }

    const_iterator begin() const NOEXCEPT {
        return store_ptr_;
    }

    const_iterator end() const NOEXCEPT {
        return store_ptr_ + size();
    }

    reverse_iterator rbegin() NOEXCEPT {
        return reverse_iterator(begin());
    }

    reverse_iterator rend() NOEXCEPT {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const NOEXCEPT {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const NOEXCEPT {
        return reverse_iterator(end());
    }

    //! @}

public :

    //! \name   Attributes
    //! @{

    v8_size_t size() const NOEXCEPT {
        return items_;
    }

private :

    v8_size_t capacity() const NOEXCEPT {
        return std::max(size(), Size);
    }

public :

    //! Returns true if using the static buffer or a dynamically allocated one.
    v8_bool_t has_static_storage() const NOEXCEPT {
        return store_ptr_ == &static_store_[0];
    }

    //! @}

private :

    //! \name Utility functions.
    //! @{

    T* allocate_and_copy(v8_size_t num_items) {
        T* new_buffer = this->allocate(num_items);

        if (!new_buffer)
            throw std::bad_alloc();

        memcpy(new_buffer, store_ptr_, size() * sizeof(T));

        return new_buffer;
    }

    T* alloc_no_copy(v8_size_t num_items) {
        T* new_buffer = this->allocate(num_items);

        if (!new_buffer)
            throw std::bad_alloc();

        return new_buffer;
    }

    T* alloc_fill(v8_size_t num_items, T const& initial_value) {
        T* new_buffer = this->allocate(num_items);

        if (!new_buffer)
            throw std::bad_alloc();

        std::fill_n(new_buffer, num_items, initial_value);
        return new_buffer;
    }

    void reset_state() {
        if (!has_static_storage()) {
            this->deallocate(store_ptr_, size());
            store_ptr_ = &static_store_[0];
        }
        items_ = Size;
    }

    //! @}

private :

    //! Static storage for the elements. Used whenever items_ <= Size.
    T           static_store_[Size];
    
    //! Pointer to the storage area of elements. 
    //! When using static storage store_ptr_ = &static_store_[0].
    T*          store_ptr_;
    
    //! Number of items stored. If using the static store items_ <= Size.
    v8_size_t   items_;
};

template
<
        typename T,
        v8_size_t Size,
        typename Allocator
>
void auto_buffer<T, Size, Allocator>::resize(v8_size_t new_size) {
    if (has_static_storage()) {
        //
        // Check if capacity is enough to hold new_size items.
        if (new_size > Size) {
            //
            // Allocate and copy old elements to new storage.
            store_ptr_ = allocate_and_copy(new_size);
        }

        //
        // Update item count
        items_ = new_size;
    } else {
        //
        // If the number of new items fits in the static buffer, then
        // we use that and release the allocated one.
        if (new_size <= Size) {

            memcpy(&static_store_[0], store_ptr_, new_size * sizeof(T));
            this->deallocate(store_ptr_, size());
            store_ptr_ = &static_store_[0];

        } else if (new_size <= size()) {
            //
            // Too many items too fit into the static buffer, so use the dynamic
            // buffer and just update element count.
            // Handled below, so the block is empty.
        } else {
            //
            // Dynamic buffer won't fit, need to reallocate.
            T* new_store_ptr = allocate_and_copy(new_size);
            this->deallocate(store_ptr_, size());
            store_ptr_ = new_store_ptr;
        }

        items_ = new_size;
    }
}

template
<
        typename T,
        v8_size_t Size,
        typename Allocator
>
void auto_buffer<T, Size, Allocator>::swap(
    auto_buffer<T, Size, Allocator>& rhs
    ) {
    //
    // If both buffers are dynamically allocated, simply swap all components.
    if (!has_static_storage() && !rhs.has_static_storage()) {
        std::swap(store_ptr_, rhs.store_ptr_);
        std::swap(items_, rhs.items_);
        return;
    }

    if (has_static_storage() && !rhs.has_static_storage()) {
        //
        // We can avoid an extra copy, since the static storage of rhs
        // can accomodate all of lhs elements.
        memcpy(&rhs.static_store_[0], store_ptr_, size() * sizeof(T));
        //
        // We take ownership of rhs's dynamic buffer.
        store_ptr_ = rhs.store_ptr_;
        //
        // rhs will use the static buffer now.
        rhs.store_ptr_ = &rhs.static_store_[0];
        std::swap(items_, rhs.items_);

    } else if (!has_static_storage() && rhs.has_static_storage()) {
        //
        // Same as above, so just call swap from the perspective of rhs,
        // to keep the code simple.
        rhs.swap(*this);
    } else {
        assert(capacity() >= rhs.size());
        assert(rhs.capacity() >= size());

        //
        // Both objects use the static buffer, so we need to copy via
        // temporary buffer.
        T tmp_buff[Size];
        memcpy(tmp_buff, store_ptr_, size() * sizeof(T));
        memcpy(store_ptr_, rhs.store_ptr_, rhs.size() * sizeof(T));
        memcpy(rhs.store_ptr_, tmp_buff, size() * sizeof(T));
        std::swap(items_, rhs.items_);
    }
}

//! @}

} // namespace base
} // namespace v8
