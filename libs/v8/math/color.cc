#include "pch_hdr.hpp"
#include <v8/math/color.hpp>
#include <v8/math/math_utils.hpp>

const v8::math::color_rgb v8::math::color_rgb::C_AliceBlue(v8::math::color_rgb::from_u32_rgba(0xF0F8FFFF));
const v8::math::color_rgb v8::math::color_rgb::C_AntiqueWhite(v8::math::color_rgb::from_u32_rgba(0xFAEBD7FF));
const v8::math::color_rgb v8::math::color_rgb::C_Aqua(v8::math::color_rgb::from_u32_rgba(0x00FFFFFF));
const v8::math::color_rgb v8::math::color_rgb::C_Aquamarine(v8::math::color_rgb::from_u32_rgba(0x7FFFD4FF));
const v8::math::color_rgb v8::math::color_rgb::C_Azure(v8::math::color_rgb::from_u32_rgba(0xF0FFFFFF));
const v8::math::color_rgb v8::math::color_rgb::C_Beige(v8::math::color_rgb::from_u32_rgba(0xF5F5DCFF));
const v8::math::color_rgb v8::math::color_rgb::C_Bisque(v8::math::color_rgb::from_u32_rgba(0xFFE4C4FF));
const v8::math::color_rgb v8::math::color_rgb::C_Black(v8::math::color_rgb::from_u32_rgba(0x000000FF));
const v8::math::color_rgb v8::math::color_rgb::C_BlanchedAlmond(v8::math::color_rgb::from_u32_rgba(0xFFEBCDFF));
const v8::math::color_rgb v8::math::color_rgb::C_Blue(v8::math::color_rgb::from_u32_rgba(0x0000FFFF));
const v8::math::color_rgb v8::math::color_rgb::C_BlueViolet(v8::math::color_rgb::from_u32_rgba(0x8A2BE2FF));
const v8::math::color_rgb v8::math::color_rgb::C_Brown(v8::math::color_rgb::from_u32_rgba(0xA52A2AFF));
const v8::math::color_rgb v8::math::color_rgb::C_BurlyWood(v8::math::color_rgb::from_u32_rgba(0xDEB887FF));
const v8::math::color_rgb v8::math::color_rgb::C_CadetBlue(v8::math::color_rgb::from_u32_rgba(0x5F9EA0FF));
const v8::math::color_rgb v8::math::color_rgb::C_Chartreuse(v8::math::color_rgb::from_u32_rgba(0x7FFF00FF));
const v8::math::color_rgb v8::math::color_rgb::C_Chocolate(v8::math::color_rgb::from_u32_rgba(0xD2691EFF));
const v8::math::color_rgb v8::math::color_rgb::C_Coral(v8::math::color_rgb::from_u32_rgba(0xFF7F50FF));
const v8::math::color_rgb v8::math::color_rgb::C_CornflowerBlue(v8::math::color_rgb::from_u32_rgba(0x6495EDFF));
const v8::math::color_rgb v8::math::color_rgb::C_Cornsilk(v8::math::color_rgb::from_u32_rgba(0xFFF8DCFF));
const v8::math::color_rgb v8::math::color_rgb::C_Crimson(v8::math::color_rgb::from_u32_rgba(0xDC143CFF));
const v8::math::color_rgb v8::math::color_rgb::C_Cyan(v8::math::color_rgb::from_u32_rgba(0x00FFFFFF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkBlue(v8::math::color_rgb::from_u32_rgba(0x00008BFF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkCyan(v8::math::color_rgb::from_u32_rgba(0x008B8BFF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkGoldenRod(v8::math::color_rgb::from_u32_rgba(0xB8860BFF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkGray(v8::math::color_rgb::from_u32_rgba(0xA9A9A9FF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkGrey(v8::math::color_rgb::from_u32_rgba(0xA9A9A9FF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkGreen(v8::math::color_rgb::from_u32_rgba(0x006400FF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkKhaki(v8::math::color_rgb::from_u32_rgba(0xBDB76BFF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkMagenta(v8::math::color_rgb::from_u32_rgba(0x8B008BFF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkOliveGreen(v8::math::color_rgb::from_u32_rgba(0x556B2FFF));
const v8::math::color_rgb v8::math::color_rgb::C_Darkorange(v8::math::color_rgb::from_u32_rgba(0xFF8C00FF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkOrchid(v8::math::color_rgb::from_u32_rgba(0x9932CCFF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkRed(v8::math::color_rgb::from_u32_rgba(0x8B0000FF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkSalmon(v8::math::color_rgb::from_u32_rgba(0xE9967AFF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkSeaGreen(v8::math::color_rgb::from_u32_rgba(0x8FBC8FFF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkSlateBlue(v8::math::color_rgb::from_u32_rgba(0x483D8BFF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkSlateGray(v8::math::color_rgb::from_u32_rgba(0x2F4F4FFF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkSlateGrey(v8::math::color_rgb::from_u32_rgba(0x2F4F4FFF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkTurquoise(v8::math::color_rgb::from_u32_rgba(0x00CED1FF));
const v8::math::color_rgb v8::math::color_rgb::C_DarkViolet(v8::math::color_rgb::from_u32_rgba(0x9400D3FF));
const v8::math::color_rgb v8::math::color_rgb::C_DeepPink(v8::math::color_rgb::from_u32_rgba(0xFF1493FF));
const v8::math::color_rgb v8::math::color_rgb::C_DeepSkyBlue(v8::math::color_rgb::from_u32_rgba(0x00BFFFFF));
const v8::math::color_rgb v8::math::color_rgb::C_DimGray(v8::math::color_rgb::from_u32_rgba(0x696969FF));
const v8::math::color_rgb v8::math::color_rgb::C_DimGrey(v8::math::color_rgb::from_u32_rgba(0x696969FF));
const v8::math::color_rgb v8::math::color_rgb::C_DodgerBlue(v8::math::color_rgb::from_u32_rgba(0x1E90FFFF));
const v8::math::color_rgb v8::math::color_rgb::C_FireBrick(v8::math::color_rgb::from_u32_rgba(0xB22222FF));
const v8::math::color_rgb v8::math::color_rgb::C_FloralWhite(v8::math::color_rgb::from_u32_rgba(0xFFFAF0FF));
const v8::math::color_rgb v8::math::color_rgb::C_ForestGreen(v8::math::color_rgb::from_u32_rgba(0x228B22FF));
const v8::math::color_rgb v8::math::color_rgb::C_Fuchsia(v8::math::color_rgb::from_u32_rgba(0xFF00FFFF));
const v8::math::color_rgb v8::math::color_rgb::C_Gainsboro(v8::math::color_rgb::from_u32_rgba(0xDCDCDCFF));
const v8::math::color_rgb v8::math::color_rgb::C_GhostWhite(v8::math::color_rgb::from_u32_rgba(0xF8F8FFFF));
const v8::math::color_rgb v8::math::color_rgb::C_Gold(v8::math::color_rgb::from_u32_rgba(0xFFD700FF));
const v8::math::color_rgb v8::math::color_rgb::C_GoldenRod(v8::math::color_rgb::from_u32_rgba(0xDAA520FF));
const v8::math::color_rgb v8::math::color_rgb::C_Gray(v8::math::color_rgb::from_u32_rgba(0x808080FF));
const v8::math::color_rgb v8::math::color_rgb::C_Grey(v8::math::color_rgb::from_u32_rgba(0x808080FF));
const v8::math::color_rgb v8::math::color_rgb::C_Green(v8::math::color_rgb::from_u32_rgba(0x008000FF));
const v8::math::color_rgb v8::math::color_rgb::C_GreenYellow(v8::math::color_rgb::from_u32_rgba(0xADFF2FFF));
const v8::math::color_rgb v8::math::color_rgb::C_HoneyDew(v8::math::color_rgb::from_u32_rgba(0xF0FFF0FF));
const v8::math::color_rgb v8::math::color_rgb::C_HotPink(v8::math::color_rgb::from_u32_rgba(0xFF69B4FF));
const v8::math::color_rgb v8::math::color_rgb::C_IndianRed(v8::math::color_rgb::from_u32_rgba(0xCD5C5CFF));
const v8::math::color_rgb v8::math::color_rgb::C_Indigo(v8::math::color_rgb::from_u32_rgba(0x4B0082FF));
const v8::math::color_rgb v8::math::color_rgb::C_Ivory(v8::math::color_rgb::from_u32_rgba(0xFFFFF0FF));
const v8::math::color_rgb v8::math::color_rgb::C_Khaki(v8::math::color_rgb::from_u32_rgba(0xF0E68CFF));
const v8::math::color_rgb v8::math::color_rgb::C_Lavender(v8::math::color_rgb::from_u32_rgba(0xE6E6FAFF));
const v8::math::color_rgb v8::math::color_rgb::C_LavenderBlush(v8::math::color_rgb::from_u32_rgba(0xFFF0F5FF));
const v8::math::color_rgb v8::math::color_rgb::C_LawnGreen(v8::math::color_rgb::from_u32_rgba(0x7CFC00FF));
const v8::math::color_rgb v8::math::color_rgb::C_LemonChiffon(v8::math::color_rgb::from_u32_rgba(0xFFFACDFF));
const v8::math::color_rgb v8::math::color_rgb::C_LightBlue(v8::math::color_rgb::from_u32_rgba(0xADD8E6FF));
const v8::math::color_rgb v8::math::color_rgb::C_LightCoral(v8::math::color_rgb::from_u32_rgba(0xF08080FF));
const v8::math::color_rgb v8::math::color_rgb::C_LightCyan(v8::math::color_rgb::from_u32_rgba(0xE0FFFFFF));
const v8::math::color_rgb v8::math::color_rgb::C_LightGoldenRodYellow(v8::math::color_rgb::from_u32_rgba(0xFAFAD2FF));
const v8::math::color_rgb v8::math::color_rgb::C_LightGray(v8::math::color_rgb::from_u32_rgba(0xD3D3D3FF));
const v8::math::color_rgb v8::math::color_rgb::C_LightGrey(v8::math::color_rgb::from_u32_rgba(0xD3D3D3FF));
const v8::math::color_rgb v8::math::color_rgb::C_LightGreen(v8::math::color_rgb::from_u32_rgba(0x90EE90FF));
const v8::math::color_rgb v8::math::color_rgb::C_LightPink(v8::math::color_rgb::from_u32_rgba(0xFFB6C1FF));
const v8::math::color_rgb v8::math::color_rgb::C_LightSalmon(v8::math::color_rgb::from_u32_rgba(0xFFA07AFF));
const v8::math::color_rgb v8::math::color_rgb::C_LightSeaGreen(v8::math::color_rgb::from_u32_rgba(0x20B2AAFF));
const v8::math::color_rgb v8::math::color_rgb::C_LightSkyBlue(v8::math::color_rgb::from_u32_rgba(0x87CEFAFF));
const v8::math::color_rgb v8::math::color_rgb::C_LightSlateGray(v8::math::color_rgb::from_u32_rgba(0x778899FF));
const v8::math::color_rgb v8::math::color_rgb::C_LightSlateGrey(v8::math::color_rgb::from_u32_rgba(0x778899FF));
const v8::math::color_rgb v8::math::color_rgb::C_LightSteelBlue(v8::math::color_rgb::from_u32_rgba(0xB0C4DEFF));
const v8::math::color_rgb v8::math::color_rgb::C_LightYellow(v8::math::color_rgb::from_u32_rgba(0xFFFFE0FF));
const v8::math::color_rgb v8::math::color_rgb::C_Lime(v8::math::color_rgb::from_u32_rgba(0x00FF00FF));
const v8::math::color_rgb v8::math::color_rgb::C_LimeGreen(v8::math::color_rgb::from_u32_rgba(0x32CD32FF));
const v8::math::color_rgb v8::math::color_rgb::C_Linen(v8::math::color_rgb::from_u32_rgba(0xFAF0E6FF));
const v8::math::color_rgb v8::math::color_rgb::C_Magenta(v8::math::color_rgb::from_u32_rgba(0xFF00FFFF));
const v8::math::color_rgb v8::math::color_rgb::C_Maroon(v8::math::color_rgb::from_u32_rgba(0x800000FF));
const v8::math::color_rgb v8::math::color_rgb::C_MediumAquaMarine(v8::math::color_rgb::from_u32_rgba(0x66CDAAFF));
const v8::math::color_rgb v8::math::color_rgb::C_MediumBlue(v8::math::color_rgb::from_u32_rgba(0x0000CDFF));
const v8::math::color_rgb v8::math::color_rgb::C_MediumOrchid(v8::math::color_rgb::from_u32_rgba(0xBA55D3FF));
const v8::math::color_rgb v8::math::color_rgb::C_MediumPurple(v8::math::color_rgb::from_u32_rgba(0x9370D8FF));
const v8::math::color_rgb v8::math::color_rgb::C_MediumSeaGreen(v8::math::color_rgb::from_u32_rgba(0x3CB371FF));
const v8::math::color_rgb v8::math::color_rgb::C_MediumSlateBlue(v8::math::color_rgb::from_u32_rgba(0x7B68EEFF));
const v8::math::color_rgb v8::math::color_rgb::C_MediumSpringGreen(v8::math::color_rgb::from_u32_rgba(0x00FA9AFF));
const v8::math::color_rgb v8::math::color_rgb::C_MediumTurquoise(v8::math::color_rgb::from_u32_rgba(0x48D1CCFF));
const v8::math::color_rgb v8::math::color_rgb::C_MediumVioletRed(v8::math::color_rgb::from_u32_rgba(0xC71585FF));
const v8::math::color_rgb v8::math::color_rgb::C_MidnightBlue(v8::math::color_rgb::from_u32_rgba(0x191970FF));
const v8::math::color_rgb v8::math::color_rgb::C_MintCream(v8::math::color_rgb::from_u32_rgba(0xF5FFFAFF));
const v8::math::color_rgb v8::math::color_rgb::C_MistyRose(v8::math::color_rgb::from_u32_rgba(0xFFE4E1FF));
const v8::math::color_rgb v8::math::color_rgb::C_Moccasin(v8::math::color_rgb::from_u32_rgba(0xFFE4B5FF));
const v8::math::color_rgb v8::math::color_rgb::C_NavajoWhite(v8::math::color_rgb::from_u32_rgba(0xFFDEADFF));
const v8::math::color_rgb v8::math::color_rgb::C_Navy(v8::math::color_rgb::from_u32_rgba(0x000080FF));
const v8::math::color_rgb v8::math::color_rgb::C_OldLace(v8::math::color_rgb::from_u32_rgba(0xFDF5E6FF));
const v8::math::color_rgb v8::math::color_rgb::C_Olive(v8::math::color_rgb::from_u32_rgba(0x808000FF));
const v8::math::color_rgb v8::math::color_rgb::C_OliveDrab(v8::math::color_rgb::from_u32_rgba(0x6B8E23FF));
const v8::math::color_rgb v8::math::color_rgb::C_Orange(v8::math::color_rgb::from_u32_rgba(0xFFA500FF));
const v8::math::color_rgb v8::math::color_rgb::C_OrangeRed(v8::math::color_rgb::from_u32_rgba(0xFF4500FF));
const v8::math::color_rgb v8::math::color_rgb::C_Orchid(v8::math::color_rgb::from_u32_rgba(0xDA70D6FF));
const v8::math::color_rgb v8::math::color_rgb::C_PaleGoldenRod(v8::math::color_rgb::from_u32_rgba(0xEEE8AAFF));
const v8::math::color_rgb v8::math::color_rgb::C_PaleGreen(v8::math::color_rgb::from_u32_rgba(0x98FB98FF));
const v8::math::color_rgb v8::math::color_rgb::C_PaleTurquoise(v8::math::color_rgb::from_u32_rgba(0xAFEEEEFF));
const v8::math::color_rgb v8::math::color_rgb::C_PaleVioletRed(v8::math::color_rgb::from_u32_rgba(0xD87093FF));
const v8::math::color_rgb v8::math::color_rgb::C_PapayaWhip(v8::math::color_rgb::from_u32_rgba(0xFFEFD5FF));
const v8::math::color_rgb v8::math::color_rgb::C_PeachPuff(v8::math::color_rgb::from_u32_rgba(0xFFDAB9FF));
const v8::math::color_rgb v8::math::color_rgb::C_Peru(v8::math::color_rgb::from_u32_rgba(0xCD853FFF));
const v8::math::color_rgb v8::math::color_rgb::C_Pink(v8::math::color_rgb::from_u32_rgba(0xFFC0CBFF));
const v8::math::color_rgb v8::math::color_rgb::C_Plum(v8::math::color_rgb::from_u32_rgba(0xDDA0DDFF));
const v8::math::color_rgb v8::math::color_rgb::C_PowderBlue(v8::math::color_rgb::from_u32_rgba(0xB0E0E6FF));
const v8::math::color_rgb v8::math::color_rgb::C_Purple(v8::math::color_rgb::from_u32_rgba(0x800080FF));
const v8::math::color_rgb v8::math::color_rgb::C_Red(v8::math::color_rgb::from_u32_rgba(0xFF0000FF));
const v8::math::color_rgb v8::math::color_rgb::C_RosyBrown(v8::math::color_rgb::from_u32_rgba(0xBC8F8FFF));
const v8::math::color_rgb v8::math::color_rgb::C_RoyalBlue(v8::math::color_rgb::from_u32_rgba(0x4169E1FF));
const v8::math::color_rgb v8::math::color_rgb::C_SaddleBrown(v8::math::color_rgb::from_u32_rgba(0x8B4513FF));
const v8::math::color_rgb v8::math::color_rgb::C_Salmon(v8::math::color_rgb::from_u32_rgba(0xFA8072FF));
const v8::math::color_rgb v8::math::color_rgb::C_SandyBrown(v8::math::color_rgb::from_u32_rgba(0xF4A460FF));
const v8::math::color_rgb v8::math::color_rgb::C_SeaGreen(v8::math::color_rgb::from_u32_rgba(0x2E8B57FF));
const v8::math::color_rgb v8::math::color_rgb::C_SeaShell(v8::math::color_rgb::from_u32_rgba(0xFFF5EEFF));
const v8::math::color_rgb v8::math::color_rgb::C_Sienna(v8::math::color_rgb::from_u32_rgba(0xA0522DFF));
const v8::math::color_rgb v8::math::color_rgb::C_Silver(v8::math::color_rgb::from_u32_rgba(0xC0C0C0FF));
const v8::math::color_rgb v8::math::color_rgb::C_SkyBlue(v8::math::color_rgb::from_u32_rgba(0x87CEEBFF));
const v8::math::color_rgb v8::math::color_rgb::C_SlateBlue(v8::math::color_rgb::from_u32_rgba(0x6A5ACDFF));
const v8::math::color_rgb v8::math::color_rgb::C_SlateGray(v8::math::color_rgb::from_u32_rgba(0x708090FF));
const v8::math::color_rgb v8::math::color_rgb::C_SlateGrey(v8::math::color_rgb::from_u32_rgba(0x708090FF));
const v8::math::color_rgb v8::math::color_rgb::C_Snow(v8::math::color_rgb::from_u32_rgba(0xFFFAFAFF));
const v8::math::color_rgb v8::math::color_rgb::C_SpringGreen(v8::math::color_rgb::from_u32_rgba(0x00FF7FFF));
const v8::math::color_rgb v8::math::color_rgb::C_SteelBlue(v8::math::color_rgb::from_u32_rgba(0x4682B4FF));
const v8::math::color_rgb v8::math::color_rgb::C_Tan(v8::math::color_rgb::from_u32_rgba(0xD2B48CFF));
const v8::math::color_rgb v8::math::color_rgb::C_Teal(v8::math::color_rgb::from_u32_rgba(0x008080FF));
const v8::math::color_rgb v8::math::color_rgb::C_Thistle(v8::math::color_rgb::from_u32_rgba(0xD8BFD8FF));
const v8::math::color_rgb v8::math::color_rgb::C_Tomato(v8::math::color_rgb::from_u32_rgba(0xFF6347FF));
const v8::math::color_rgb v8::math::color_rgb::C_Turquoise(v8::math::color_rgb::from_u32_rgba(0x40E0D0FF));
const v8::math::color_rgb v8::math::color_rgb::C_Violet(v8::math::color_rgb::from_u32_rgba(0xEE82EEFF));
const v8::math::color_rgb v8::math::color_rgb::C_Wheat(v8::math::color_rgb::from_u32_rgba(0xF5DEB3FF));
const v8::math::color_rgb v8::math::color_rgb::C_White(v8::math::color_rgb::from_u32_rgba(0xFFFFFFFF));
const v8::math::color_rgb v8::math::color_rgb::C_WhiteSmoke(v8::math::color_rgb::from_u32_rgba(0xF5F5F5FF));
const v8::math::color_rgb v8::math::color_rgb::C_Yellow(v8::math::color_rgb::from_u32_rgba(0xFFFF00FF));
const v8::math::color_rgb v8::math::color_rgb::C_YellowGreen(v8::math::color_rgb::from_u32_rgba(0x9ACD32FF));

void v8::math::rgb_to_hsv(const color_rgb *rgb, color_hsv *hsv) {

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

void v8::math::hsv_to_rgb(const color_hsv *hsv, color_rgb *rgb) {

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
    const color_rgb *rgb,
    color_hls *hls
    ) {

    const float max = v8::math::max(v8::math::max(rgb->Red, rgb->Green),
                                    rgb->Blue);

    const float min = v8::math::min(v8::math::min(rgb->Red, rgb->Green),
                                    rgb->Blue);

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

    if (rgb->Red == max) {
        hls->Hue = (rgb->Green - rgb->Blue) / delta;
    } else if (rgb->Green == max) {
        hls->Hue = 2.0f + (rgb->Blue - rgb->Red) / delta;
    } else {
        hls->Hue = 4.0f + (rgb->Red - rgb->Green) / delta;
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
    const color_hls *hls,
    color_rgb *rgb
    ) {

    float m1 = 0.0f;
    float m2 = 0.0f;

    if (operands_le(hls->Lightness, 0.5f)) {
        m2 = hls->Lightness * (1.0f + hls->Saturation);
    } else {
        m2 = hls->Lightness + hls->Saturation - hls->Lightness * hls->Saturation;
        m1 = 2.0f * hls->Lightness - m2;
    }

    if (is_zero(hls->Saturation)) {
        rgb->Red = rgb->Green = rgb->Blue = hls->Lightness;
        return;
    }

    rgb->Red = compute_color_value(m1, m2, hls->Hue + 120.0f);
    rgb->Green = compute_color_value(m1, m2, hls->Hue);
    rgb->Blue = compute_color_value(m1, m2, hls->Hue - 120.0f);
}
