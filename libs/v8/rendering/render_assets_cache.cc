#include <unordered_map>
#include "v8/base/associative_container_veneer.hpp"
#include "v8/base/scoped_pointer.hpp"
#include "v8/rendering/effect.hpp"
#include "v8/rendering/effect_info.hpp"
#include "v8/rendering/renderer.hpp"
// #include "v8/rendering/simple_mesh.hpp"
#include "v8/rendering/texture.hpp"
#include "v8/utility/hash_spooky.hpp"
#include "v8/utility/hash_string.hpp"

#include "v8/rendering/render_assets_cache.hpp"

//
// Resource initializer shims

//inline v8_bool_t resource_initialize(
//    const v8::rendering::effect_info_t& eff_info,
//    v8::rendering::renderer* render_sys,
//    v8::rendering::effect* eff_ptr
//    ) {
//    return eff_ptr->initialize(eff_info, render_sys);
//}

inline v8_bool_t resource_initialize(
    const v8::rendering::texture_info_t& tex_info,
    v8::rendering::renderer* render_sys,
    v8::rendering::texture* tex_ptr
    ) {
    return tex_ptr->initialize(tex_info, render_sys);
}

// inline v8_bool_t resource_initialize(
//     const v8::hash_string& mesh_name,
//     v8::rendering::renderer* render_sys,
//     v8::rendering::simple_mesh* simple_mesh
//     ) {
//     return simple_mesh->initialize(mesh_name, render_sys);
// }

template
<
    typename key_type, 
    typename resource_type, 
    typename descriptor_type = key_type
>
struct resource_pool {
    typedef std::unordered_map
    <
        key_type, 
        resource_type*
    >                                                       pool_storage_t;

    typedef typename pool_storage_t::value_type             pool_entry_t;

    struct pool_entry_destructor {
        void operator()(pool_entry_t& pool_item) const {
            delete pool_item.second;
        }
    };

    typedef v8::base::associative_container_veneer
    <
        pool_storage_t, 
        pool_entry_destructor
    >                                                   managed_pool_storage_t;

    managed_pool_storage_t                              res_pool_;

    resource_type* get_resource(
        const key_type& key, 
        const descriptor_type& res_descriptor,
        v8::rendering::renderer* render_context
        );
};

template<typename KT, typename RT, typename DT>
RT* resource_pool<KT, RT, DT>::get_resource(
    const KT& key, 
    const DT& res_descriptor, 
    v8::rendering::renderer* render_context
    ) {
    auto itr_respool = res_pool_.find(key);
    if (itr_respool != std::end(res_pool_)) {
        return itr_respool->second;
    }

    v8::base::scoped_ptr<RT> new_res(new RT());
    if (!resource_initialize(res_descriptor, render_context, 
                             v8::base::scoped_pointer_get(new_res))) {
        return nullptr;
    }

    res_pool_.insert(std::make_pair(key, v8::base::scoped_pointer_get(new_res)));
    return v8::base::scoped_pointer_release(new_res);
}

v8_size_t compute_effect_unique_id(const v8::rendering::effect_info_t& eff_info) {
    //
    // The horror, the horror.
    // TODO : find a better way to to differentiate between effects.
    v8_uint32_t member_hashes[4] = {
        v8::utility::SpookyHash::Hash32(eff_info.eff_filepath.c_str(), 
                                        eff_info.eff_filepath.length(), 
                                        0),
        v8::utility::SpookyHash::Hash32(eff_info.eff_compile_macros.c_str(),
                                        eff_info.eff_compile_macros.length(),
                                        0),
        eff_info.eff_compile_flags,
        eff_info.eff_target
    };
    return v8::utility::SpookyHash::Hash64(member_hashes, sizeof(member_hashes), 0);
}

struct v8::rendering::render_assets_cache::implementation_details {
    rendering::renderer*                                        render_sys_;

    ///< Pool of compiled effects.
    /*resource_pool
    <
        v8_size_t, 
        v8::rendering::effect,
        v8::rendering::effect_info_t
    >                                                           effects_pool_;*/

    resource_pool
    <
        v8_size_t,
        v8::rendering::texture,
        v8::rendering::texture_info_t
    >                                                           textures_pool_;

    // resource_pool
    // <
    //     v8::hash_string,
    //     v8::rendering::simple_mesh
    // >                                                           mesh_pool_;
};

v8::rendering::render_assets_cache::render_assets_cache(renderer* rsys)
    : pimpl_(new implementation_details())
{
    pimpl_->render_sys_ = rsys;
}

v8::rendering::render_assets_cache::~render_assets_cache() {}

//v8::rendering::effect* v8::rendering::render_assets_cache::get_effect(
//    const effect_info_t& eff_data
//    ) {
//    const v8_size_t effect_id = compute_effect_unique_id(eff_data);
//    return pimpl_->effects_pool_.get_resource(effect_id, eff_data, 
//                                              pimpl_->render_sys_);
//}

v8::rendering::texture* v8::rendering::render_assets_cache::get_texture(
    const v8::rendering::texture_info_t& tex_info
    ) {
    const v8_uint32_t member_hashes[] = {
        v8::utility::SpookyHash::Hash32(tex_info.tex_filename.c_str(), 
                                        tex_info.tex_filename.length(), 
                                        0),
        v8::utility::SpookyHash::Hash32(&tex_info.tex_bindflags, 
                                        sizeof(tex_info.tex_bindflags),
                                        0)
    };

    const v8_size_t tex_unique_id = v8::utility::SpookyHash::Hash64(
        member_hashes, sizeof(member_hashes), 0
        );
    return pimpl_->textures_pool_.get_resource(tex_unique_id, 
                                               tex_info,
                                               pimpl_->render_sys_);
}

// v8::rendering::simple_mesh* v8::rendering::render_assets_cache::get_mesh(
//     const v8::hash_string& mesh_name
//     ) {
//     return pimpl_->mesh_pool_.get_resource(mesh_name, 
//                                            mesh_name, 
//                                            pimpl_->render_sys_);
// }
