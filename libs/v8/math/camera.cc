#include "pch_hdr.hpp"
#include "v8/v8.hpp"
#include "v8/math/camera.hpp"

const v8::math::camera::depth_range_entry_t 
v8::math::camera::C_depth_ranges[2] = {
    { 0.0f,     1.0f },
    { -1.0f,    1.0f }
};

v8::math::camera::camera()
    :   view_pos_(vector3F::zero),
        view_side_(vector3F::unit_x),
        view_up_(vector3F::unit_y),
        view_dir_(vector3F::unit_z),
        view_matrix_(matrix_4X4F::identity),
        projection_matrix_(matrix_4X4F::identity),
        projection_view_matrix_(matrix_4X4F::identity),
        projection_type_(Projection::Perspective),
        frameid_(0),
        depth_type_(DepthType::DirectX) {
}

void v8::math::camera::update_view_matrix() {
    view_matrix_.a11_ = view_side_.x_;
    view_matrix_.a12_ = view_side_.y_;
    view_matrix_.a13_ = view_side_.z_;
    view_matrix_.a14_ = -v8::math::dot_product(view_side_, view_pos_);

    view_matrix_.a21_ = view_up_.x_;
    view_matrix_.a22_ = view_up_.y_;
    view_matrix_.a23_ = view_up_.z_;
    view_matrix_.a24_ = -v8::math::dot_product(view_up_, view_pos_);

    view_matrix_.a31_ = view_dir_.x_;
    view_matrix_.a32_ = view_dir_.y_;
    view_matrix_.a33_ = view_dir_.z_;
    view_matrix_.a34_ = -v8::math::dot_product(view_dir_, view_pos_);

    view_matrix_.set_row(4, v8::math::vector4F(0.0f, 0.0f, 0.0f, 1.0f));

    ++frameid_;
}

v8::math::camera&
v8::math::camera::set_view_frame( 
    const v8::math::vector3F& origin, 
    const v8::math::vector3F& dir_vector, 
    const v8::math::vector3F& up_vector, 
    const v8::math::vector3F& right_vector 
    ) {
    view_pos_ = origin;
    return set_axes(dir_vector, up_vector, right_vector);
}

v8::math::camera& 
v8::math::camera::set_axes( 
    const v8::math::vector3F& dir_vector, 
    const v8::math::vector3F& up_vector, 
    const v8::math::vector3F& right_vector
    ) {
    assert(is_zero(v8::math::dot_product(dir_vector, up_vector)));
    assert(is_zero(v8::math::dot_product(dir_vector, right_vector)));
    assert(is_zero(v8::math::dot_product(up_vector, right_vector)));
    
    view_side_ = right_vector;
    view_up_ = up_vector;
    view_dir_ = dir_vector;
    
    update_view_matrix();
    update_projection_view_transform();
    return *this;
}

v8::math::camera& v8::math::camera::look_at( 
    const math::vector3F& origin, 
    const math::vector3F& world_up, 
    const math::vector3F& target
    ) {
    const vector3F D = (target - origin).normalize();
    const vector3F R = cross_product(world_up, D).normalize();
    const vector3F U = cross_product(D, R);

    return set_view_frame(origin, D, U, R);
}

void v8::math::camera::set_projection_matrix(
    const v8::math::vector4F& first_col, 
    const v8::math::vector4F& second_col, 
    const v8::math::vector4F& third_col, 
    const v8::math::vector4F& fourth_col
    ) {
    projection_matrix_.set_column(1, first_col);
    projection_matrix_.set_column(2, second_col);
    projection_matrix_.set_column(3, third_col);
    projection_matrix_.set_column(4, fourth_col);
    
    update_projection_view_transform();
}

void v8::math::camera::handle_frustrum_param_change() {
    const float dmin = frustrum_params_[FrustrumParam::DMin];
    const float dmax = frustrum_params_[FrustrumParam::DMax];
    const float umin = frustrum_params_[FrustrumParam::UMin];
    const float umax = frustrum_params_[FrustrumParam::UMax];
    const float rmin = frustrum_params_[FrustrumParam::RMin];
    const float rmax = frustrum_params_[FrustrumParam::RMax];
    const float NDCZmin = C_depth_ranges[depth_type_][NDC_dmin];
    const float NDCZmax = C_depth_ranges[depth_type_][NDC_dmax];
    const float ndc_zdiff = NDCZmax - NDCZmin;
    
    if (projection_type_ == Projection::Perspective) {
        const float inv_rdiff = 1.0f / (rmax - rmin);
        const float inv_udiff = 1.0f / (umax - umin);
        const float inv_ddiff = 1.0f / (dmax - dmin);
        
        projection_matrix_(1, 1) = 2 * dmin * inv_rdiff;
        projection_matrix_(2, 1) = 0.0f;
        projection_matrix_(3, 1) = 0.0f;
        projection_matrix_(4, 1) = 0.0f;

        projection_matrix_(1, 2) = 0.0f;
        projection_matrix_(2, 2) = 2 * dmin * inv_udiff;
        projection_matrix_(3, 2) = 0.0f;
        projection_matrix_(4, 2) = 0.0f;

        projection_matrix_(1, 3) = -(rmax + rmin) * inv_rdiff;
        projection_matrix_(2, 3) = -(umax + umin) * inv_udiff;
        projection_matrix_(3, 3) = NDCZmin + dmax * ndc_zdiff * inv_ddiff;
        projection_matrix_(4, 3) = 1.0f;

        projection_matrix_(1, 4) = 0.0f;
        projection_matrix_(2, 4) = 0.0f;
        projection_matrix_(3, 4) = -(dmin * dmax) * ndc_zdiff * inv_ddiff;
        projection_matrix_(4, 4) = 0.0f;
    } else {
        const float inv_width = 1.0f / (rmax - rmin);
        const float inv_height = 1.0f / (umax - umin);
        const float inv_depth = 1.0f / (dmax - dmin);

        projection_matrix_(1, 1) = 2 * inv_width;
        projection_matrix_(2, 1) = 0.0f;
        projection_matrix_(3, 1) = 0.0f;
        projection_matrix_(4, 1) = 0.0f;

        projection_matrix_(1, 2) = 0.0f;
        projection_matrix_(2, 2) = 2 * inv_height;
        projection_matrix_(3, 2) = 0.0f;
        projection_matrix_(4, 2) = 0.0f;

        projection_matrix_(1, 3) = 0.0f;
        projection_matrix_(2, 3) = 0.0f;
        projection_matrix_(3, 3) = ndc_zdiff * inv_depth;
        projection_matrix_(4, 3) = 0.0f;

        projection_matrix_(1, 4) = -(rmax + rmin) * inv_width;
        projection_matrix_(2, 4) = -(umax + umin) * inv_height;
        projection_matrix_(3, 4) = NDCZmin - dmin *  ndc_zdiff * inv_depth;
        projection_matrix_(4, 4) = 1.0f;
    }

    ++frameid_;
    update_projection_view_transform();
}

void v8::math::camera::set_frustrum(
    float dmin, 
    float dmax, 
    float umin, 
    float umax, 
    float rmin, 
    float rmax,
    int projection_type
    ) {
    assert(((projection_type >= Projection::Perspective)
           && (projection_type < Projection::Last))
           && "Invalid projection type specified!");

    projection_type_ = projection_type;
    frustrum_params_[FrustrumParam::DMin] = dmin;
    frustrum_params_[FrustrumParam::DMax] = dmax;
    frustrum_params_[FrustrumParam::UMin] = umin;
    frustrum_params_[FrustrumParam::UMax] = umax;
    frustrum_params_[FrustrumParam::RMin] = rmin;
    frustrum_params_[FrustrumParam::RMax] = rmax;
    handle_frustrum_param_change();
}

void v8::math::camera::set_symmetric_frustrum(
    const float   depth_min,
    const float   depth_max,
    const float   width,
    const float   height,
    const int     projection_type
    ) {
    const float k_half_width = width * 0.5f;
    const float k_half_height = height * 0.5f;
    set_frustrum(depth_min, depth_max, 
                 -k_half_height, +k_half_height,
                 -k_half_width, +k_half_width,
                 projection_type);
}

void v8::math::camera::set_symmetric_frustrum_perspective(
    float fov_angle, float aspect_ratio, float dmin, float dmax
    ) {
    const float tan_half_angle = tan(to_radians(fov_angle / 2));

    frustrum_params_[FrustrumParam::DMin] = dmin;
    frustrum_params_[FrustrumParam::DMax] = dmax;
    frustrum_params_[FrustrumParam::UMax] = dmin * tan_half_angle;
    frustrum_params_[FrustrumParam::UMin] =
            -frustrum_params_[FrustrumParam::UMax];
    frustrum_params_[FrustrumParam::RMax] =
        aspect_ratio * frustrum_params_[FrustrumParam::UMax];
    frustrum_params_[FrustrumParam::RMin] =
            -frustrum_params_[FrustrumParam::RMax];

    projection_type_ = Projection::Perspective;
    handle_frustrum_param_change();
}

void v8::math::camera::copy_frustrum_vertices(
    v8::math::vector3F* vertices
    ) const {
    assert(vertices);

    const float d_min = frustrum_params_[FrustrumParam::DMin];
    const float d_max = frustrum_params_[FrustrumParam::DMax];
    const float r_min = frustrum_params_[FrustrumParam::RMin];
    const float r_max = frustrum_params_[FrustrumParam::RMax];
    const float u_min = frustrum_params_[FrustrumParam::UMin];
    const float u_max = frustrum_params_[FrustrumParam::UMax];
    const float far_mul = d_max / d_min;

    const vector3F d_min_D = d_min * view_dir_;
    const vector3F u_max_U = u_max * view_up_;
    const vector3F u_min_U = u_min * view_up_;
    const vector3F r_min_R = r_min * view_side_;
    const vector3F r_max_R = r_max * view_side_;

    vertices[FrustrumVertex::Near_TopLeft] = d_min_D + u_max_U + r_min_R;

    vertices[FrustrumVertex::Near_TopRight] = d_min_D + u_max_U + r_max_R;

    vertices[FrustrumVertex::Near_BottomLeft] = d_min_D + u_min_U + r_min_R;

    vertices[FrustrumVertex::Near_BottomRight] = d_min_D + u_min_U + r_max_R;

    vertices[FrustrumVertex::Far_TopLeft] =
        far_mul * vertices[FrustrumVertex::Near_TopLeft];

    vertices[FrustrumVertex::Far_TopRight] =
        far_mul * vertices[FrustrumVertex::Near_TopRight];

    vertices[FrustrumVertex::Far_BottomLeft] =
        far_mul * vertices[FrustrumVertex::Near_BottomLeft];

    vertices[FrustrumVertex::Far_BottomRight] =
        far_mul * vertices[FrustrumVertex::Near_BottomRight];

    for (v8_size_t i = 0U; i < 8U; ++i) {
        vertices[i] += view_pos_;
    }
}

void v8::math::camera::extract_frustrum_planes(
    plane3F* fplanes) const {

    const float d_min = frustrum_params_[FrustrumParam::DMin];
    const float d_max = frustrum_params_[FrustrumParam::DMax];
    const float r_min = frustrum_params_[FrustrumParam::RMin];
    const float r_max = frustrum_params_[FrustrumParam::RMax];
    const float u_min = frustrum_params_[FrustrumParam::UMin];
    const float u_max = frustrum_params_[FrustrumParam::UMax];
    const float d_min_squared = d_min * d_min;
    //
    // near plane bottom right vertex
    const vector3F vertex_nbr = view_pos_ + d_min * view_dir_ 
                                + u_min * view_up_ + r_max * view_side_;
    //
    // near plane top left vertex
    const vector3F vertex_ntl = view_pos_ + d_min * view_dir_
                                + u_max * view_up_ + r_min * view_side_;
    
    //
    // Let C be a camera with origin in point E, direction vector D,
    // up vector U and right vector R. Let the frustrum parameters be
    // d_min, d_max, r_min, r_max, u_min, u_max. We can derive the frustrum
    // planes in the following way :
    struct fplane_data {
        vector3F    fp_normal;
        vector3F    fp_origin;
    } frustrum_planes_data[] = {
        //
        // Near plane
        // Normal : D
        // Origin point : E + d_min * D
        { view_dir_, view_pos_ + d_min * view_dir_ },
        //
        // Far plane
        // Normal : -D
        // Origin point : E + d_max * D
        { -view_dir_, view_pos_ + d_max * view_dir_ },
        //
        // Top plane
        // Normal : (u_max * D - d_min * U) / sqrt(d_min^2 + u_max^2)
        // Origin point : E + d_min * D + u_max * U + r_min * R
        { (u_max * view_dir_ - d_min * view_up_)
          * inv_sqrt(d_min_squared + u_max * u_max),
          vertex_ntl },
        //
        // Bottom plane
        // Normal : (-u_min * D + d_min * U) / sqrt(d_min^2 + u_min^2)
        // Origin point : E + d_min * D + u_min * U + r_max * R
        { (d_min * view_up_ - u_min * view_dir_) 
          * inv_sqrt(d_min_squared + u_min * u_min),
          vertex_nbr },
        //
        // Left plane
        // Normal : (d_min * R - r_min * D) / sqrt(d_min^2 + r_min^2)
        // Origin point : E + d_min * D + u_max * U + r_min * R
        { (d_min * view_side_ - r_min * view_dir_) 
          * inv_sqrt(d_min_squared + r_min * r_min),
          vertex_ntl },
        //
        // Right plane
        // Normal : (r_max * D - d_min * R) / sqrt(d_min^2 + r_max^2)
        // Origin point : E + d_min * D + u_min * U + r_max * R
        { (r_max * view_dir_ - d_min * view_side_) 
          * inv_sqrt(d_min_squared + r_max * r_max),
          vertex_nbr }
    };

    using namespace std;
    transform(begin(frustrum_planes_data), end(frustrum_planes_data), 
              fplanes,
              [](const fplane_data& plane_data) {
        return plane3F(plane_data.fp_normal, plane_data.fp_origin);
    });
}

void v8::math::camera::extract_frustrum_planes2(plane3F* planes) const {
    using namespace std;

    vector4F mtx_row1;
    projection_view_matrix_.get_row(1, &mtx_row1);

    vector4F mtx_row2;
    projection_view_matrix_.get_row(2, &mtx_row2);

    vector4F mtx_row3;
    projection_view_matrix_.get_row(3, &mtx_row3);

    vector4F mtx_row4;
    projection_view_matrix_.get_row(4, &mtx_row4);

    const vector4F N_near = mtx_row3 + mtx_row4;
    const vector4F N_far = mtx_row4 - mtx_row3;
    const vector4F N_left = mtx_row1 + mtx_row4;
    const vector4F N_right = mtx_row4 - mtx_row1;
    const vector4F N_top = mtx_row4 - mtx_row2;
    const vector4F N_bottom = mtx_row4 + mtx_row2;

    //
    // Near
    planes[0] = plane3F(N_near.x_, N_near.y_, N_near.z_, N_near.w_);
    //
    // Far
    planes[1] = plane3F(N_far.x_, N_far.y_, N_far.z_, N_far.w_);
    //
    // Top
    planes[2] = plane3F(N_top.x_, N_top.y_, N_top.z_, N_top.w_);
    //
    // Bottom
    planes[3] = plane3F(N_bottom.x_, N_bottom.y_, N_bottom.z_, N_bottom.w_);
    //
    // Left
    planes[4] = plane3F(N_left.x_, N_left.y_, N_left.z_, N_left.w_);
    //
    // Right
    planes[5] = plane3F(N_right.x_, N_right.y_, N_right.z_, N_right.w_);

    for_each(planes, planes + 6, [](plane3F& frustrum_plane) {
        frustrum_plane.normalize();
    });
}
