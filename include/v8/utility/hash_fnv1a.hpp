#pragma once

#include <string>
#include <v8/v8.hpp>

namespace v8 { namespace utility {

namespace detail {

///
/// \brief Compile time evaluation of FNV1A hash for an array of bytes.
template<v8_size_t N, v8_size_t P>
struct fnv1a_impl {
    static  v8_uint_t hash(const v8_byte_t (&arr)[N]) {
       return (fnv1a_impl<N, P - 1>::hash(arr) ^ arr[P - 1]) * 16777619U;
    }
};

template<v8_size_t N>
struct fnv1a_impl<N, 1U> {
    static v8_uint_t hash(const v8_byte_t (&arr)[N]) {
        return (2166136261U ^ arr[0]) * 16777619U;
    }
};

} // namespace detail

///
/// \brief Implements FNV1A hash.
/// \remarks The FNV1A hashing algorithm is described here :
/// http://isthe.com/chongo/tech/comp/fnv/ .
struct FNV1AHash_t {

/// \name Hashing.
/// @{

    ///
    /// \brief Computes the FNV1A hash, for a stream of bytes.
    template<typename T>
    static v8_uint_t hash(const T* data, v8_size_t data_len) {
        v8_uint_t hash_val = 2166136261U;
        
        const v8_size_t num_bytes = data_len * sizeof(T);
        const v8_byte_t* data_ptr = reinterpret_cast<const v8_byte_t*>(data);
        size_t i = 0;

        while (i < num_bytes) {

            hash_val ^= *data_ptr++;
            hash_val *= 16777619U;
            ++i;
        }
        
        return hash_val;
    }
    
    ///
    /// \brief Computes the FNV1A hash, at compile time, for an array of data.
    /// \remarks This relies on the assumption that the compiler will be able
    /// to evaluate the template at compile time.
    template<typename T, size_t N>
    static v8_uint_t compile_time_hash(const T (&arr)[N]) {
        return detail::fnv1a_impl<
            N * sizeof(T), 
            N * sizeof(T)
        >::hash(reinterpret_cast<const v8_byte_t (&)[N * sizeof(T)]>(arr));
    }

    static v8_uint_t hash_string(const char* str) {
        return FNV1AHash_t::hash(str, strlen(str) + 1);
    }

    static v8_uint_t hash_string(const std::string& str) {
        return FNV1AHash_t::hash(str.c_str(), str.length() + 1);
    }

/// @}
};


} // namespace utility
} // namespace v8
