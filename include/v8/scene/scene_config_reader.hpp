#pragma once

#include <string>
#include <unordered_map>

#include <v8/fast_delegate/fast_delegate.hpp>
#include <v8/v8.hpp>
#include <v8/math/light.hpp>
#include <v8/io/config_file_reader.hpp>
#include <v8/scene/fwd_scene_loading_info.hpp>

namespace v8 { namespace scene {

class section_reader {
public :
    virtual ~section_reader() {}

    virtual void read_section_entry(const io::V8ConfigFile_ReadOnlySection&);
};

class section_reader_lights : public section_reader {
public :
    typedef fastdelegate::FastDelegate2
    <
        const v8::math::light&,
        v8_bool_t
    >                                               light_process_delegate_t;
    
public :

    section_reader_lights(light_process_delegate_t light_proc_del)
        :   light_proc_del_(light_proc_del)
    {}    

    void read_section_entry(const io::V8ConfigFile_ReadOnlySection&);

private :
    light_process_delegate_t                        light_proc_del_;
};

class seaction_reader_standard_entity : public section_reader {
public :
    typedef fastdelegate::FastDelegate1
    <
        const standard_entity_load_info_t&
    >                                               std_entity_delegate_t;

public :

    seaction_reader_standard_entity(std_entity_delegate_t ent_del)
        :   std_ent_del_(ent_del)
    {}

    void read_section_entry(const io::V8ConfigFile_ReadOnlySection&);

private :
    std_entity_delegate_t                           std_ent_del_;
};

class section_reader_std_object : public section_reader {
public :
    typedef fastdelegate::FastDelegate1
    <
        const standard_object_loading_info_t&
    >                                               std_object_delegate_t;

public :

    section_reader_std_object(std_object_delegate_t obj_del)
        :   std_obj_del_(obj_del)
    {}

    void read_section_entry(const io::V8ConfigFile_ReadOnlySection&);

private :
    std_object_delegate_t                           std_obj_del_;
};

class section_reader_userdefined_object : public section_reader {
public :
    typedef fastdelegate::FastDelegate1
    <
        const userdefined_object_loading_info_t&
    >                                               user_object_delegate_t;

public :

    section_reader_userdefined_object(user_object_delegate_t obj_del)
        :   user_obj_del_(obj_del)
    {}

    void read_section_entry(const io::V8ConfigFile_ReadOnlySection&);

private :
    user_object_delegate_t                           user_obj_del_;
};

class scene_config_reader {
public :
/// \name Constructors.
/// @{

public :    

    scene_config_reader();

/// @}    

/// \name Operations
/// @{

public :    

    v8_bool_t read_config_file(const char* full_path);

    void add_section_reader(const std::string& sec_name, section_reader* sec_rd) {
        sec_readers_.insert(std::make_pair(sec_name, sec_rd));
    }

/// @}    

/// \name Data members
/// @{

private :    
    std::unordered_map<std::string, section_reader*>    sec_readers_;

/// @}    
};

} // namespace scene
} // namespace v8
