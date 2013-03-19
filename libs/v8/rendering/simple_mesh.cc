#include <vector>
#include <utility>

#include <third_party/assimp/Importer.hpp>
#include <third_party/assimp/scene.h>
#include <third_party/assimp/postprocess.h>
#include <third_party/stlsoft/platformstl/filesystem/path.hpp>

#include "v8/v8.hpp"
#include "v8/base/debug_helpers.hpp"
#include "v8/base/scoped_pointer.hpp"
#include "v8/global_state.hpp"
#include "v8/io/filesystem.hpp"
#include "v8/rendering/renderer.hpp"
#include "v8/rendering/vertex_pn.hpp"
#include "v8/rendering/vertex_pnt.hpp"
#include "v8/utility/ifs_loader.hpp"

#include "v8/rendering/simple_mesh.hpp"

namespace {

///
/// \brief Loads a mesh from an IFS file. The vertex format consists
/// of position and normal vector.
v8_bool_t load_ifs_mesh(
    const char* const k_mesh_file_path,
    std::vector<v8::rendering::vertex_pn>* vertices,
    std::vector<v8_int32_t>* indices
    ) {

    v8::utility::ifs_loader loader;
    if (!loader.loadModel(k_mesh_file_path, false)) {
        return false;
    }

    std::swap(*vertices, loader.vertexData_);
    std::swap(*indices, loader.indexData_);
    return true;
}

v8_bool_t load_standard_mesh(
    const char* const k_mesh_file_path,
    std::vector<v8::rendering::vertex_pn>* vertices,
    std::vector<v8_int32_t>* indices
    ) {
    Assimp::Importer importer;
    const v8_uint_t k_load_flags = aiProcess_JoinIdenticalVertices 
        | aiProcess_MakeLeftHanded | aiProcess_Triangulate 
        | aiProcess_GenSmoothNormals | aiProcess_FlipWindingOrder;
    const aiScene* k_scene = importer.ReadFile(k_mesh_file_path, k_load_flags);
    if (!k_scene) {
        OUTPUT_DBG_MSGA("Failed to import scene, error %s", importer.GetErrorString());
        return false;
    }

    v8_size_t num_indices  = 0;
    v8_size_t num_vertices = 0;

    for (v8_uint_t mesh_index = 0; mesh_index < k_scene->mNumMeshes; ++mesh_index) {
        const aiMesh* k_mesh = k_scene->mMeshes[mesh_index];
        num_vertices += k_mesh->mNumVertices;
        for (v8_uint_t face_index = 0; face_index < k_mesh->mNumFaces; ++face_index) {
            const aiFace* k_face = &k_mesh->mFaces[face_index];
            num_indices += k_face->mNumIndices;
        }
    }
    vertices->reserve(num_vertices);
    indices->reserve(num_indices);

    using namespace v8;
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

            vertices->push_back(new_vtx);
        }
        for (v8_uint_t face_index = 0; face_index < k_mesh->mNumFaces; ++face_index) {
            const aiFace* k_face = &k_mesh->mFaces[face_index];
            for (v8_uint_t i = 0; i < k_face->mNumIndices; ++i) {
                indices->push_back(k_face->mIndices[i] + indices_offset);
            }
        }
        indices_offset += k_mesh->mNumVertices;
    }
    return true;
}

} // anonymous namespace

v8::rendering::simple_mesh::simple_mesh()
    :       vertex_buffer_()
        ,   index_buffer_()
        ,   topology_(PrimitiveTopology::Undefined)
        ,   valid_(false)
{}

v8::rendering::simple_mesh::simple_mesh(
    const v8::hash_string& mesh_name, v8::rendering::renderer* render_sys
    )
    :       vertex_buffer_()
        ,   index_buffer_()
        ,   topology_(PrimitiveTopology::Undefined)
{
    valid_ = initialize(mesh_name, render_sys);
}

v8::rendering::simple_mesh::~simple_mesh() {}

v8_bool_t v8::rendering::simple_mesh::initialize(
    const hash_string& mesh_name, renderer* render_sys
    ) {
    if (is_valid()) {
        return true;
    }
    
    platformstl::path mesh_file_path(state->file_sys()->get_dir_path(
        filesys::Dir::Models).c_str());
    mesh_file_path.push(mesh_name.c_str());

    std::vector<rendering::vertex_pn>   vertices;
    std::vector<v8_int32_t>             indices;
    v8_bool_t                           ret_code = false;

    if (!strcmp("ifs", mesh_file_path.get_ext())) {
        ret_code = load_ifs_mesh(mesh_file_path.c_str(), &vertices, &indices);
    } else {
        ret_code = load_standard_mesh(mesh_file_path.c_str(), &vertices, &indices); 
    }
    if (!ret_code) {
        return false;
    }

    topology_ = PrimitiveTopology::TriangleList;
    index_buffer_.initialize(
        render_sys, indices.size(), sizeof(indices[0]), &indices[0]
        );
    if (!index_buffer_) {
        return false;
    }

    vertex_buffer_.initialize(
        render_sys, vertices.size(), sizeof(vertices[0]), &vertices[0]
        );
    if (!vertex_buffer_) {
        return false;
    }
    valid_ = true;
    return true;
}