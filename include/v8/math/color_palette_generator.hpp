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

#include <vector>

#include <v8/v8.hpp>
#include <v8/base/array_proxy.hpp>
#include <v8/math/color.hpp>
#include <v8/math/math_utils.hpp>
#include <v8/math/random/random.hpp>

namespace v8 { namespace math {

class procedural_palette {
public :

    /** Fills an array with random generated colors. 
        \see http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
    */
    static void 
    gen_uniform_colors(
        v8::base::array_proxy<color_rgb>&    output_colors);

    /** Generates random colors from standard color harmonies, applying the
        specified constraint parameters.
        \see http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
    */
    static void
    gen_harmony_colors(
        const float                          offset_angle1,
        const float                          offset_angle2,
        const float                          range_angle0,
        const float                          range_angle1,
        const float                          range_angle2,
        const float                          saturation,
        const float                          luminance,
        v8::base::array_proxy<color_rgb>&    output_colors);

    /** Generates random colors from standard color harmonies, applying the
        specified constraint parameters.
        \see http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
    */
    static void
    gen_harmony_colors2(
        const float                          offset_angle1,
        const float                          offset_angle2,
        const float                          range_angle0,
        const float                          range_angle1,
        const float                          range_angle2,
        const float                          saturation,
        const float                          saturation_range,
        const float                          luminance,
        const float                          luminance_range,
        v8::base::array_proxy<color_rgb>&    output_colors);

    /** Fills an array with random colors generated from a starting color. 
        \see http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
    */
    static void
    gen_random_walk_colors(
        const color_rgb&                     base_color,
        const float                          min,
        const float                          max,
        v8::base::array_proxy<color_rgb>&    output_colors);

    /** Fills an array with random colors, generated from 3 base colors.
        \see http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
    */
    static void
    gen_random_mix_colors(
        const color_rgb&                    color1,
        const color_rgb&                    color2,
        const color_rgb&                    color3,
        const float                         grey_control,
        const v8_bool_t                     paint,
        v8::base::array_proxy<color_rgb>&   output_colors);

    /** Fills an array with random colors, generated from 3 base colors.
        \see http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
    */
    static void
    gen_random_add_colors(
        const color_rgb&                    color1,
        const color_rgb&                    color2,
        const color_rgb&                    color3,
        const float                         non_gray_bias,
        v8::base::array_proxy<color_rgb>&   output_colors);

    /** Fills an array with random colors, generated from a base color.
        \see http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
    */
    static void
    gen_colors_offset(
        const color_rgb&                    base_color,
        const float                         max_range,
        v8::base::array_proxy<color_rgb>&   output_colors);

    /** Fills an array with random colors, using a random hue.
        \see http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
        \remarks    Colors are generated in HSL space, then converted to RGB.
    */
    static void
    gen_colors_hue(
        const float                         saturation,
        const float                         luminance,
        v8::base::array_proxy<color_rgb>&   output_colors);

    /** Fills an array with random colors, using a random luminance.
        \see http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
        \remarks    Colors are generated in HSL space, then converted to RGB.
    */
    static void
    gen_colors_luminance(
        const float                         hue,
        const float                         saturation,
        v8::base::array_proxy<color_rgb>&   output_colors);

    /** Fills an array with random colors, using a random luminance and saturation.
        \see http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
        \remarks    Colors are generated in HSL space, then converted to RGB.
    */
    static void
    gen_colors_luminance_saturation(
        const float                         hue,
        v8::base::array_proxy<color_rgb>&   output_colors);

    /** Fills an array with random colors. Starts with a random hue and accumulates
        with each color the golden ratio conjugate.
        \see http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
        \remarks    Colors are generated in HSL space, then converted to RGB.
    */
    static void
    gen_colors_golden_ration_rainbow(
        const float                         saturation,
        const float                         luminance,
        v8::base::array_proxy<color_rgb>&   output_colors);

    /** Fills an array with random colors. Each color is generated by sampling
        the gradient colors with a random value that is incremented with
        the golden ratio conjugate.
        \see http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
        \remarks    Colors are generated in HSL space, then converted to RGB.
    */
    static void
    gen_colors_golden_ration_gradient(
        const float                                 saturation,
        const float                                 luminance,
        const v8::base::array_proxy<color_rgb>&     gradient,
        v8::base::array_proxy<color_rgb>&           output_colors);

    /** Fills an array with random colors. Each color is generated from a
        random hue in the [hue_min, hue_max] interval.
        \see http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
        \remarks    Colors are generated in HSL space, then converted to RGB.
    */
    static void
    gen_colors_hue_range(
        const float                                 hue_min,
        const float                                 hue_max,
        const float                                 saturation,
        const float                                 luminance,
        v8::base::array_proxy<color_rgb>&           output_colors);


    /** Fills an array with random colors.
        \see http://devmag.org.za/2012/07/29/how-to-choose-colours-procedurally-algorithms/
        \remarks    Colors are generated in HSL space, then converted to RGB.
    */
    static void
    gen_colors_jittered_rainbow(
        const float                                 hue_min,
        const float                                 hue_max,
        const float                                 saturation,
        const float                                 luminance,
        const v8_bool_t                             jitter,
        v8::base::array_proxy<color_rgb>&           output_colors);

    /** Generates a color palette.
        \see    http://tools.medialab.sciences-po.fr/iwanthue/theory.php
                algorithm explanation.
    */
    template<typename ColorChecker>
    static void 
    generate_color_palette(const ColorChecker&                  color_checker__,
                           const v8_bool_t                      force_mode,
                           const v8_int32_t                     quality,
                           const v8_bool_t                      /*higher_precision*/,
                           v8::base::array_proxy<color_rgb>&    output_colors);

private :

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

};

template<typename ColorChecker>
void 
procedural_palette::generate_color_palette(
    const ColorChecker&                  color_checker__,
    const v8_bool_t                      force_mode,
    const v8_int32_t                     quality,
    const v8_bool_t                      /*higher_precision*/,
    v8::base::array_proxy<color_rgb>&    output_colors)
{
    const v8_size_t num_colors = output_colors.length();

    if (force_mode) {

        auto check_if_lab_in_rgb_space = [&color_checker__](const color_lab& lab) -> bool {
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

        random rng;

        for (v8_size_t idx = 0; idx < num_colors; ++idx) {
            color_lab lcolor(rng.next_float(),
                             2.0f * rng.next_float() - 1.0f,
                             2.0f * rng.next_float() - 1.0f);

            while (!check_if_lab_in_rgb_space(lcolor)) {
                lcolor = color_lab(rng.next_float(),
                                   2.0f * rng.next_float() - 1.0f,
                                   2.0f * rng.next_float() - 1.0f);
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
            for (v8_size_t i = 0; i < colors.size(); ++i) {
                const color_lab& color_a = colors[i];

                for (v8_size_t j = 0; j < colors.size(); ++j) {
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

                        vectors[j].dl += 0.02f - 0.04f * rng.next_float();
                        vectors[j].da += 0.02f - 0.04f * rng.next_float();
                        vectors[j].db += 0.02f - 0.04f * rng.next_float();

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
        for (v8_size_t idx = 0; idx < colors.size(); ++idx) {
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

        for (size_t idx = 0; idx < colors.size(); ++idx) {
            const color_lab& lab_color = colors [idx];
            lab_to_rgb(lab_color, &output_colors[idx]);
        }
    }
}

} // namespace math
} // namespace v8
