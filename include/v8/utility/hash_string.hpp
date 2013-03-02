#pragma once

#include <string>
#include <utility>

#include <v8/v8.hpp>
#include <v8/utility/hash_fnv1a.hpp>

namespace v8 {

class hash_string {
public :
    explicit hash_string(const char* const k_str)
        :       str_(k_str)
            ,   hash_code_(utility::FNV1AHash_t::hash_string(k_str)) {
    }

    template<v8_size_t k_char_cnt>
    explicit hash_string(const char (&an_array)[k_char_cnt])
        :   str_(an_array)
            , hash_code_(utility::FNV1AHash_t::compile_time_hash(an_array))
    {}

    v8_uint32_t get_hash_code() const {
        return hash_code_;
    }

    const std::string& std_str() const {
        return str_;
    }

    const char* c_str() const {
        return str_.c_str();
    }

    template<v8_size_t k_char_cnt>
    hash_string& operator=(const char (&an_array)[k_char_cnt]) {
        str_ = an_array;
        hash_code_ = utility::FNV1AHash_t::compile_time_hash(an_array);
        return *this;
    }

private :
    std::string     str_;
    v8_uint32_t     hash_code_;
};

inline 
bool 
operator==(const hash_string& lhs, const hash_string& rhs)  {
    return lhs.get_hash_code() == rhs.get_hash_code();
}

inline 
bool 
operator!=(const hash_string& lhs, const hash_string& rhs)  {
    return !(lhs == rhs);
}

} // namespace v8

namespace std {

template<>
struct hash<v8::hash_string> {
    size_t operator()(const v8::hash_string& hstr) const {
        return hstr.get_hash_code();
    }
};

} // namespace std
