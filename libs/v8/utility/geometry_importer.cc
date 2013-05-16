#include "v8/base/debug_helpers.hpp"
#include "v8/base/scoped_pointer.hpp"
#include "v8/rendering/vertex_pn.hpp"

#include <third_party/assimp/Importer.hpp>
#include <third_party/assimp/scene.h>
#include <third_party/assimp/postprocess.h>
#include <third_party/stlsoft/platformstl/filesystem/path.hpp>

#include "v8/utility/geometry_importer.hpp"

v8_bool_t v8::utility::import_geometry(
    const char* file_name,
    v8::rendering::vertex_pn** vertices,
    v8_uint32_t** indices,
    v8_uint32_t* num_vertices,
    v8_uint32_t* num_indices
    ) {

    assert(vertices);
    assert(indices);
    assert(num_vertices);
    assert(num_indices);

    Assimp::Importer importer;
    const v8_uint_t k_load_flags = aiProcess_JoinIdenticalVertices 
        | aiProcess_Triangulate | aiProcess_GenSmoothNormals;
    const aiScene* k_scene = importer.ReadFile(file_name, k_load_flags);
    if (!k_scene) {
        OUTPUT_DBG_MSGA("Failed to import scene, error %s", importer.GetErrorString());
        return false;
    }

    *num_vertices = 0;
    *num_indices = 0;

    for (v8_uint_t mesh_index = 0; mesh_index < k_scene->mNumMeshes; ++mesh_index) {
        const aiMesh* k_mesh = k_scene->mMeshes[mesh_index];
        *num_vertices += k_mesh->mNumVertices;
        for (v8_uint_t face_index = 0; face_index < k_mesh->mNumFaces; ++face_index) {
            const aiFace* k_face = &k_mesh->mFaces[face_index];
            *num_indices += k_face->mNumIndices;
        }
    }

    using namespace v8;
    using namespace v8::base;
    using namespace v8::rendering;

    scoped_ptr<vertex_pn, default_array_storage>    ptr_vertices(
        new vertex_pn[*num_vertices]);
    scoped_ptr<v8_uint32_t, default_array_storage>  ptr_indices(
        new v8_uint32_t[*num_indices]);
    v8_uint32_t vertex_count   = 0;
    v8_uint32_t index_count    = 0;
    v8_uint_t indices_offset = 0;

    for (v8_uint_t mesh_index = 0; mesh_index < k_scene->mNumMeshes; ++mesh_index) {
        const aiMesh* k_mesh = k_scene->mMeshes[mesh_index];
        
        for (v8_uint_t vertex_idx = 0; vertex_idx < k_mesh->mNumVertices; ++vertex_idx) {
            const aiVector3D* k_imported_vtx = &k_mesh->mVertices[vertex_idx];
            rendering::vertex_pn new_vtx;
            new_vtx.position.x_ = k_imported_vtx->x;
            new_vtx.position.y_ = k_imported_vtx->y;
            new_vtx.position.z_ = k_imported_vtx->z;
            
            k_imported_vtx = &k_mesh->mNormals[vertex_idx];
            new_vtx.normal.x_ = k_imported_vtx->x;
            new_vtx.normal.y_ = k_imported_vtx->y;
            new_vtx.normal.z_ = k_imported_vtx->z;

            //vertices->push_back(new_vtx);
            ptr_vertices[vertex_count++] = new_vtx;
        }

        for (v8_uint_t face_index = 0; face_index < k_mesh->mNumFaces; ++face_index) {
            const aiFace* k_face = &k_mesh->mFaces[face_index];
            for (v8_uint_t i = 0; i < k_face->mNumIndices; ++i) {
                //indices->push_back(k_face->mIndices[i] + indices_offset);
                ptr_indices[index_count++] = k_face->mIndices[i] + indices_offset;
            }
        }

        indices_offset += k_mesh->mNumVertices;
    }

    *vertices = scoped_pointer_release(ptr_vertices);
    *indices = scoped_pointer_release(ptr_indices);
    return true;
}
