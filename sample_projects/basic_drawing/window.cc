// #include <d3dcompiler.h>
// #include <v8/math/matrix3X3.hpp>
// #include <v8/math/matrix4X4.hpp>
// #include <v8/base/count_of.hpp>
// #include <v8/rendering/constants.hpp>
// #include <v8/rendering/renderer.hpp>
// #include <v8/rendering/index_buffer.hpp>
// #include <v8/rendering/vertex_buffer.hpp>
// #include <v8/rendering/fragment_shader.hpp>
// #include <v8/rendering/shader_info.hpp>
// #include <v8/rendering/vertex_shader.hpp>
// #include <v8/rendering/vertex_pc.hpp>
// #include <v8/rendering/vertex_pn.hpp>
// #include "window.hpp"

// extern v8::rendering::renderer* graphics_sys_ptr;

// struct xrDrawingContext {
//     v8::rendering::renderer*    graphics_context;
//     v8::math::matrix_4X4F       view_proj_matrix;
// };

// void main_window::frame_draw_impl() {
//     graphics_sys_ptr->reset_blending_state();
//     graphics_sys_ptr->reset_depth_stencil_state();
//     graphics_sys_ptr->clear_depth_stencil();
//     graphics_sys_ptr->clear_backbuffer();
// }

// class xrWorldCoordFrame {
// public :
//     xrWorldCoordFrame();

// public :
//     v8_bool_t   Initialize(v8::rendering::renderer* graphics_context);

//     void        Update(const float delta);

//     void        Draw(xrDrawingContext* draw_context);
// private :
//     v8::rendering::vertex_buffer            vbuff_;
//     v8::rendering::index_buffer             ibuff_;
//     v8::rendering::fragment_shader          fragshader_;
//     v8::rendering::vertex_shader            vertshader_;
//     v8_int32_t                              topology_;

// private :
//     NO_CC_ASSIGN(xrWorldCoordFrame);
// };

// xrWorldCoordFrame::xrWorldCoordFrame()
//     :   topology_(v8::rendering::PrimitiveTopology::LineList)
// {}

// v8_bool_t xrWorldCoordFrame::Initialize(
//     v8::rendering::renderer* graphics_context
//     ) {
//     using namespace v8::rendering;
//     using namespace v8::math;

//     const v8::rendering::vertex_pc kVertexList[] = {
//         vertex_pc(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
//         vertex_pc(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
//         //
//         // Y axis (blue)
//         vertex_pc(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),
//         vertex_pc(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
//         //
//         // Z axis (green)
//         vertex_pc(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
//         vertex_pc(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
//     };

//     const v8_uint16_t kIndices[] = {
//         0, 1, 2, 3, 4, 5
//     };

//     if (!vbuff_.initialize(graphics_context, dimension_of(kVertexList),
//                            sizeof(kVertexList[0]), kVertexList)) {
//         return false;
//     }

//     if (!ibuff_.initialize(graphics_context, dimension_of(kIndices), 
//                            sizeof(kIndices[0]), kIndices)) {
//         return false;
//     }

//     const v8_uint32_t kCompileFlags = 
//         D3DCOMPILE_DEBUG | D3DCOMPILE_WARNINGS_ARE_ERRORS
//         | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

//     shader_info_t shader_data;
//     shader_data.compile_flags = kCompileFlags;
//     shader_data.entrypoint = "vs_main";
//     shader_data.is_filename = true;
//     shader_data.name_or_source = "C:\\shaders\\basic_vs.hlsl";
//     shader_data.shader_model = "vs_5_0";

//     if (!vertshader_.initialize(shader_data, graphics_context)) {
//         return false;
//     }

//     shader_data.entrypoint = "ps_main";
//     shader_data.name_or_source = "C:\\shaders\\basic_ps.hlsl";
//     shader_data.shader_model = "ps_5_0";
//     if (!fragshader_.initialize(shader_data, graphics_context)) {
//         return false;
//     }
//     return true;
// }

// void xrWorldCoordFrame::Update(const float /*delta*/) {
// }

// void xrWorldCoordFrame::Draw(xrDrawingContext* draw_context) {
//     const v8::math::matrix_4X4F model_to_world(
//         v8::math::matrix_3X3F().make_scale(20.0f, 20.0f, 20.0f)
//         );
//     const v8::math::matrix_4X4F world_view_proj = 
//         draw_context->view_proj_matrix * model_to_world;

//     vbuff_.bind_to_pipeline(draw_context->graphics_context);
//     ibuff_.bind_to_pipeline(draw_context->graphics_context);
//     draw_context->graphics_context->ia_stage_set_primitive_topology_type(
//         v8::rendering::PrimitiveTopology::LineList
//         );

//     vertshader_.set_uniform_by_name("world_view_proj", world_view_proj);

//     vertshader_.bind_to_pipeline(draw_context->graphics_context);
//     fragshader_.bind_to_pipeline(draw_context->graphics_context);

//     draw_context->graphics_context->draw_indexed(6);
// }