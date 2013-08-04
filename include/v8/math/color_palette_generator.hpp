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

#include <algorithm>
#include <vector>

#include <v8/v8.hpp>
#include <v8/math/color.hpp>
#include <v8/math/math_utils.hpp>

namespace v8 { namespace math {

struct force_vector_t {
    float   dl;
    float   da;
    float   db;

    force_vector_t() {}

    force_vector_t(const float delta_lum,
                   const float delta_a,
                   const float delta_b)
        :       dl(delta_lum)
            ,   da(delta_a)
            ,   db(delta_b)
    {}
};

template<typename ColorChecker>
void 
generate_color_palette(const v8_int32_t                 num_colors,
                       const ColorChecker&              color_checker__,
                       const v8_bool_t                  force_mode,
                       const v8_int32_t                 quality,
                       const v8_bool_t                  /*higher_precision*/,
                       v8::math::color_rgb*             output_colors)
{
    if (force_mode) {

        auto check_if_lab_in_rgb_space = [&color_checker__](const color_lab& lab) {
            color_rgb rgb;
            lab_to_rgb(lab, &rgb);

            return rgb.Red >= 0.0f 
                    && rgb.Green >= 0.0f 
                    && rgb.Blue >= 0.0f 
                    && rgb.Red <= 1.0f 
                    && rgb.Green <= 1.0f 
                    && rgb.Blue <= 1.0f 
                    && color_checker__(rgb);
        };

        std::vector<color_lab> colors;
        colors.reserve(num_colors);

        for (int idx = 0; idx < num_colors; ++idx) {
            color_lab lcolor(random_number_in_interval(0.0f, 1.0f),
                             2.0f * random_number_in_interval(0.0f, 1.0f) - 1.0f,
                             2.0f * random_number_in_interval(0.0f, 1.0f) - 1.0f);

            while (!check_if_lab_in_rgb_space(lcolor)) {
                lcolor = color_lab(random_number_in_interval(0.0f, 1.0f),
                                   2.0f * random_number_in_interval(0.0f, 1.0f) - 1.0f,
                                   2.0f * random_number_in_interval(0.0f, 1.0f) - 1.0f);
            }

            colors.push_back(lcolor);
        }

        const float repulsion      = 0.3f;
        const float speed          = 0.05f;
        v8_int32_t num_steps = quality * 20;

        std::vector<force_vector_t> vectors(colors.size(), 
                                            force_vector_t(0.0f, 0.0f, 0.0f));

        while (num_steps-- > 0) {
            //
            // compute force
            for (int i = 0; i < colors.size(); ++i) {
                const color_lab& color_a = colors[i];

                for (int j = 0; j < colors.size(); ++j) {
                    const color_lab& color_b = colors[j];

                    //
                    // compute repulsion force
                    const float delta_l = color_a.L - color_b.L;
                    const float delta_a = color_a.A - color_b.A;
                    const float delta_b = color_a.B - color_b.B;

                    const float dist = sqrt(delta_l * delta_l 
                                            + delta_a * delta_a
                                            + delta_b * delta_b);

                    if (is_zero(dist)) {

                        vectors[j].dl += 0.02f - 0.04f * random_number_in_interval(0.0f, 1.0f);
                        vectors[j].da += 0.02f - 0.04f * random_number_in_interval(0.0f, 1.0f);
                        vectors[j].db += 0.02f - 0.04f * random_number_in_interval(0.0f, 1.0f);

                    } else {
                        const float force    = repulsion / dist * dist;
                        const float inv_dist = 1.0f / dist;

                        vectors[i].dl += delta_l * force * inv_dist;
                        vectors[i].da += delta_a * force * inv_dist;
                        vectors[i].db += delta_b * force * inv_dist;

                        vectors[j].dl -= delta_l * force * inv_dist;
                        vectors[j].da -= delta_a * force * inv_dist;
                        vectors[j].db -= delta_b * force * inv_dist;
                    }
                }
            }
        }

        //
        // apply force
        for (int idx = 0; idx < colors.size(); ++idx) {
            const color_lab& color = colors[idx];
            const force_vector_t& fvec = vectors[idx];

            const float displacement = speed * sqrt(fvec.dl * fvec.dl
                                                    + fvec.da * fvec.da
                                                    + fvec.db * fvec.db);

            if (is_zero(displacement)) {
                continue;
            }

            const float ratio = speed * min(0.1f, displacement) / displacement;
            const color_lab candidate_lab(color.L + fvec.dl * ratio,
                                          color.A + fvec.da * ratio,
                                          color.B + fvec.db * ratio);

            if (check_if_lab_in_rgb_space(candidate_lab)) {
                colors[idx] = candidate_lab;
            }
        }

        using namespace std;
        transform(begin(colors),
                  end(colors),
                  output_colors,
                  [](const color_lab& lab_color) {
            color_rgb rgb;
            lab_to_rgb(lab_color, &rgb);
            return rgb;
        });
    }
}

} // namespace math
} // namespace v8
