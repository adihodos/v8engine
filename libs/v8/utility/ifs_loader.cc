#include <Windows.h>
#include <v8/base/scoped_handle.hpp>
#include <v8/base/win32_handle_policies.hpp>

#include "v8/utility/ifs_loader.hpp"

v8_bool_t
v8::utility::ifs_loader::readStringData(
    HANDLE fptr, 
    std::vector<v8_uint8_t>* str
    ) {
    unsigned int strLen;
    DWORD bytesOut;
    if (!::ReadFile(fptr, &strLen, sizeof(strLen), &bytesOut, 0)) {
        return false;
    }

    str->resize(strLen);
    return ::ReadFile(fptr, &(*str)[0], strLen, &bytesOut, 0);
}

v8_bool_t
v8::utility::ifs_loader::readFloat(
    HANDLE fptr, 
    float* outVal
    ) {
    DWORD bytesRead;
    return ::ReadFile(fptr, outVal, sizeof(float), &bytesRead, 0) == TRUE;
}

v8_bool_t
v8::utility::ifs_loader::readUint(
    HANDLE fptr, 
    v8_uint_t* outVal
    ) {
    DWORD bytesRead;
    return ::ReadFile(fptr, outVal, sizeof(*outVal), &bytesRead, 0);
}

v8_bool_t
v8::utility::ifs_loader::readElementCount(
    HANDLE fptr, 
    v8_uint_t* vertexCnt, 
    const char* hdrString
    ) {
    std::vector<v8_uint8_t> headerStr;
    if (!readStringData(fptr, &headerStr))
        return false;

    std::string vertexHDRStr((const char*)&headerStr[0]);
    if (vertexHDRStr.compare(hdrString))
        return false;

    return readUint(fptr, vertexCnt);
}

v8_bool_t
v8::utility::ifs_loader::readVertices(
    HANDLE fptr, 
    v8_uint_t howMany
    ) {
    vertexData_.reserve(howMany);
    v8_bool_t failed = true;
    for (unsigned int i = 0; (i < howMany) && failed; ++i) {
        rendering::vertex_pn currentVertex;
        failed = readFloat(fptr, &currentVertex.position.x_);
        failed = readFloat(fptr, &currentVertex.position.y_);
        failed = readFloat(fptr, &currentVertex.position.z_);

        if (invert_z_) {
            currentVertex.position.z_ *= -1.0f;
        }
        currentVertex.normal = v8::math::vector3F::zero;
        vertexData_.push_back(currentVertex);
    }
    return failed;
}

v8_bool_t
v8::utility::ifs_loader::readFaces(
    HANDLE fptr, 
    v8_uint_t howMany
    ) {
    indexData_.reserve(howMany);
    v8_bool_t succeeded = true;
    v8_uint_t currentIndex;
    for (v8_uint_t i = 0; (i < howMany) && succeeded; ++i) {
        succeeded = readUint(fptr, &currentIndex);
        indexData_.push_back(currentIndex);
        succeeded = readUint(fptr, &currentIndex);
        indexData_.push_back(currentIndex);
        succeeded = readUint(fptr, &currentIndex);
        indexData_.push_back(currentIndex);
    }

    return succeeded;
}

v8_bool_t
v8::utility::ifs_loader::readIFSHeader(
    HANDLE fptr
    ) {
    std::vector<v8_uint8_t> strBuff;
    if (!readStringData(fptr, &strBuff))
        return false;

    std::string readStr((const char*) &strBuff[0]);
    if (readStr.compare("IFS"))
        return false;

    if (!readFloat(fptr, &ifsVersion_))
        return false;

    if (!readStringData(fptr, &strBuff))
        return false;

    modelName_.assign((const char*) &strBuff[0]);
    return true;
}

void v8::utility::ifs_loader::compute_mesh_normals() {
    using rendering::vertex_pn;

    for (v8_uint_t i = 0; i < numFaces_; ++i) {
        vertex_pn& v0 = vertexData_[indexData_[i * 3 + 0]];
        vertex_pn& v1 = vertexData_[indexData_[i * 3 + 1]];
        vertex_pn& v2 = vertexData_[indexData_[i * 3 + 2]];

        const auto e0 = v1.position - v0.position;
        const auto e1 = v2.position - v0.position;

        const auto normal = v8::math::cross_product(e0, e1);

        v0.normal += normal;
        v1.normal += normal;
        v2.normal += normal;
    }

    for (v8_size_t i = 0; i < vertexData_.size(); ++i) {
        vertexData_[i].normal.normalize();
    }
}

namespace {
    struct win32_file_deleter_t {
        void operator()(HANDLE fp) const {
            if (fp != INVALID_HANDLE_VALUE)
                ::CloseHandle(fp);
        }
    };
}

v8_bool_t
v8::utility::ifs_loader::loadModel(
    const char* modelFile,
    v8_bool_t invert_z
    ) {
    isValid_    = false;
    invert_z_   = invert_z;

    std::unique_ptr<void, win32_file_deleter_t> fileFD(
        ::CreateFileA(modelFile, GENERIC_READ, 0, 0, OPEN_EXISTING, 
                      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 0));

    if (fileFD.get() == INVALID_HANDLE_VALUE)
        return false;

    if (!readIFSHeader(fileFD.get()))
        return false;

    //
    // read vertex count
    v8_uint_t elements;
    if (!readElementCount(fileFD.get(), &elements, "VERTICES"))
        return false;

    if (!readVertices(fileFD.get(), elements))
        return false;

    //
    // read face count
    if (!readElementCount(fileFD.get(), &numFaces_, "TRIANGLES"))
        return false;

    if (!readFaces(fileFD.get(), numFaces_))
        return false;


    compute_mesh_normals();
    isValid_ = true;
    return true;
}
