#include "pch_hdr.hpp"

#include <v8/math/math_constants.hpp>
#include <v8/math/geometry_generators.hpp>

namespace {

void subdivide_geometry(v8::math::geometry_gen::mesh_data_t* mesh_data) {
	// Save a copy of the input geometry.
    using namespace v8::math::geometry_gen;
    using namespace v8::math;

    mesh_data_t input_copy = *mesh_data;

	mesh_data->md_vertices.resize(0);
	mesh_data->md_indices.resize(0);

    /*
           v1
           .
          / \
         /   \
      m0.-----.m1
       / \   / \
      /   \ /   \
     .-----.-----.
     v0    m2     v2
     */

	uint32_t numTris = static_cast<uint32_t>(input_copy.md_indices.size() / 3);
	for(uint32_t i = 0; i < numTris; ++i) {
		vertex_pntt v0 = input_copy.md_vertices[input_copy.md_indices[i * 3 + 0]];
		vertex_pntt v1 = input_copy.md_vertices[input_copy.md_indices[i * 3 + 1]];
		vertex_pntt v2 = input_copy.md_vertices[input_copy.md_indices[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		vertex_pntt m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex_pntt components in CreateGeosphere.

        m0.vt_position = vector3F(
			0.5f*(v0.vt_position.x_ + v1.vt_position.x_),
			0.5f*(v0.vt_position.y_ + v1.vt_position.y_),
			0.5f*(v0.vt_position.z_ + v1.vt_position.z_));

        m1.vt_position = vector3F(
			0.5f*(v1.vt_position.x_ + v2.vt_position.x_),
			0.5f*(v1.vt_position.y_ + v2.vt_position.y_),
			0.5f*(v1.vt_position.z_ + v2.vt_position.z_));

        m2.vt_position = vector3F(
			0.5f*(v0.vt_position.x_ + v2.vt_position.x_),
			0.5f*(v0.vt_position.y_ + v2.vt_position.y_),
			0.5f*(v0.vt_position.z_ + v2.vt_position.z_));

		//
		// Add new geometry.
		//

		mesh_data->md_vertices.push_back(v0); // 0
		mesh_data->md_vertices.push_back(v1); // 1
		mesh_data->md_vertices.push_back(v2); // 2
		mesh_data->md_vertices.push_back(m0); // 3
		mesh_data->md_vertices.push_back(m1); // 4
		mesh_data->md_vertices.push_back(m2); // 5
 
		mesh_data->md_indices.push_back(i*6+0);
		mesh_data->md_indices.push_back(i*6+3);
		mesh_data->md_indices.push_back(i*6+5);

		mesh_data->md_indices.push_back(i*6+3);
		mesh_data->md_indices.push_back(i*6+4);
		mesh_data->md_indices.push_back(i*6+5);

		mesh_data->md_indices.push_back(i*6+5);
		mesh_data->md_indices.push_back(i*6+4);
		mesh_data->md_indices.push_back(i*6+2);

		mesh_data->md_indices.push_back(i*6+3);
		mesh_data->md_indices.push_back(i*6+1);
		mesh_data->md_indices.push_back(i*6+4);
	}
}

void generate_cylinder_top_cap(
    float /*bottom_radius*/, 
    float top_radius, 
    float height, 
	size_t slice_count, 
    size_t /*stack_count*/, 
    v8::math::geometry_gen::mesh_data_t* mesh_data
    ) {
    using namespace v8::math;
    using namespace v8::math::geometry_gen;

	uint32_t base_index = static_cast<uint32_t>(mesh_data->md_vertices.size());

	float y = 0.5f * height;
    float delta_theta = numericsF::two_pi() / slice_count;

	// Duplicate cap ring md_vertices because the texture coordinates and normals differ.
	for(size_t i = 0; i <= slice_count; ++i) {
		float x = top_radius*cosf(i*delta_theta);
		float z = top_radius*sinf(i*delta_theta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x/height + 0.5f;
		float v = z/height + 0.5f;

		mesh_data->md_vertices.push_back(
            vertex_pntt(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
	}

	// Cap center vertex.
	mesh_data->md_vertices.push_back(
        vertex_pntt(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

	// Index of center vertex.
	uint32_t center_index = static_cast<uint32_t>(
        mesh_data->md_vertices.size() - 1);

	for(uint32_t i = 0; i < static_cast<uint32_t>(slice_count); ++i) {
		mesh_data->md_indices.push_back(center_index);
		mesh_data->md_indices.push_back(base_index + i + 1);
		mesh_data->md_indices.push_back(base_index + i);
	}
}

void generate_cylinder_bottom_cap(
    float bottom_radius, 
    float /*top_radius*/, 
    float height, 
	size_t slice_count, 
    size_t /*stack_count*/, 
    v8::math::geometry_gen::mesh_data_t* mesh_data) {
    using namespace v8::math;
    using namespace v8::math::geometry_gen;
	// 
	// Build bottom cap.
	//
	uint32_t base_index = static_cast<uint32_t>(mesh_data->md_vertices.size());
	float y = -0.5f * height;

	// md_vertices of ring
    float delta_theta = numericsF::two_pi() / slice_count;
	for(size_t i = 0; i <= slice_count; ++i) {
		float x = bottom_radius * cosf(i * delta_theta);
		float z = bottom_radius * sinf(i * delta_theta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

        mesh_data->md_vertices.push_back(vertex_pntt(
            x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
	}

	// Cap center vertex_pntt.
	mesh_data->md_vertices.push_back(
        vertex_pntt(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

	// Cache the index of center vertex_pntt.
	uint32_t center_index = static_cast<uint32_t>(
        mesh_data->md_vertices.size() - 1);

	for(uint32_t i = 0; i < static_cast<uint32_t>(slice_count); ++i) {
		mesh_data->md_indices.push_back(center_index);
		mesh_data->md_indices.push_back(base_index + i);
		mesh_data->md_indices.push_back(base_index + i + 1);
	}
}

} // anonymous namespace

void v8::math::geometry_gen::create_box(
    float width, 
    float height, 
    float depth, 
    mesh_data_t* mesh_data
    ) {
	vertex_pntt v[24];

	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;
    
	// Fill in the front face vertex_pntt data.
	v[0] = vertex_pntt(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = vertex_pntt(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = vertex_pntt(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = vertex_pntt(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex_pntt data.
	v[4] = vertex_pntt(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = vertex_pntt(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = vertex_pntt(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = vertex_pntt(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex_pntt data.
	v[8]  = vertex_pntt(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9]  = vertex_pntt(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = vertex_pntt(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = vertex_pntt(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex_pntt data.
	v[12] = vertex_pntt(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = vertex_pntt(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = vertex_pntt(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = vertex_pntt(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex_pntt data.
	v[16] = vertex_pntt(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = vertex_pntt(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = vertex_pntt(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = vertex_pntt(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex_pntt data.
	v[20] = vertex_pntt(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = vertex_pntt(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = vertex_pntt(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = vertex_pntt(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

    using namespace std;

    mesh_data->md_vertices.reserve(24);
    mesh_data->md_vertices.assign(begin(v), end(v));
 
	uint32_t indices[36];

	//
    // Front
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// back
    //
	indices[6] = 4; indices[7]  = 5; indices[8]  = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;

	//
    // top
	indices[12] = 8; indices[13] =  9; indices[14] = 10;
	indices[15] = 8; indices[16] = 10; indices[17] = 11;

	//
    // bottom
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;

	//
    // left
	indices[24] = 16; indices[25] = 17; indices[26] = 18;
	indices[27] = 16; indices[28] = 18; indices[29] = 19;

	//
    // right
	indices[30] = 20; indices[31] = 21; indices[32] = 22;
	indices[33] = 20; indices[34] = 22; indices[35] = 23;

    mesh_data->md_indices.reserve(36);
    mesh_data->md_indices.assign(begin(indices), end(indices));
}

void v8::math::geometry_gen::create_sphere(
    float radius,
    size_t slice_count,
    size_t stack_count,
    mesh_data_t* mesh_data
    ) {
	mesh_data->md_vertices.clear();
	mesh_data->md_indices.clear();

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	vertex_pntt top_vertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertex_pntt bottom_vertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	mesh_data->md_vertices.push_back( top_vertex );

    float phiStep   = numericsF::pi() / stack_count;
    float thetaStep = numericsF::two_pi() / slice_count;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for(uint32_t i = 1; i <= stack_count - 1; ++i) {
		float phi = i * phiStep;

		// Vertices of ring.
		for(uint32_t j = 0; j <= slice_count; ++j) {
			float theta = j * thetaStep;

			vertex_pntt v;

			// spherical to cartesian
			v.vt_position.x_ = radius * sinf(phi) * cosf(theta);
            v.vt_position.y_ = radius * cosf(phi);
            v.vt_position.z_ = radius * sinf(phi) * sinf(theta);

			// Partial derivative of P with respect to theta
			v.vt_tangent.x_ = -radius * sinf(phi) * sinf(theta);
			v.vt_tangent.y_ = 0.0f;
			v.vt_tangent.z_ = +radius * sinf(phi) * cosf(theta);
            v.vt_tangent.normalize();

            v.vt_normal = v.vt_position;
            v.vt_normal.normalize();

            v.vt_texcoord.x_ = theta / numericsF::two_pi();
            v.vt_texcoord.y_ = phi / numericsF::pi();

			mesh_data->md_vertices.push_back( v );
		}
	}

	mesh_data->md_vertices.push_back(bottom_vertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex_pntt buffer
	// and connects the top pole to the first ring.
	//

	for(uint32_t i = 1; i <= slice_count; ++i) {
		mesh_data->md_indices.push_back(0);
		mesh_data->md_indices.push_back(i + 1);
		mesh_data->md_indices.push_back(i);
	}
	
	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex_pntt in the first ring.
	// This is just skipping the top pole vertex_pntt.
	uint32_t base_index = 1;
	uint32_t ring_count = static_cast<uint32_t>(slice_count + 1);
	for(uint32_t i = 0; i < stack_count - 2; ++i) {
		for(uint32_t j = 0; j < slice_count; ++j) {
			mesh_data->md_indices.push_back(base_index + i * ring_count + j);
			mesh_data->md_indices.push_back(base_index + i * ring_count + j + 1);
			mesh_data->md_indices.push_back(base_index + (i + 1) * ring_count + j);

			mesh_data->md_indices.push_back(base_index + (i + 1) * ring_count + j);
			mesh_data->md_indices.push_back(base_index + i * ring_count + j + 1);
			mesh_data->md_indices.push_back(base_index + (i + 1) * ring_count + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex_pntt buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex_pntt was added last.
	uint32_t south_pole_idx = (uint32_t) mesh_data->md_vertices.size() - 1;

	// Offset the indices to the index of the first vertex_pntt in the last ring.
	base_index = south_pole_idx - ring_count;
	
	for(uint32_t i = 0; i < slice_count; ++i) {
		mesh_data->md_indices.push_back(south_pole_idx);
		mesh_data->md_indices.push_back(base_index + i);
		mesh_data->md_indices.push_back(base_index + i + 1);
	}
}


void v8::math::geometry_gen::create_geosphere(
    float radius,
    size_t subdivisions,
    mesh_data_t* mesh_data
    ) {
    subdivisions = clamp(subdivisions, subdivisions, size_t(5));

	// Approximate a sphere by tessellating an icosahedron.
	const float X = 0.525731f; 
	const float Z = 0.850651f;

	const vector3F pos[12] = {
		vector3F(-X, 0.0f, Z),  vector3F(X, 0.0f, Z),  
		vector3F(-X, 0.0f, -Z), vector3F(X, 0.0f, -Z),    
		vector3F(0.0f, Z, X),   vector3F(0.0f, Z, -X), 
		vector3F(0.0f, -Z, X),  vector3F(0.0f, -Z, -X),    
		vector3F(Z, X, 0.0f),   vector3F(-Z, X, 0.0f), 
		vector3F(Z, -X, 0.0f),  vector3F(-Z, -X, 0.0f)
	};

	const uint32_t k[60] = {
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	};

	mesh_data->md_vertices.resize(12);
	mesh_data->md_indices.resize(60);

	for(uint32_t i = 0; i < 12; ++i)
        mesh_data->md_vertices[i].vt_position = pos[i];

	for(uint32_t i = 0; i < 60; ++i)
		mesh_data->md_indices[i] = k[i];

	for(uint32_t i = 0; i < subdivisions; ++i)
        subdivide_geometry(mesh_data);

	// Project md_vertices onto sphere and scale.
	for(uint32_t i = 0; i < mesh_data->md_vertices.size(); ++i) {
        mesh_data->md_vertices[i].vt_normal = normal_of(mesh_data->md_vertices[i].vt_position);
        mesh_data->md_vertices[i].vt_position = radius * mesh_data->md_vertices[i].vt_position;		

		// Derive texture coordinates from spherical coordinates.
        float theta = angle_from_xy(
			mesh_data->md_vertices[i].vt_position.x_, 
			mesh_data->md_vertices[i].vt_position.z_);

		float phi = acosf(mesh_data->md_vertices[i].vt_position.y_ / radius);

        mesh_data->md_vertices[i].vt_texcoord.x_ = theta / numericsF::two_pi();
        mesh_data->md_vertices[i].vt_texcoord.y_ = phi / numericsF::pi();

		// Partial derivative of P with respect to theta
		mesh_data->md_vertices[i].vt_tangent.x_ = -radius * sinf(phi) * sinf(theta);
		mesh_data->md_vertices[i].vt_tangent.y_ = 0.0f;
		mesh_data->md_vertices[i].vt_tangent.z_ = +radius * sinf(phi) * cosf(theta);
        mesh_data->md_vertices[i].vt_tangent.normalize();
	}
}

void v8::math::geometry_gen::create_cylinder(
    float bottom_radius,
    float top_radius,
    float height,
    size_t slice_count,
    size_t stack_count,
    mesh_data_t* mesh_data
    ) {
    mesh_data->md_vertices.clear();
	mesh_data->md_indices.clear();

    using namespace v8::math;
    using namespace v8::math::geometry_gen;

	float stackHeight = height / stack_count;
    //
	// Amount to increment radius as we move up each stack level from bottom to top.
	float radiusStep = (top_radius - bottom_radius) / stack_count;
	size_t ringCount = stack_count + 1;

	// Compute md_vertices for each stack ring starting at the bottom and moving up.
	for(size_t i = 0; i < ringCount; ++i) {
		float y = -0.5f * height + i * stackHeight;
		float r = bottom_radius + i * radiusStep;

		// md_vertices of ring
        float dTheta = numericsF::two_pi() / slice_count;
		for(size_t j = 0; j <= slice_count; ++j) {
			vertex_pntt vertex;

			float c = cosf(j * dTheta);
			float s = sinf(j * dTheta);

            vertex.vt_position = vector3F(r*c, y, r*s);

            vertex.vt_texcoord.x_ = (float) j / slice_count;
			vertex.vt_texcoord.y_ = 1.0f - (float) i / stack_count;

			// Cylinder can be parameterized as follows, where we introduce v
			// parameter that goes in the same direction as the v tex-coord
			// so that the bitangent goes in the same direction as the v tex-coord.
			//   Let r0 be the bottom radius and let r1 be the top radius.
			//   y(v) = h - hv for v in [0,1].
			//   r(v) = r1 + (r0-r1)v
			//
			//   x(t, v) = r(v)*cos(t)
			//   y(t, v) = h - hv
			//   z(t, v) = r(v)*sin(t)
			// 
			//  dx/dt = -r(v)*sin(t)
			//  dy/dt = 0
			//  dz/dt = +r(v)*cos(t)
			//
			//  dx/dv = (r0-r1)*cos(t)
			//  dy/dv = -h
			//  dz/dv = (r0-r1)*sin(t)

			// This is unit length.
            vertex.vt_tangent = vector3F(-s, 0.0f, c);

			float dr = bottom_radius - top_radius;
            vector3F bitangent(dr * c, -height, dr * s);
            vertex.vt_normal = normal_of(cross_product(vertex.vt_tangent, bitangent));
            mesh_data->md_vertices.push_back(vertex);
		}
	}

	// Add one because we duplicate the first and last vertex_pntt per ring
	// since the texture coordinates are different.
	uint32_t ring_count = static_cast<uint32_t>(slice_count + 1);

	// Compute md_indices for each stack.
	for(uint32_t i = 0; i < static_cast<uint32_t>(stack_count); ++i) {
		for(uint32_t j = 0; j < static_cast<uint32_t>(slice_count); ++j) {
			mesh_data->md_indices.push_back(i * ring_count + j);
			mesh_data->md_indices.push_back((i + 1) * ring_count + j);
			mesh_data->md_indices.push_back((i + 1) * ring_count + j + 1);

			mesh_data->md_indices.push_back(i * ring_count + j);
			mesh_data->md_indices.push_back((i + 1) * ring_count + j + 1);
			mesh_data->md_indices.push_back(i * ring_count + j + 1);
		}
	}

    generate_cylinder_top_cap(bottom_radius, top_radius, height, slice_count, 
                              stack_count, mesh_data);
    generate_cylinder_bottom_cap(bottom_radius, top_radius, height, 
                                 slice_count, stack_count, mesh_data);
}

void v8::math::geometry_gen::create_grid(
    const float         grid_width,
    const float         grid_depth,
    const v8_int_t      row_count,
    const v8_int_t      column_count,
    mesh_data_t*        mesh
    ) {
    const v8_int_t vertexCount = row_count * column_count;
    const v8_int_t faceCount   = (row_count - 1) * (column_count - 1) * 2;

    const float halfWidth = 0.5f * grid_width;
    const float halfDepth = 0.5f * grid_depth;

    const float delta_x = grid_width / (column_count - 1);
    const float delta_z = grid_depth / (row_count - 1 );

    const float delta_tu = 1.0f / (column_count - 1);
    const float delta_tv = 1.0f / (row_count - 1);

    mesh->md_vertices.resize(static_cast<size_t>(vertexCount));

    for(v8_int_t row_idx = 0; row_idx < row_count; ++row_idx) {
        const float z_coord = halfDepth - row_idx * delta_z;

        for(v8_int_t col_idx = 0; col_idx < column_count; ++col_idx) {
            const float x_coord = -halfWidth + col_idx * delta_x;

            mesh->md_vertices[row_idx * column_count + col_idx].vt_position =
                vector3F(x_coord, 0.0f, z_coord);
            mesh->md_vertices[row_idx * column_count + col_idx].vt_normal =
                vector3F(0.0f, 1.0f, 0.0f);
            mesh->md_vertices[row_idx * column_count + col_idx].vt_tangent =
                vector3F(1.0f, 0.0f, 0.0f);

			// Stretch texture over grid.
            mesh->md_vertices[row_idx * column_count + col_idx].vt_texcoord.x_ =
                    col_idx * delta_tu;
            mesh->md_vertices[row_idx * column_count + col_idx].vt_texcoord.y_ =
                    row_idx * delta_tv;
		}
	}
 
    //
    // Setup indices for the generated vertices.
    mesh->md_indices.resize(static_cast<size_t>(faceCount * 3));

    v8_int_t quad_idx = 0;
    for(v8_int_t row_idx = 0; row_idx < row_count - 1; ++row_idx) {
        for(v8_int_t col_idx = 0; col_idx < column_count - 1; ++col_idx) {
            mesh->md_indices[quad_idx]     =
                    (uint32_t) (row_idx * column_count + col_idx);
            mesh->md_indices[quad_idx + 1] =
                    (uint32_t) (row_idx * column_count + col_idx + 1);
            mesh->md_indices[quad_idx + 2] =
                    (uint32_t) ((row_idx + 1) * column_count + col_idx);

            mesh->md_indices[quad_idx + 3] =
                    (uint32_t) ((row_idx + 1) * column_count + col_idx);
            mesh->md_indices[quad_idx + 4] =
                    (uint32_t) (row_idx * column_count + col_idx + 1);
            mesh->md_indices[quad_idx + 5] =
                    (uint32_t) ((row_idx + 1) * column_count + col_idx + 1);
            quad_idx += 6; // next quad
		}
    }
}

void v8::math::geometry_gen::create_fullscreen_quad(
    mesh_data_t* mesh_data
    ) {
    mesh_data->md_vertices.resize(4);
	mesh_data->md_indices.resize(6);

	// Position coordinates specified in NDC space.
	mesh_data->md_vertices[0] = vertex_pntt(
		-1.0f, -1.0f, 0.0f, 
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f);

	mesh_data->md_vertices[1] = vertex_pntt(
		-1.0f, +1.0f, 0.0f, 
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f);

	mesh_data->md_vertices[2] = vertex_pntt(
		+1.0f, +1.0f, 0.0f, 
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f);

	mesh_data->md_vertices[3] = vertex_pntt(
		+1.0f, -1.0f, 0.0f, 
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f);

	mesh_data->md_indices[0] = 0;
	mesh_data->md_indices[1] = 1;
	mesh_data->md_indices[2] = 2;

	mesh_data->md_indices[3] = 0;
	mesh_data->md_indices[4] = 2;
	mesh_data->md_indices[5] = 3;
}
