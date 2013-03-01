#pragma once

#include <v8/v8.hpp>
#include <v8/math/vector3.hpp>

#include <string>
#include <vector>

#include <v8/rendering/vertex_pn.hpp>

namespace v8 { namespace utility {

class ifs_loader {
public :
    float                                           ifsVersion_;
    std::string                                     modelName_;
    std::vector<rendering::vertex_pn>               vertexData_;
    std::vector<v8_int32_t>                         indexData_;
    v8_uint_t                                       numFaces_;
    v8_bool_t                                       isValid_;
    v8_bool_t                                       invert_z_;

private :

    v8_bool_t readStringData(HANDLE fptr, std::vector<v8_uint8_t>* str);

    v8_bool_t readFloat(HANDLE fptr, float* outVal);

    v8_bool_t readUint(HANDLE fptr, v8_uint_t* outVal);

    v8_bool_t readElementCount(
        HANDLE fptr, v8_uint_t* vertexCnt, const char* hdrString
        );

    v8_bool_t readVertices(HANDLE fptr, v8_uint_t howMany);

    v8_bool_t readFaces(HANDLE fptr, v8_uint_t howMany);

    v8_bool_t readIFSHeader(HANDLE fptr);

    void compute_mesh_normals();

public :
    ifs_loader() : isValid_(false), invert_z_(false) {}

    v8_bool_t loadModel(const char* modelFile, v8_bool_t invert_z = false);

    v8_size_t getFaceCount() const {
        assert(isValid_);
        return numFaces_;
    }
};

} // namespace utility
} // namespace v8_framework
