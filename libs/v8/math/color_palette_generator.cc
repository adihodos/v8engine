#include "pch_hdr.hpp"
#include <cmath>
#include "v8/math/color.hpp"
#include "v8/math/math_utils.hpp"
#include "v8/math/random/random.hpp"

#include "v8/math/color_palette_generator.hpp"

namespace {

v8::math::color_rgb
random_mix_hsl(
    const v8::math::color_rgb&          color1,
    const v8::math::color_rgb&          color2,
    const v8::math::color_rgb&          color3,
    const float                         grey_control)
{
    v8::math::random rng;

    const v8_int_t rand_index = static_cast<v8_int_t>(rng.next(256u)) % 3;

    float mix_ratio1 = (rand_index == 0) ? rng.next_float() * grey_control : rng.next_float();
    float mix_ratio2 = (rand_index == 1) ? rng.next_float() * grey_control : rng.next_float();
    float mix_ratio3 = (rand_index == 2) ? rng.next_float() * grey_control : rng.next_float();

    const float inv_sum = 1.0f / (mix_ratio1 + mix_ratio2 + mix_ratio3);

    mix_ratio1 *= inv_sum;
    mix_ratio2 *= inv_sum;
    mix_ratio3 *= inv_sum;

    v8::math::color_hls hls1(color1);
    v8::math::color_hls hls2(color2);
    v8::math::color_hls hls3(color3);

    v8::math::color_hls hls(
        mix_ratio1 * hls1.Hue + mix_ratio2 * hls2.Hue + mix_ratio3 * hls3.Hue,
        mix_ratio2 * hls1.Lightness + mix_ratio2 * hls2.Lightness + mix_ratio3 * hls3.Lightness,
        mix_ratio1 * hls1.Saturation + mix_ratio2 * hls2.Saturation + mix_ratio3 * hls3.Saturation);

    return v8::math::color_rgb(hls);
}

v8::math::color_rgb
random_mix_paint(
    const v8::math::color_rgb&      color1,
    const v8::math::color_rgb&      color2,
    const v8::math::color_rgb&      color3,
    const float                     grey_control)
{
    v8::math::random rng;

    const v8_int_t rand_index = static_cast<v8_int_t>(rng.next(256u)) % 3;

    float mix_ratio1 = (rand_index == 0) ? rng.next_float() * grey_control : rng.next_float();
    float mix_ratio2 = (rand_index == 1) ? rng.next_float() * grey_control : rng.next_float();
    float mix_ratio3 = (rand_index == 2) ? rng.next_float() * grey_control : rng.next_float();

    const float inv_sum = 1.0f / (mix_ratio1 + mix_ratio2 + mix_ratio3);

    mix_ratio1 *= inv_sum;
    mix_ratio2 *= inv_sum;
    mix_ratio3 *= inv_sum;

    return v8::math::color_rgb(1.0f - (mix_ratio1 * (1.0f - color1.Red)
                                       + mix_ratio2 * (1.0f - color2.Red)
                                       + mix_ratio3 * (1.0f - color3.Red)),
                               1.0f - (mix_ratio1 * (1.0f - color1.Blue)
                                       + mix_ratio2 * (1.0f - color2.Blue)
                                       + mix_ratio3 * (1.0f - color3.Blue)),
                               1.0f - (mix_ratio1 * (1.0f - color1.Green)
                                       + mix_ratio2 * (1.0f - color2.Green)
                                       + mix_ratio3 * (1.0f - color3.Green)));
}

v8::math::color_rgb
random_add(
    const v8::math::color_rgb&      color1,
    const v8::math::color_rgb&      color2,
    const v8::math::color_rgb&      color3,
    const float                     non_gray_bias)
{
    v8::math::random rng;
    const v8_int_t rand_index = static_cast<v8_int_t>(rng.next(256u)) % 3;

    float r1 = rng.next_float();
    if (rand_index == 0) {
        r1 *= non_gray_bias;
    }

    float r2 = rng.next_float();
    if (rand_index == 1) {
        r2 *= non_gray_bias;
    }

    float r3 = rng.next_float();
    if (rand_index == 3) {
        r3 *= non_gray_bias;
    }

    const float p = 20.0f;
    const float inv_sum = 1.0f / pow(pow(r1, p) + pow(r2, p) + pow(r3, p), 1.0f / p);
    
    r1 *= inv_sum;
    r2 *= inv_sum;
    r3 *= inv_sum;

    using v8::math::clamp;

    const float red = clamp(r1 * color1.Red + r2 * color2.Red + r3 * color3.Red, 
                            0.0f, 1.0f);

    const float green = clamp(r1 * color1.Green + r2 * color2.Green + r3 * color3.Green,
                              0.0f,
                              1.0f);

    const float blue = clamp(r1 * color1.Blue + r2 * color2.Blue + r3 * color3.Green,
                             0.0f,
                             1.0f);

    return v8::math::color_rgb(red, green, blue);
}

v8::math::color_rgb
SampleLinearGradient(
    const v8::base::array_proxy<v8::math::color_rgb>&       colors,
    const float                                             t)
{
    const v8_int_t colorCount = static_cast<v8_int_t>(colors.length());
    const v8_size_t leftIndex = static_cast<v8_size_t>(t * colorCount);

    const float cellRange = 1.0f / colorCount;
    const float alpha = (t - leftIndex * cellRange) / cellRange;

    const v8::math::color_rgb& leftColor = colors[leftIndex];
    const v8::math::color_rgb& rightColor = colors[(leftIndex + 1) % colorCount];

    return v8::math::color_rgb(
        leftColor.Red * (1.0f - alpha) + rightColor.Red * alpha,
        leftColor.Green * (1.0f - alpha) + rightColor.Green * alpha,
        leftColor.Blue * (1.0f - alpha) + rightColor.Blue * alpha);
}

}

void 
v8::math::procedural_palette::gen_uniform_colors(
    v8::base::array_proxy<color_rgb>& output_colors) {

    random rng;

    for (v8_size_t idx = 0; idx < output_colors.length(); ++idx) {
        output_colors[idx] = color_rgb(rng.next_float(),
                                       rng.next_float(),
                                       rng.next_float());
    }
}

void
v8::math::procedural_palette::gen_harmony_colors(
    const float                          offset_angle1,
    const float                          offset_angle2,
    const float                          range_angle0,
    const float                          range_angle1,
    const float                          range_angle2,
    const float                          saturation,
    const float                          luminance,
    v8::base::array_proxy<color_rgb>&    output_colors)
{
    const float reference_angle = random_number_in_interval(0.0f, 1.0f) * 360.0f;
    random rng;

    for (v8_size_t idx = 0; idx < output_colors.length(); ++idx) {
        float random_angle = rng.next_float_closed()
                             * (range_angle0 + range_angle1 + range_angle2);

        if (random_angle > range_angle0) {
            if (random_angle < (range_angle0 + range_angle1)) {
                random_angle += offset_angle1;
            } else {
                random_angle += offset_angle2;
            }
        }

        const float hue = fmod((reference_angle + random_angle) / 360.0f, 1.0f);
        color_hls hls(hue, luminance, saturation);
        hls_to_rgb(hls, &output_colors[idx]);
    }
}

void
v8::math::procedural_palette::gen_harmony_colors2(
    const float                          offset_angle1,
    const float                          offset_angle2,
    const float                          range_angle0,
    const float                          range_angle1,
    const float                          range_angle2,
    const float                          saturation,
    const float                          saturation_range,
    const float                          luminance,
    const float                          luminance_range,
    v8::base::array_proxy<color_rgb>&    output_colors)
{
    random rng;

    const float reference_angle = rng.next_float() * 360.0f;

    for (v8_size_t idx = 0; idx < output_colors.length(); ++idx) {
        float rand_angle = rng.next_float() 
                           * (range_angle0 + range_angle1 + range_angle2);

        if (rand_angle > range_angle0) {
            if (rand_angle < (range_angle0 + range_angle1)) {
                rand_angle += offset_angle1;
            } else {
                rand_angle += offset_angle2;
            }
        }

        const float new_saturation = 
            saturation + (rng.next_float() - 0.5f) * saturation_range;

        const float new_luminance = 
            luminance + (rng.next_float() - 0.5f) * luminance_range;

        const float angle = fmod((reference_angle + rand_angle) / 360.0f, 1.0f);

        color_hls hls(angle, new_saturation, new_luminance);
        hls_to_rgb(hls, &output_colors[idx]);
    }
}


void
v8::math::procedural_palette::gen_random_walk_colors(
    const color_rgb&                        base_color,
    const float                             min,
    const float                             max,
    v8::base::array_proxy<color_rgb>&       output_colors) 
{
    random rng;

    for (size_t idx = 0; idx < output_colors.length(); ++idx) {
        const float r_sgn = rng.next(0, 2) % 2 == 0 ? 1.0f : -1.0f;
        const float g_sgn = rng.next(0, 2) % 2 == 0 ? 1.0f : -1.0f;
        const float b_sgn = rng.next(0, 2) % 2 == 0 ? 1.0f : -1.0f;

        const float red = clamp(base_color.Red 
                                + r_sgn * rng.next(min, max),
                                0.0f,
                                1.0f);

        const float green = clamp(base_color.Green
                                  + g_sgn * rng.next(min, max),
                                  0.0f,
                                  1.0f);

        const float blue = clamp(base_color.Blue
                                 + b_sgn * rng.next(min, max),
                                 0.0f,
                                 1.0f);

        output_colors[idx].Red = red;
        output_colors[idx].Green = green;
        output_colors[idx].Blue = blue;
        output_colors[idx].Alpha = 1.0f;
    }
}

void
v8::math::procedural_palette::gen_random_mix_colors(
    const color_rgb&                    color1,
    const color_rgb&                    color2,
    const color_rgb&                    color3,
    const float                         grey_control,
    const v8_bool_t                     paint,
    v8::base::array_proxy<color_rgb>&   output_colors)
{
    for (v8_size_t idx = 0; idx < output_colors.length(); ++idx) {
        if (paint) {
            output_colors[idx] = random_mix_hsl(color1, color2, color3, grey_control);
        } else {
            output_colors[idx] = random_mix_paint(color1, color2, color3, grey_control);
        }
    }
}

void
v8::math::procedural_palette::gen_random_add_colors(
    const color_rgb&                    color1,
    const color_rgb&                    color2,
    const color_rgb&                    color3,
    const float                         non_gray_bias,
    v8::base::array_proxy<color_rgb>&   output_colors)
{
    for (v8_size_t idx = 0; idx < output_colors.length(); ++idx) {
        output_colors[idx] = random_add(color1, color2, color3, non_gray_bias);
    }
}

void
v8::math::procedural_palette::gen_colors_offset(
    const color_rgb&                    base_color,
    const float                         max_range,
    v8::base::array_proxy<color_rgb>&   output_colors)
{
    random rng;

    for (v8_size_t idx = 0; idx < output_colors.length(); ++idx) {
        const float r = base_color.Red + max_range * (rng.next_float() * 2.0f - 1.0f);
        const float g = base_color.Green + max_range * (rng.next_float() * 2.f - 1.f);
        const float b = base_color.Blue + max_range * (rng.next_float() * 2.f - 1.f);

        output_colors[idx].Red = clamp(r, 0.f, 1.f);
        output_colors[idx].Green = clamp(g, 0.f, 1.f);
        output_colors[idx].Blue = clamp(b, 0.f, 1.f);
    }
}

void
v8::math::procedural_palette::gen_colors_hue(
    const float                         saturation,
    const float                         luminance,
    v8::base::array_proxy<color_rgb>&   output_colors)
{
    random rng;

    for (v8_size_t idx = 0; idx < output_colors.length(); ++idx) {
        color_hls hsl(rng.next_float(), luminance, saturation);
        output_colors[idx].from_hsl(hsl);
    }
}

void
v8::math::procedural_palette::gen_colors_luminance(
    const float                         hue,
    const float                         saturation,
    v8::base::array_proxy<color_rgb>&   output_colors)
{
    random rng;

    for (v8_size_t idx = 0; idx < output_colors.length(); ++idx) {
        color_hls hsl(hue, rng.next_float(), saturation);
        output_colors[idx].from_hsl(hsl);
    }
}

void
v8::math::procedural_palette::gen_colors_luminance_saturation(
    const float                         hue,
    v8::base::array_proxy<color_rgb>&   output_colors)
{
    random rng;

    for (v8_size_t idx = 0; idx < output_colors.length(); ++idx) {
        color_hls hsl(hue, rng.next_float(), rng.next_float());
        output_colors[idx].from_hsl(hsl);
    }
}

void
v8::math::procedural_palette::gen_colors_golden_ration_rainbow(
    const float                         saturation,
    const float                         luminance,
    v8::base::array_proxy<color_rgb>&   output_colors)
{
    const float goldenRatioConjugate = 0.618033988749895f;

    random rng;

    float currentHue = rng.next_float();

    for (v8_size_t idx = 0; idx < output_colors.length(); ++idx) {
        color_hls hsl(currentHue, luminance, saturation);

        output_colors[idx].from_hsl(hsl);

        currentHue += goldenRatioConjugate;
        currentHue = fmod(currentHue, 1.f);
    }
}

void
v8::math::procedural_palette::gen_colors_golden_ration_gradient(
    const float                                 saturation,
    const float                                 luminance,
    const v8::base::array_proxy<color_rgb>&     gradient,
    v8::base::array_proxy<color_rgb>&           output_colors)
{
    const float goldenRatioConjugate = 0.618033988749895f;

    random rng;
    float currentHue = rng.next_float();

    for (v8_size_t idx = 0; idx < output_colors.length(); ++idx) {
        color_hls hsl(currentHue, luminance, saturation);

        output_colors[idx] = SampleLinearGradient(gradient, currentHue);

        currentHue += goldenRatioConjugate;
        currentHue = fmod(currentHue, 1.f);
    }
}

void
v8::math::procedural_palette::gen_colors_hue_range(
    const float                                 hue_min,
    const float                                 hue_max,
    const float                                 saturation,
    const float                                 luminance,
    v8::base::array_proxy<color_rgb>&           output_colors)
{
    float hueRange = hue_max - hue_min;

    if (hueRange < 0.f) {
        hueRange += 1.f;
    }

    random rng;
    for (v8_size_t idx = 0; idx < output_colors.length(); ++idx) {
        float newHue = hueRange * rng.next_float() + hue_min;

        if (newHue > 1.f) {
            newHue -= 1.f;
        }

        color_hls hsl(newHue, luminance, saturation);
        output_colors[idx].from_hsl(hsl);
    }
}

void
v8::math::procedural_palette::gen_colors_jittered_rainbow(
    const float                                 hue_min,
    const float                                 hue_max,
    const float                                 saturation,
    const float                                 luminance,
    const v8_bool_t                             jitter,
    v8::base::array_proxy<color_rgb>&           output_colors)
{
    float hueRange = hue_max - hue_min;

    if (hueRange < 1.f) {
        hueRange += 1.f;
    }

    const v8_size_t colorCount = output_colors.length();
    random rng;

    const float cellRange = hueRange / static_cast<float>(colorCount);
    const float cellOffset = rng.next_float() * cellRange;

    for (v8_size_t idx = 0; idx < output_colors.length(); ++idx) {
        float newHue;

        if (jitter) {
            newHue = cellRange * idx + rng.next_float() * cellRange + hue_min;
        } else {
            newHue = cellRange * idx + cellOffset + hue_min;
        }

        if (newHue > 1.f) {
            newHue -= 1.f;
        }

        color_hls hsl(newHue, luminance, saturation);
        output_colors[idx].from_hsl(hsl);
    }
}