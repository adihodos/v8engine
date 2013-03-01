#pragma once

#include <cassert>
#include <string>

#include <v8/v8.hpp>
#include <v8/math/vector3.hpp>

#pragma warning(push)
#pragma warning(disable : 4100 4127 4201 4701 4702 4611 4512)
#include <third_party/rapidjson/document.h>
#pragma warning(pop)

#include <third_party/stlsoft/platformstl/filesystem/memory_mapped_file.hpp>

namespace v8 { namespace io {

template<typename OutputValueType>
inline void get_json_value(
    const rapidjson::Value& json_value, OutputValueType& output_value
    );

template<>
inline void get_json_value(
    const rapidjson::Value& json_value, v8_int_t& int_value
    ) {
    assert(json_value.IsInt());
    int_value = json_value.GetInt();
}

template<>
inline void get_json_value(
    const rapidjson::Value& json_value, v8_int64_t& int64_value
    ) {
    assert(json_value.IsInt64());
    int64_value = json_value.GetInt64();
}

template<>
inline void get_json_value( 
    const rapidjson::Value& json_value, v8_size_t& size_t_value
    ) {
    assert(json_value.IsUint());
    size_t_value = static_cast<v8_size_t>(json_value.GetUint());
}

template<>
inline void get_json_value( 
    const rapidjson::Value& json_value, v8_uint_t& uint_value
    ) {
    assert(json_value.IsUint());
    uint_value = json_value.GetUint();
}


template<>
inline void get_json_value(
    const rapidjson::Value& json_value, float& float_value
    ) {
    assert(json_value.IsDouble());
    float_value = static_cast<float>(json_value.GetDouble());
}

template<>
inline void get_json_value(
    const rapidjson::Value& json_value, bool& bool_value
    ) {
    assert(json_value.IsBool());
    bool_value = json_value.GetBool();
}

template<>
inline void get_json_value(
    const rapidjson::Value& json_value, std::string& string_value
    ) {
    assert(json_value.IsString());
    string_value.assign(json_value.GetString());
}

template<>
inline void get_json_value(
    const rapidjson::Value& json_value, const char*& string_value
    ) {
    assert(json_value.IsString());
    string_value = json_value.GetString();
}

template<typename OutputValueType>
inline void get_json_value(
    const rapidjson::Value& val_container,
    const char* const k_val_name,
    OutputValueType& output_value
    ) {
    assert(k_val_name);
    get_json_value(val_container[k_val_name], output_value);
}

template<typename OutputValueType>
inline void get_json_array(
    const rapidjson::Value& array_container, 
    const char* const k_array_name,
    const v8_size_t k_output_array_size,
    OutputValueType* output_array
    ) {

    assert(k_array_name);
    assert(output_array);

    assert(array_container.HasMember(k_array_name));
    const rapidjson::Value& k_array = array_container[k_array_name];
    assert(k_array.IsArray());

    const v8_size_t k_num_entries = static_cast<v8_size_t>(k_array.Size());

    for (v8_size_t val_index = 0; 
         (val_index < k_num_entries) && (val_index < k_output_array_size);
         ++val_index) {
        const rapidjson::Value& array_element = 
            k_array[static_cast<rapidjson::SizeType>(val_index)];
        get_json_value(array_element, output_array[val_index]);
    }
}

template<typename OutputValueType, v8_size_t N>
inline void get_json_array(
    const rapidjson::Value& array_container, 
    const char* const k_array_name,
    OutputValueType (&array_ref)[N]
    ) {
    get_json_array(array_container, k_array_name, N, &array_ref[0]);
}

struct RDOnlyAccess {};
struct RDWRAccess   {};

template<typename AccessType>
class V8ConfigFile_Section;

template<>
class V8ConfigFile_Section<RDOnlyAccess> {
public :
    typedef V8ConfigFile_Section<RDOnlyAccess>              class_type;
    typedef const rapidjson::Value*                         section_pointer_type;

    V8ConfigFile_Section(section_pointer_type section_ref)
        : section_(section_ref)
    {}

public :

    v8_bool_t CheckIfValid() const {
        return section_ != nullptr;
    }

    inline class_type GetSection(
        const char** section_hierarchy, 
        const v8_size_t k_num_hier_entries
        ) const;

    inline class_type GetSectionAt(
        v8_size_t index
        ) const;

    template<typename ValueType>
    inline v8_bool_t GetValue(
        const char* section_entry, 
        ValueType& output_value
        ) const;

    template<typename ValueType, v8_size_t ArraySize>
    inline v8_bool_t GetValue(
        const char* section_entry,
        ValueType (&array_ref)[ArraySize]
        ) const;

    template<typename ValueType>
    inline v8_bool_t GetArray(
        const char* section_entry, 
        const v8_size_t num_entries, 
        ValueType* output_values
        ) const;

private :
    section_pointer_type                                             section_;
};

template<typename ValueType>
inline v8_bool_t V8ConfigFile_Section<RDOnlyAccess>::GetValue(
    const char*     section_entry, 
    ValueType&      output_value
    ) const {
    assert(CheckIfValid());
    assert(section_entry);

    if (section_->HasMember(section_entry)) {
        get_json_value(*section_, section_entry, output_value);
        return true;
    }
    return false;
}

template<typename ValueType, v8_size_t ArraySize>
inline v8_bool_t V8ConfigFile_Section<RDOnlyAccess>::GetValue(
    const char* section_entry,
    ValueType (&array_ref)[ArraySize]
    ) const {
    return GetArray(section_entry, ArraySize, array_ref);
}

template<typename ValueType>
inline v8_bool_t V8ConfigFile_Section<RDOnlyAccess>::GetArray(
    const char*         section_entry, 
    const v8_size_t     num_entries, 
    ValueType*          output_values
    ) const {
    assert(CheckIfValid());
    assert(section_entry);
    assert(output_values);

    if (section_->HasMember(section_entry)) {
        get_json_array(*section_, section_entry, num_entries, output_values);
        return true;
    }
    return false;
}

inline
V8ConfigFile_Section<RDOnlyAccess>
V8ConfigFile_Section<RDOnlyAccess>::GetSection(
    const char**        section_hierarchy, 
    const v8_size_t     k_num_hier_entries
    ) const {
    assert(CheckIfValid());
    assert(section_hierarchy);

    section_pointer_type section_entry = section_;
    for (v8_size_t idx = 0; idx < k_num_hier_entries; ++idx) {
        if (!section_entry->HasMember(section_hierarchy[idx])) {
            section_entry = nullptr;
            break;
        }
        section_entry = &(*section_entry)[section_hierarchy[idx]];
    }
    return class_type(section_entry);
}

inline
V8ConfigFile_Section<RDOnlyAccess>
V8ConfigFile_Section<RDOnlyAccess>::GetSectionAt(v8_size_t index) const {
    assert(CheckIfValid());
    if ((section_->IsArray()) && (index < section_->Size())) {
        return class_type(&(*section_)[(rapidjson::SizeType) index]);
    }
    return class_type(nullptr);    
}

typedef V8ConfigFile_Section<RDOnlyAccess>      V8ConfigFile_ReadOnlySection;

class V8ConfigFile {
public :
    V8ConfigFile(const char* const k_file_path);

    v8_bool_t CheckIfValid() const {
        return is_valid_;
    }

    V8ConfigFile_ReadOnlySection GetSection(
        const char**        k_section_hierarchy = nullptr,
        const v8_size_t     k_hier_entries = 0
        ) const;

private :
    v8_bool_t                                                   is_valid_;
    platformstl::memory_mapped_file                             mmfile_;
    rapidjson::Document                                         config_file_;
    
private :
    NO_CC_ASSIGN(V8ConfigFile);
};

} // namespace io
} // namespace v8   
