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

#include <v8/v8.hpp>
#include <v8/rendering/constants.hpp>

namespace v8 { namespace rendering {

struct textureType_t {
    enum {
        Tex1D,
        Tex2D,
        Tex3D,
        TexCube
    };
};

struct textureDescriptor_t {
    v8_uint32_t     texWidth;
    v8_uint32_t     texHeight;
    v8_int32_t      texType;
    v8_uint32_t     arraySize;
    v8_int32_t      elementsPerTexel;
    v8_int32_t      elementType;
    v8_uint32_t     pipelineStageBindFlags;
    v8_uint32_t     usageFlags;
    v8_uint32_t     cpuFlags;
    v8_bool_t       generateMips;

    textureDescriptor_t() 
        :       texWidth(1024)
            ,   texHeight(1024)
            ,   texType(textureType_t::Tex2D)
            ,   arraySize(1)
            ,   elementsPerTexel(4)
            ,   elementType(ElementType::Unorm8)
            ,   pipelineStageBindFlags(BindingFlag::ShaderResource)
            ,   usageFlags(ResourceUsage::Default)
            ,   cpuFlags(CPUAccess::None)
            ,   generateMips(false)
    {}

    textureDescriptor_t(const v8_uint32_t       width,
                        const v8_uint32_t       height,
                        const v8_int32_t        type,
                        const v8_uint32_t       array_size,
                        const v8_int32_t        elements_per_texel,
                        const v8_int32_t        element_type,
                        const v8_uint32_t       bind_stage,
                        const v8_uint32_t       usage,
                        const v8_uint32_t       cpu_access,
                        const v8_bool_t         gen_mips
                        )
        :       texWidth(width)
            ,   texHeight(height)
            ,   texType(type)
            ,   arraySize(array_size)
            ,   elementsPerTexel(elements_per_texel)
            ,   elementType(element_type)
            ,   pipelineStageBindFlags(bind_stage)
            ,   usageFlags(usage)
            ,   cpuFlags(cpu_access)
            ,   generateMips(gen_mips)
    {}
};

struct textureInitData_t {
    const void*         dataPointer;
    v8_uint32_t         rowPitch;
    v8_uint32_t         depthPitch;
};

} // namespace rendering
} // namespace v8
