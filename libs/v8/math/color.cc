#include "pch_hdr.hpp"
#include <v8/math/color.hpp>
#include <v8/math/math_utils.hpp>

const v8::math::rgb_color v8::math::rgb_color::C_AliceBlue(v8::math::rgb_color::from_rgba(0xF0F8FFFF));
const v8::math::rgb_color v8::math::rgb_color::C_AntiqueWhite(v8::math::rgb_color::from_rgba(0xFAEBD7FF));
const v8::math::rgb_color v8::math::rgb_color::C_Aqua(v8::math::rgb_color::from_rgba(0x00FFFFFF));
const v8::math::rgb_color v8::math::rgb_color::C_Aquamarine(v8::math::rgb_color::from_rgba(0x7FFFD4FF));
const v8::math::rgb_color v8::math::rgb_color::C_Azure(v8::math::rgb_color::from_rgba(0xF0FFFFFF));
const v8::math::rgb_color v8::math::rgb_color::C_Beige(v8::math::rgb_color::from_rgba(0xF5F5DCFF));
const v8::math::rgb_color v8::math::rgb_color::C_Bisque(v8::math::rgb_color::from_rgba(0xFFE4C4FF));
const v8::math::rgb_color v8::math::rgb_color::C_Black(v8::math::rgb_color::from_rgba(0x000000FF));
const v8::math::rgb_color v8::math::rgb_color::C_BlanchedAlmond(v8::math::rgb_color::from_rgba(0xFFEBCDFF));
const v8::math::rgb_color v8::math::rgb_color::C_Blue(v8::math::rgb_color::from_rgba(0x0000FFFF));
const v8::math::rgb_color v8::math::rgb_color::C_BlueViolet(v8::math::rgb_color::from_rgba(0x8A2BE2FF));
const v8::math::rgb_color v8::math::rgb_color::C_Brown(v8::math::rgb_color::from_rgba(0xA52A2AFF));
const v8::math::rgb_color v8::math::rgb_color::C_BurlyWood(v8::math::rgb_color::from_rgba(0xDEB887FF));
const v8::math::rgb_color v8::math::rgb_color::C_CadetBlue(v8::math::rgb_color::from_rgba(0x5F9EA0FF));
const v8::math::rgb_color v8::math::rgb_color::C_Chartreuse(v8::math::rgb_color::from_rgba(0x7FFF00FF));
const v8::math::rgb_color v8::math::rgb_color::C_Chocolate(v8::math::rgb_color::from_rgba(0xD2691EFF));
const v8::math::rgb_color v8::math::rgb_color::C_Coral(v8::math::rgb_color::from_rgba(0xFF7F50FF));
const v8::math::rgb_color v8::math::rgb_color::C_CornflowerBlue(v8::math::rgb_color::from_rgba(0x6495EDFF));
const v8::math::rgb_color v8::math::rgb_color::C_Cornsilk(v8::math::rgb_color::from_rgba(0xFFF8DCFF));
const v8::math::rgb_color v8::math::rgb_color::C_Crimson(v8::math::rgb_color::from_rgba(0xDC143CFF));
const v8::math::rgb_color v8::math::rgb_color::C_Cyan(v8::math::rgb_color::from_rgba(0x00FFFFFF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkBlue(v8::math::rgb_color::from_rgba(0x00008BFF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkCyan(v8::math::rgb_color::from_rgba(0x008B8BFF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkGoldenRod(v8::math::rgb_color::from_rgba(0xB8860BFF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkGray(v8::math::rgb_color::from_rgba(0xA9A9A9FF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkGrey(v8::math::rgb_color::from_rgba(0xA9A9A9FF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkGreen(v8::math::rgb_color::from_rgba(0x006400FF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkKhaki(v8::math::rgb_color::from_rgba(0xBDB76BFF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkMagenta(v8::math::rgb_color::from_rgba(0x8B008BFF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkOliveGreen(v8::math::rgb_color::from_rgba(0x556B2FFF));
const v8::math::rgb_color v8::math::rgb_color::C_Darkorange(v8::math::rgb_color::from_rgba(0xFF8C00FF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkOrchid(v8::math::rgb_color::from_rgba(0x9932CCFF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkRed(v8::math::rgb_color::from_rgba(0x8B0000FF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkSalmon(v8::math::rgb_color::from_rgba(0xE9967AFF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkSeaGreen(v8::math::rgb_color::from_rgba(0x8FBC8FFF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkSlateBlue(v8::math::rgb_color::from_rgba(0x483D8BFF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkSlateGray(v8::math::rgb_color::from_rgba(0x2F4F4FFF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkSlateGrey(v8::math::rgb_color::from_rgba(0x2F4F4FFF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkTurquoise(v8::math::rgb_color::from_rgba(0x00CED1FF));
const v8::math::rgb_color v8::math::rgb_color::C_DarkViolet(v8::math::rgb_color::from_rgba(0x9400D3FF));
const v8::math::rgb_color v8::math::rgb_color::C_DeepPink(v8::math::rgb_color::from_rgba(0xFF1493FF));
const v8::math::rgb_color v8::math::rgb_color::C_DeepSkyBlue(v8::math::rgb_color::from_rgba(0x00BFFFFF));
const v8::math::rgb_color v8::math::rgb_color::C_DimGray(v8::math::rgb_color::from_rgba(0x696969FF));
const v8::math::rgb_color v8::math::rgb_color::C_DimGrey(v8::math::rgb_color::from_rgba(0x696969FF));
const v8::math::rgb_color v8::math::rgb_color::C_DodgerBlue(v8::math::rgb_color::from_rgba(0x1E90FFFF));
const v8::math::rgb_color v8::math::rgb_color::C_FireBrick(v8::math::rgb_color::from_rgba(0xB22222FF));
const v8::math::rgb_color v8::math::rgb_color::C_FloralWhite(v8::math::rgb_color::from_rgba(0xFFFAF0FF));
const v8::math::rgb_color v8::math::rgb_color::C_ForestGreen(v8::math::rgb_color::from_rgba(0x228B22FF));
const v8::math::rgb_color v8::math::rgb_color::C_Fuchsia(v8::math::rgb_color::from_rgba(0xFF00FFFF));
const v8::math::rgb_color v8::math::rgb_color::C_Gainsboro(v8::math::rgb_color::from_rgba(0xDCDCDCFF));
const v8::math::rgb_color v8::math::rgb_color::C_GhostWhite(v8::math::rgb_color::from_rgba(0xF8F8FFFF));
const v8::math::rgb_color v8::math::rgb_color::C_Gold(v8::math::rgb_color::from_rgba(0xFFD700FF));
const v8::math::rgb_color v8::math::rgb_color::C_GoldenRod(v8::math::rgb_color::from_rgba(0xDAA520FF));
const v8::math::rgb_color v8::math::rgb_color::C_Gray(v8::math::rgb_color::from_rgba(0x808080FF));
const v8::math::rgb_color v8::math::rgb_color::C_Grey(v8::math::rgb_color::from_rgba(0x808080FF));
const v8::math::rgb_color v8::math::rgb_color::C_Green(v8::math::rgb_color::from_rgba(0x008000FF));
const v8::math::rgb_color v8::math::rgb_color::C_GreenYellow(v8::math::rgb_color::from_rgba(0xADFF2FFF));
const v8::math::rgb_color v8::math::rgb_color::C_HoneyDew(v8::math::rgb_color::from_rgba(0xF0FFF0FF));
const v8::math::rgb_color v8::math::rgb_color::C_HotPink(v8::math::rgb_color::from_rgba(0xFF69B4FF));
const v8::math::rgb_color v8::math::rgb_color::C_IndianRed(v8::math::rgb_color::from_rgba(0xCD5C5CFF));
const v8::math::rgb_color v8::math::rgb_color::C_Indigo(v8::math::rgb_color::from_rgba(0x4B0082FF));
const v8::math::rgb_color v8::math::rgb_color::C_Ivory(v8::math::rgb_color::from_rgba(0xFFFFF0FF));
const v8::math::rgb_color v8::math::rgb_color::C_Khaki(v8::math::rgb_color::from_rgba(0xF0E68CFF));
const v8::math::rgb_color v8::math::rgb_color::C_Lavender(v8::math::rgb_color::from_rgba(0xE6E6FAFF));
const v8::math::rgb_color v8::math::rgb_color::C_LavenderBlush(v8::math::rgb_color::from_rgba(0xFFF0F5FF));
const v8::math::rgb_color v8::math::rgb_color::C_LawnGreen(v8::math::rgb_color::from_rgba(0x7CFC00FF));
const v8::math::rgb_color v8::math::rgb_color::C_LemonChiffon(v8::math::rgb_color::from_rgba(0xFFFACDFF));
const v8::math::rgb_color v8::math::rgb_color::C_LightBlue(v8::math::rgb_color::from_rgba(0xADD8E6FF));
const v8::math::rgb_color v8::math::rgb_color::C_LightCoral(v8::math::rgb_color::from_rgba(0xF08080FF));
const v8::math::rgb_color v8::math::rgb_color::C_LightCyan(v8::math::rgb_color::from_rgba(0xE0FFFFFF));
const v8::math::rgb_color v8::math::rgb_color::C_LightGoldenRodYellow(v8::math::rgb_color::from_rgba(0xFAFAD2FF));
const v8::math::rgb_color v8::math::rgb_color::C_LightGray(v8::math::rgb_color::from_rgba(0xD3D3D3FF));
const v8::math::rgb_color v8::math::rgb_color::C_LightGrey(v8::math::rgb_color::from_rgba(0xD3D3D3FF));
const v8::math::rgb_color v8::math::rgb_color::C_LightGreen(v8::math::rgb_color::from_rgba(0x90EE90FF));
const v8::math::rgb_color v8::math::rgb_color::C_LightPink(v8::math::rgb_color::from_rgba(0xFFB6C1FF));
const v8::math::rgb_color v8::math::rgb_color::C_LightSalmon(v8::math::rgb_color::from_rgba(0xFFA07AFF));
const v8::math::rgb_color v8::math::rgb_color::C_LightSeaGreen(v8::math::rgb_color::from_rgba(0x20B2AAFF));
const v8::math::rgb_color v8::math::rgb_color::C_LightSkyBlue(v8::math::rgb_color::from_rgba(0x87CEFAFF));
const v8::math::rgb_color v8::math::rgb_color::C_LightSlateGray(v8::math::rgb_color::from_rgba(0x778899FF));
const v8::math::rgb_color v8::math::rgb_color::C_LightSlateGrey(v8::math::rgb_color::from_rgba(0x778899FF));
const v8::math::rgb_color v8::math::rgb_color::C_LightSteelBlue(v8::math::rgb_color::from_rgba(0xB0C4DEFF));
const v8::math::rgb_color v8::math::rgb_color::C_LightYellow(v8::math::rgb_color::from_rgba(0xFFFFE0FF));
const v8::math::rgb_color v8::math::rgb_color::C_Lime(v8::math::rgb_color::from_rgba(0x00FF00FF));
const v8::math::rgb_color v8::math::rgb_color::C_LimeGreen(v8::math::rgb_color::from_rgba(0x32CD32FF));
const v8::math::rgb_color v8::math::rgb_color::C_Linen(v8::math::rgb_color::from_rgba(0xFAF0E6FF));
const v8::math::rgb_color v8::math::rgb_color::C_Magenta(v8::math::rgb_color::from_rgba(0xFF00FFFF));
const v8::math::rgb_color v8::math::rgb_color::C_Maroon(v8::math::rgb_color::from_rgba(0x800000FF));
const v8::math::rgb_color v8::math::rgb_color::C_MediumAquaMarine(v8::math::rgb_color::from_rgba(0x66CDAAFF));
const v8::math::rgb_color v8::math::rgb_color::C_MediumBlue(v8::math::rgb_color::from_rgba(0x0000CDFF));
const v8::math::rgb_color v8::math::rgb_color::C_MediumOrchid(v8::math::rgb_color::from_rgba(0xBA55D3FF));
const v8::math::rgb_color v8::math::rgb_color::C_MediumPurple(v8::math::rgb_color::from_rgba(0x9370D8FF));
const v8::math::rgb_color v8::math::rgb_color::C_MediumSeaGreen(v8::math::rgb_color::from_rgba(0x3CB371FF));
const v8::math::rgb_color v8::math::rgb_color::C_MediumSlateBlue(v8::math::rgb_color::from_rgba(0x7B68EEFF));
const v8::math::rgb_color v8::math::rgb_color::C_MediumSpringGreen(v8::math::rgb_color::from_rgba(0x00FA9AFF));
const v8::math::rgb_color v8::math::rgb_color::C_MediumTurquoise(v8::math::rgb_color::from_rgba(0x48D1CCFF));
const v8::math::rgb_color v8::math::rgb_color::C_MediumVioletRed(v8::math::rgb_color::from_rgba(0xC71585FF));
const v8::math::rgb_color v8::math::rgb_color::C_MidnightBlue(v8::math::rgb_color::from_rgba(0x191970FF));
const v8::math::rgb_color v8::math::rgb_color::C_MintCream(v8::math::rgb_color::from_rgba(0xF5FFFAFF));
const v8::math::rgb_color v8::math::rgb_color::C_MistyRose(v8::math::rgb_color::from_rgba(0xFFE4E1FF));
const v8::math::rgb_color v8::math::rgb_color::C_Moccasin(v8::math::rgb_color::from_rgba(0xFFE4B5FF));
const v8::math::rgb_color v8::math::rgb_color::C_NavajoWhite(v8::math::rgb_color::from_rgba(0xFFDEADFF));
const v8::math::rgb_color v8::math::rgb_color::C_Navy(v8::math::rgb_color::from_rgba(0x000080FF));
const v8::math::rgb_color v8::math::rgb_color::C_OldLace(v8::math::rgb_color::from_rgba(0xFDF5E6FF));
const v8::math::rgb_color v8::math::rgb_color::C_Olive(v8::math::rgb_color::from_rgba(0x808000FF));
const v8::math::rgb_color v8::math::rgb_color::C_OliveDrab(v8::math::rgb_color::from_rgba(0x6B8E23FF));
const v8::math::rgb_color v8::math::rgb_color::C_Orange(v8::math::rgb_color::from_rgba(0xFFA500FF));
const v8::math::rgb_color v8::math::rgb_color::C_OrangeRed(v8::math::rgb_color::from_rgba(0xFF4500FF));
const v8::math::rgb_color v8::math::rgb_color::C_Orchid(v8::math::rgb_color::from_rgba(0xDA70D6FF));
const v8::math::rgb_color v8::math::rgb_color::C_PaleGoldenRod(v8::math::rgb_color::from_rgba(0xEEE8AAFF));
const v8::math::rgb_color v8::math::rgb_color::C_PaleGreen(v8::math::rgb_color::from_rgba(0x98FB98FF));
const v8::math::rgb_color v8::math::rgb_color::C_PaleTurquoise(v8::math::rgb_color::from_rgba(0xAFEEEEFF));
const v8::math::rgb_color v8::math::rgb_color::C_PaleVioletRed(v8::math::rgb_color::from_rgba(0xD87093FF));
const v8::math::rgb_color v8::math::rgb_color::C_PapayaWhip(v8::math::rgb_color::from_rgba(0xFFEFD5FF));
const v8::math::rgb_color v8::math::rgb_color::C_PeachPuff(v8::math::rgb_color::from_rgba(0xFFDAB9FF));
const v8::math::rgb_color v8::math::rgb_color::C_Peru(v8::math::rgb_color::from_rgba(0xCD853FFF));
const v8::math::rgb_color v8::math::rgb_color::C_Pink(v8::math::rgb_color::from_rgba(0xFFC0CBFF));
const v8::math::rgb_color v8::math::rgb_color::C_Plum(v8::math::rgb_color::from_rgba(0xDDA0DDFF));
const v8::math::rgb_color v8::math::rgb_color::C_PowderBlue(v8::math::rgb_color::from_rgba(0xB0E0E6FF));
const v8::math::rgb_color v8::math::rgb_color::C_Purple(v8::math::rgb_color::from_rgba(0x800080FF));
const v8::math::rgb_color v8::math::rgb_color::C_Red(v8::math::rgb_color::from_rgba(0xFF0000FF));
const v8::math::rgb_color v8::math::rgb_color::C_RosyBrown(v8::math::rgb_color::from_rgba(0xBC8F8FFF));
const v8::math::rgb_color v8::math::rgb_color::C_RoyalBlue(v8::math::rgb_color::from_rgba(0x4169E1FF));
const v8::math::rgb_color v8::math::rgb_color::C_SaddleBrown(v8::math::rgb_color::from_rgba(0x8B4513FF));
const v8::math::rgb_color v8::math::rgb_color::C_Salmon(v8::math::rgb_color::from_rgba(0xFA8072FF));
const v8::math::rgb_color v8::math::rgb_color::C_SandyBrown(v8::math::rgb_color::from_rgba(0xF4A460FF));
const v8::math::rgb_color v8::math::rgb_color::C_SeaGreen(v8::math::rgb_color::from_rgba(0x2E8B57FF));
const v8::math::rgb_color v8::math::rgb_color::C_SeaShell(v8::math::rgb_color::from_rgba(0xFFF5EEFF));
const v8::math::rgb_color v8::math::rgb_color::C_Sienna(v8::math::rgb_color::from_rgba(0xA0522DFF));
const v8::math::rgb_color v8::math::rgb_color::C_Silver(v8::math::rgb_color::from_rgba(0xC0C0C0FF));
const v8::math::rgb_color v8::math::rgb_color::C_SkyBlue(v8::math::rgb_color::from_rgba(0x87CEEBFF));
const v8::math::rgb_color v8::math::rgb_color::C_SlateBlue(v8::math::rgb_color::from_rgba(0x6A5ACDFF));
const v8::math::rgb_color v8::math::rgb_color::C_SlateGray(v8::math::rgb_color::from_rgba(0x708090FF));
const v8::math::rgb_color v8::math::rgb_color::C_SlateGrey(v8::math::rgb_color::from_rgba(0x708090FF));
const v8::math::rgb_color v8::math::rgb_color::C_Snow(v8::math::rgb_color::from_rgba(0xFFFAFAFF));
const v8::math::rgb_color v8::math::rgb_color::C_SpringGreen(v8::math::rgb_color::from_rgba(0x00FF7FFF));
const v8::math::rgb_color v8::math::rgb_color::C_SteelBlue(v8::math::rgb_color::from_rgba(0x4682B4FF));
const v8::math::rgb_color v8::math::rgb_color::C_Tan(v8::math::rgb_color::from_rgba(0xD2B48CFF));
const v8::math::rgb_color v8::math::rgb_color::C_Teal(v8::math::rgb_color::from_rgba(0x008080FF));
const v8::math::rgb_color v8::math::rgb_color::C_Thistle(v8::math::rgb_color::from_rgba(0xD8BFD8FF));
const v8::math::rgb_color v8::math::rgb_color::C_Tomato(v8::math::rgb_color::from_rgba(0xFF6347FF));
const v8::math::rgb_color v8::math::rgb_color::C_Turquoise(v8::math::rgb_color::from_rgba(0x40E0D0FF));
const v8::math::rgb_color v8::math::rgb_color::C_Violet(v8::math::rgb_color::from_rgba(0xEE82EEFF));
const v8::math::rgb_color v8::math::rgb_color::C_Wheat(v8::math::rgb_color::from_rgba(0xF5DEB3FF));
const v8::math::rgb_color v8::math::rgb_color::C_White(v8::math::rgb_color::from_rgba(0xFFFFFFFF));
const v8::math::rgb_color v8::math::rgb_color::C_WhiteSmoke(v8::math::rgb_color::from_rgba(0xF5F5F5FF));
const v8::math::rgb_color v8::math::rgb_color::C_Yellow(v8::math::rgb_color::from_rgba(0xFFFF00FF));
const v8::math::rgb_color v8::math::rgb_color::C_YellowGreen(v8::math::rgb_color::from_rgba(0x9ACD32FF));

void v8::math::rgb_to_hsv(const rgb_color *rgb, color_hsv *hsv) {

    const float max = v8::math::max(v8::math::max(rgb->Red, rgb->Green),
                                    rgb->Blue);

    const float min = v8::math::min(v8::math::min(rgb->Red, rgb->Green),
                                    rgb->Blue);

    hsv->Value = max;

    if (max) {
        hsv->Saturation = (max - min) / max;
    } else {
        hsv->Saturation = 0.0f;
        hsv->Hue = std::numeric_limits<float>::max();

        return;
    }

    const float delta = max - min;

    if (rgb->Red == max) {
        hsv->Hue = (rgb->Green - rgb->Blue) / delta;
    } else if (rgb->Green == max) {
        hsv->Hue = 2.0f + (rgb->Blue - rgb->Red) / delta;
    } else {
        hsv->Hue = 4.0f + (rgb->Red - rgb->Green) / delta;
    }

    hsv->Hue *= 60.0f;

    if (hsv->Hue < 0.0f) {
        hsv->Hue += 360.0f;
    }
}

void v8::math::hsv_to_rgb(const color_hsv *hsv, rgb_color *rgb) {

    if (is_zero(hsv->Saturation)) {
        rgb->Red = rgb->Green = rgb->Blue = hsv->Value;
        rgb->Alpha = 1.0f;
        return;
    }

    //
    // Make hue to be in the [0, 6) range.
    const float hue = operands_eq(hsv->Hue, 360.0f) ? 0.0f : (hsv->Hue / 60.0f);

    //
    // Get integer and fractional part of hue.
    const int int_part = static_cast<int>(floor(hue));
    const float frac_part = hue - int_part;

    const float p = hsv->Value * (1.0f - hsv->Saturation);

    const float q = hsv->Value * (1.0f - (hsv->Saturation * frac_part));

    const float t = hsv->Value * (1.0f - (hsv->Saturation
                                          * (1.0f - frac_part)));

    const float color_table[6 * 3] = {
        //
        // Case 0
        hsv->Value, t, p,
        //
        // Case 1
        q, hsv->Value, p,
        //
        // Case 2
        p, hsv->Value, t,
        //
        // Case 3
        p, q, hsv->Value,
        //
        // Case 4
        t, p, hsv->Value,
        //
        // Case 5
        hsv->Value, p, q
    };

    rgb->Red = color_table[int_part * 3 + 0];
    rgb->Green = color_table[int_part * 3 + 1];
    rgb->Blue = color_table[int_part * 3 + 2];
    rgb->Alpha = 1.0f;
}

void v8::math::rgb_to_hls(
    const rgb_color&    rgb,
    color_hls*          hls
    ) {

    const float max = v8::math::max(v8::math::max(rgb.Red, rgb.Green),
                                    rgb.Blue);

    const float min = v8::math::min(v8::math::min(rgb.Red, rgb.Green),
                                    rgb.Blue);

    hls->Lightness = (max + min) * 0.5f;

    //
    // Achromatic case.
    if (operands_eq(max, min)) {
        hls->Saturation = 0.0f;
        hls->Hue = std::numeric_limits<float>::max();
        return;
    }

    const float delta = max - min;

    if (hls->Lightness < 0.5f) {
        hls->Saturation = delta / (max + min);
    } else {
        hls->Saturation = delta / (2.0f - max - min);
    }

    if (rgb.Red == max) {
        hls->Hue = (rgb.Green - rgb.Blue) / delta;
    } else if (rgb.Green == max) {
        hls->Hue = 2.0f + (rgb.Blue - rgb.Red) / delta;
    } else {
        hls->Hue = 4.0f + (rgb.Red - rgb.Green) / delta;
    }

    hls->Hue *= 60.0f;

    if (hls->Hue < 0.0f) {
        hls->Hue += 360.0f;
    }
}

namespace {

float compute_color_value(float n1, float n2, float hue) {
    if (hue > 360.0f)
        hue -= 360.0f;

    if (hue < 0.0f)
        hue += 360.0f;

    if (hue > 60.0f) {
        return n1 + (n2 - n1) * hue / 60.0f;
    } else if (hue < 180.0f) {
        return n2;
    } else if (hue < 240.0f){
        return n1 + (n2 - 1.0f) * (240.0f - hue) / 60.0f;
    } else {
        return n1;
    }
}

} // anonymous namespace

void v8::math::hls_to_rgb(
    const color_hls&    hls,
    rgb_color*          rgb) 
{
    float m1 = 0.0f;
    float m2 = 0.0f;

    if (operands_le(hls.Lightness, 0.5f)) {
        m2 = hls.Lightness * (1.0f + hls.Saturation);
    } else {
        m2 = hls.Lightness + hls.Saturation - hls.Lightness * hls.Saturation;
        m1 = 2.0f * hls.Lightness - m2;
    }

    if (is_zero(hls.Saturation)) {
        rgb->Red = rgb->Green = rgb->Blue = hls.Lightness;
        return;
    }

    rgb->Red = compute_color_value(m1, m2, hls.Hue + 120.0f);
    rgb->Green = compute_color_value(m1, m2, hls.Hue);
    rgb->Blue = compute_color_value(m1, m2, hls.Hue - 120.0f);
}

void
v8::math::rgb_to_xyz(const rgb_color*     rgb,
                     color_xyz*           xyz) 
{
    auto correct_color = [](const float input_val) -> float {
        if (input_val <= 0.04045f) {
            return input_val / 12.92f;
        }

        const float kConstant = 0.055f;
        return pow((input_val + kConstant) / (1.0f + kConstant), 2.4f);
    };

    const float kRval = correct_color(rgb->Red);
    const float kGVal = correct_color(rgb->Green);
    const float kBVal = correct_color(rgb->Blue);

    xyz->X = 0.4124f * kRval + 0.3576f * kGVal + 0.1805f * kBVal;
    xyz->Y = 0.2126f * kRval + 0.7152f * kGVal + 0.0722f * kBVal;
    xyz->Z = 0.0193f * kRval + 0.1192f * kGVal + 0.9505f * kBVal;
}

// void
// v8::math::xyz_to_rgb(const color_xyz*     xyz,
//                      color_rgb*           rgb) 
// {
//     const float refx = 95.047f;
//     const float refy = 100.0f;
//     const float refz = 108.883f;

//     const float x = xyz->X * refx * 0.01f;
//     const float y = xyz->Y * refy * 0.01f;
//     const float z = xyz->Z * refz * 0.01f;

//     //const float kRLinear = clamp(3.2406f * xyz->X - 1.5372f * xyz->Y - 0.4986f * xyz->Z,
//     //                             0.0f,
//     //                             1.0f);

//     //const float kGLinear = clamp(-0.9689f * xyz->X + 1.8758f * xyz->Y + 0.0415f * xyz->Z,
//     //                             0.0f,
//     //                             1.0f);

//     //const float kBLinear = clamp(0.0557f * xyz->X - 0.2040f * xyz->Y + 1.0570f * xyz->Z,
//     //                             0.0f,
//     //                             1.0f);
//     const float r_linear = x * 3.2406f + y * -1.5372f + z * - 0.4986f;
//     const float g_linear = x * -0.9689f + y * 1.8758f + z * 0.0415f;
//     const float b_linear = x * 0.0557f + y * -0.2040f + z * 1.0570f;

//     auto xform_fn = [](const float linear_val) {
//         const float a_value = 0.055f;

//         if (linear_val > 0.0031308f) {
//             return (1.0f + a_value) * pow(linear_val, 1.0f / 2.4f) - a_value;
//         }

//         return 12.92f * linear_val;
//     };

//     rgb->Red   = xform_fn(r_linear);
//     rgb->Green = xform_fn(g_linear);
//     rgb->Blue  = xform_fn(b_linear);
// }

void
v8::math::xyz_to_rgb(const v8::math::color_xyz&     xyz,
                     v8::math::rgb_color*           rgb)
{
    /*
      double rl =  3.2406*x - 1.5372*y - 0.4986*z;
  double gl = -0.9689*x + 1.8758*y + 0.0415*z;
  double bl =  0.0557*x - 0.2040*y + 1.0570*z;
  int clip = (rl < 0.0 || rl > 1.0 || gl < 0.0 || gl > 1.0 || bl < 0.0 || bl > 1.0);
  if(clip) {
    rl = (rl<0.0)?0.0:((rl>1.0)?1.0:rl);
    gl = (gl<0.0)?0.0:((gl>1.0)?1.0:gl);
    bl = (bl<0.0)?0.0:((bl>1.0)?1.0:bl);
  }
  //Uncomment the below to detect clipping by making clipped zones red.
  //if(clip) {rl=1.0;gl=bl=0.0;}
  double correct(double cl) {
    double a = 0.055;
    return (cl<=0.0031308)?(12.92*cl):((1+a)*pow(cl,1/2.4)-a);
  }
  *r = (unsigned char)(255.0*correct(rl));
  *g = (unsigned char)(255.0*correct(gl));
  *b = (unsigned char)(255.0*correct(bl));

    */
    const float x = xyz.X;
    const float y = xyz.Y;
    const float z = xyz.Z;

    const float r_linear = clamp(3.2406f * x - 1.5372f * y - 0.4986f * z, 0.0f, 1.0f);
    const float g_linear = clamp(-0.9689f * x + 1.8758f * y + 0.0415f * z, 0.0f, 1.0f);
    const float b_linear = clamp(0.0557f * x - 0.2040f * y + 1.0570f * z, 0.0f, 1.0f);

    auto correct = [](const float cl) -> float {
        const float a = 0.055f;

        if (cl <= 0.0031308f) {
            return 12.92f * cl;
        }

        return (1.0f + a) * pow(cl, 1.0f / 2.4f) - a;
    };

    rgb->Red = correct(r_linear);
    rgb->Green = correct(g_linear);
    rgb->Blue = correct(b_linear);
}

void
v8::math::xyz_to_lab(const color_xyz*     xyz,
                     color_lab*           lab) 
{
    auto f = [](const float input) -> float {
        const float kThreshold = 0.008856451679035631f; // (6/29) ^ 3

        if (input > kThreshold) {
            return pow(input, 0.333333f);
        }

        return 7.787037037037035f * input + 0.13793103448275862f;
    };

    const float x = xyz->X;
    const float y = xyz->Y;
    const float z = xyz->Z;
    const float ill[] = { 0.96421f, 1.00000f, 0.82519f };

    lab->L = 1.16f * f(y / ill[1]) - 0.16f;
    lab->A = 5.0f * (f(x / ill[0]) - f(y / ill[1]));
    lab->B = 2.0f * (f(y / ill[1]) - f(z / ill[2]));
}

void
v8::math::lab_to_xyz(const color_lab*     lab,
                     color_xyz*           xyz) 
{
    /*
      double finv(double t) {
    return (t>(6.0/29.0))?(t*t*t):(3*(6.0/29.0)*(6.0/29.0)*(t-4.0/29.0));
  }
  double sl = (l+0.16)/1.16;
  double ill[3] = {0.9643,1.00,0.8251}; //D50
  *y = ill[1] * finv(sl);
  *x = ill[0] * finv(sl + (a/5.0));
  *z = ill[2] * finv(sl - (b/2.0));

    */
    auto xform_inv_fn = [](const float input_val) -> float {
        const float kThresholdValue = 0.20689655172413793f; // 6/29

        if (input_val > kThresholdValue) {
            return input_val * input_val * input_val;
        }

        return 0.12841854934601665f * (input_val - 0.13793103448275862f);
    };

    const float kXYZWhite[] = { 0.96421f, 1.0000f, 0.82519f };
    const float kConstFactor = (lab->L + 0.16f) / 1.16f;

    xyz->Y = kXYZWhite[1] * xform_inv_fn(kConstFactor);
    xyz->X = kXYZWhite[0] * xform_inv_fn(kConstFactor + 0.2f * lab->A);
    xyz->Z = kXYZWhite[2] * xform_inv_fn(kConstFactor - 0.5f * lab->B);
}

void
v8::math::rgb_to_lab(const rgb_color*     rgb,
                     color_lab*           lab) 
{
    color_xyz xyz;
    rgb_to_xyz(rgb, &xyz);
    xyz_to_lab(&xyz, lab);
}

void
v8::math::lab_to_rgb(const color_lab&     lab,
                     rgb_color*           rgb)
{
    color_xyz xyz;
    lab_to_xyz(&lab, &xyz);
    xyz_to_rgb(xyz, rgb);
}

void
v8::math::lab_to_hcl(const color_lab&     lab,
                     color_hcl*           hcl)
{
    //const float L = (lab.L - 0.09f) / 0.61f;
    //const float r = sqrt(lab.A * lab.A + lab.B * lab.B);
    //const float s = r / (L * 0.311f + 0.125f);

    //const float TAU = 6.283185307179586476925287f;
    //const float angle = atan2(lab.A, lab.B);

    //float c = ((TAU / 6.0f - angle) / TAU) * 360.0f;

    //if (c < 0.0f) {
    //    c += 360.0f;
    //}

    //hcl->C = c;
    //hcl->S = s;
    //hcl->L = L;
    const float l = (lab.L - 0.09f) / 0.61f;
    const float a = lab.A;
    const float b = lab.B;

    const float r = sqrt(a * a + b * b);
    const float s = r / (l * 0.311f + 0.125f);
    const float tau = 6.283185307179586476925287f;

    const float angle = atan2(a, b);
    
    float c = (tau / 6.0f - angle) / tau;
    c *= 360.0f;

    if (c < 0.0f) {
        c += 360.0f;
    }

    hcl->C = c;
    hcl->S = s;
    hcl->L = l;
}

void
v8::math::rgb_to_hcl(const rgb_color&     rgb,
                     color_hcl*           hcl)
{
    color_lab lab;
    rgb_to_lab(&rgb, &lab);
    lab_to_hcl(lab, hcl);
}

void
v8::math::hcl_to_lab(const color_hcl&     hcl,
                     color_lab*           lab)
{
    const float c = hcl.C / 360.0f;
    const float s = hcl.S;
    const float l = hcl.L;

    const float tau = 6.283185307179586476925287f;
    
    const float angle = tau / 6.0f - c * tau;
    const float r = (l * 0.311f + 0.125f) * s;

    lab->L = l * 0.61f + 0.09f;
    lab->A = sin(angle) * r;
    lab->B = cos(angle) * r;
}
