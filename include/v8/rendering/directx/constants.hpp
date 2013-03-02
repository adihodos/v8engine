#pragma once

namespace v8 { namespace directx {

struct PrimitiveTopology { 
    enum Type {
        Undefined,
        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,
        LineList_Adjacency,
        LineStrip_Adjacency,
        TriangleList_Adjacency,
        TriangleStrip_Adjacency
    };
};    

struct Filter {
    enum Type {
        Min_Mag_Mip_Point,
        Min_Mag_Point_Mip_Linear,
        Min_Point_Mag_Linear_Mip_Point,
        Min_Point_Mag_Mip_Linear,
        Min_Linear_Mag_Mip_Point,
        Min_Linear_Mag_Point_Mip_Linear,
        Min_Mag_Linear_Mip_Point,
        Min_Mag_Mip_Linear,
        Anisotropic
    };
};

struct Texture_Address_Mode {
    enum Type {
        Wrap,
        Mirror,
        Clamp,
        Border,
        Mirror_Once
    };
};

struct DepthWriteMask {
    enum Type {
        Zero,
        All
    };
};

struct CompareFunc {
    enum Type {
        Never = 1,
        Less,
        Equal,
        Less_Equal,
        Greater,
        Not_Equal,
        Greater_Equal,
        Always
    };
};

struct StencilOp {
    enum Type {
        Keep = 1,
        Zero,
        Replace,
        Increment_Sat,
        Decrement_Sat,
        Invert,
        Increment,
        Decrement
    };
};

//! Blend factors that can be applied to operands (RGB and alpha components).
struct Blend_Factor {
    enum Type {
        Zero,
        One,
        Source_Color,
        Invert_Source_Color,
        Source_Alpha,
        Invert_Source_Alpha,
        Destionation_Aplha,
        Invert_Destination_Alpha,
        Destination_Color,
        Invert_Destination_Color,
        Source_Alpha_Saturate,
        BlendFactor,
        Invert_BlendFactor,
        Source1_Color,
        Invert_Source1_Color,
        Source1_Alpha,
        Invert_Source1_Alpha
    };
};

//! Blend operators.
struct Blend_Op {
    enum Type {
        Add,
        Substract,
        Reverse_Substract,
        Minimum,
        Maximum
    };
};

struct Color_Write_Flag {
    enum {
        Red = 1,
        Green = 2,
        Blue = 4,
        Alpha = 8,
        All_Channels = (Red | Green | Blue | Alpha)
    };
};

struct FillMode {
    enum Type {
        Solid = 0,
        Wireframe
    };
};

struct CullMode {
    enum Type {
        None,
        Front,
        Back
    };
};

struct Shader_Model {
    enum Type {
        SM_3_0,
        SM_4_0,
        SM_4_1,
        SM_5_0
    };
};

struct FX_Model {
    enum Type {
        FX_4_0,
        FX_4_1,
        FX_5_0
    };
};

struct ResourceMapFlags {
    enum Type {
        Read = 1,
        Write,
        ReadWrite,
        WriteDiscard,
        WriteNoOverWrite
    };
};

struct ResourceUsage {
    enum Type {
        Default,
        Immutable,
        Dynamic,
        Staging
    };
};

struct CPUAccess {
    enum Type {
        None = 0U,
        Write = (1U << 0),
        Read = (1U << 1)
    };
};

struct BindingFlag {
    enum Value {
        ShaderResource          = (1u << 0),
        StreamOutput            = (1u << 1),
        RenderTarget            = (1u << 2),
        DepthStencil            = (1u << 3),
        UnorderedAccess         = (1u << 4)
    };
};

//! \brief Options that can be used when presenting a frame to the screen.
struct FramePresent {
    enum Type {
        All, //!< Present the frame.
        Test //!< Don't present the frame, but test if window is occluded.
    };
};

//! \brief Result of presenting a frame to the screen.
struct FramePresentResult {
    enum Type {
        Ok,
        Error,
        WindowOccluded
    };
};

struct Compile_Options {
    enum Value {
        Avoid_Flow_Control                                  = (1U << 0),
        Generate_Debug_Info                                 = (1U << 1),
        Use_Strict_Syntax                                   = (1U << 2),
        Enable_Backwards_Compatibility                      = (1U << 3),
        VS_Software                                         = (1U << 4),
        PS_Software                                         = (1U << 5),
        IEEE_Strictness                                     = (1U << 6),
        No_Preshader                                        = (1U << 7),
        Optimization_L0                                     = (1U << 8),
        Optimization_L1                                     = (1U << 9),
        Optimization_L2                                     = (1U << 10),
        Optimization_L3                                     = (1U << 11),
        Matrix_Packing_Row_Major                            = (1U << 12),
        Matrix_Packing_Column_Major                         = (1U << 13),
        Use_Partial_Precision                               = (1U << 14),
        Skip_Optimization                                   = (1U << 15),
        Skip_Validation                                     = (1U << 16),
        Warnings_Are_Errors                                 = (1U << 17),
        Prefer_Flow_Control                                 = (1U << 18)
    };
};

struct ElementType {
    enum Type {
        Unknown,
        Typeless32,
        Float32,
        Uint32,
        Sint32,
        Typeless16,
        Float16,
        Unorm16,
        Uint16,
        Snorm16,
        Typeless8,
        Unorm8,
        Uint8,
        Snorm8,
        Sint8
    };
};

} // namespace directx
} // namespace v8
