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

#include <cstdint>
#include <cmath>

namespace v8 { namespace math {

struct color_cmy {
    float Cyan;
    float Magenta;
    float Yellow;

    color_cmy() {}

    color_cmy(float cyan, float magenta, float yellow)
        :       Cyan(cyan)
            ,   Magenta(magenta)
            ,   Yellow(yellow) {}
};

struct color_yiq {
    float   Y;
    float   I;
    float   Q;

    color_yiq() {}

    color_yiq(float yval, float ival, float qval)
        :       Y(yval)
            ,   I(ival)
            ,   Q(qval) {}
};

struct color_hsv {
    float   Hue;  //! Hue.
    float   Saturation;  //! Saturation.
    float   Value;  //! Value.

    color_hsv() {}

    color_hsv(float hue, float saturation, float value)
        :       Hue(hue)
            ,   Saturation(saturation)
            ,   Value(value) {}
};

struct color_hls {
    float   Hue;  //! Hue.
    float   Lightness;  //! Lightness.
    float   Saturation;  //! Saturation.

    color_hls() {}

    color_hls(float hue, float lightness, float saturation)
        :       Hue(hue)
            ,   Lightness(lightness)
            ,   Saturation(saturation) {}
};

/*!
 *\brief Represents a 4 component (red, green, blue, alpha) 
 *       normalized color vector (128 bits).
 *\remarks Color operations (addition, substraction, 
 *         component-wise multiplication, scalar multiplication) can result
 *         in individual components having values out of the [0, 1] range, so
 *         some form of normalization should be used, to correct those situations.
 */
class color_rgb {
public :
    union {

        struct {
            float Red; /*!<< Red component intensity */
            float Green; /*!<< Green component intensity */
            float Blue; /*!<< Blue component intensity */
            float Alpha; /*!<< Alpha component (opacity) */
        };

        float components_[4];
    };

    color_rgb(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
        : Red(r), Green(g), Blue(b), Alpha(a) {}

    static color_rgb from_u32_rgba(unsigned int u32color) {
        unsigned char red = (u32color >> 24) & 0xFF;
        unsigned char green = (u32color >> 16) & 0xFF;
        unsigned char blue = (u32color >> 8) & 0xFF;
        unsigned char alpha = u32color & 0xFF;
        return color_rgb(
            static_cast<float>(red) / 255.0f, 
            static_cast<float>(green) / 255.0f, 
            static_cast<float>(blue) / 255.0f, 
            static_cast<float>(alpha) / 255.0f
            );
    }

    static color_rgb from_u32_bgra(unsigned int u32bgra) {
        unsigned char blue = (u32bgra >> 24) & 0xFF;
        unsigned char green = (u32bgra >> 16) & 0xFF;
        unsigned char red = (u32bgra >> 8) & 0xFF;
        unsigned char alpha = u32bgra & 0xFF;
        return color_rgb(
            static_cast<float>(red) / 255.0f, 
            static_cast<float>(green) / 255.0f, 
            static_cast<float>(blue) / 255.0f, 
            static_cast<float>(alpha) / 255.0f
            );
    }

    static color_rgb from_u32_argb(unsigned int u32argb) {
        unsigned char alpha = (u32argb >> 24) & 0xFF;
        unsigned char red = (u32argb >> 16) & 0xFF;
        unsigned char green = (u32argb >> 8) & 0xFF;
        unsigned char blue = u32argb & 0xFF;
        return color_rgb(
            static_cast<float>(red) / 255.0f, 
            static_cast<float>(green) / 255.0f, 
            static_cast<float>(blue) / 255.0f, 
            static_cast<float>(alpha) / 255.0f
            );
    }

    uint32_t to_uint32_rgba() const {
        unsigned int red = static_cast<unsigned int>(ceil(255 * Red)) << 24;
        unsigned int green = static_cast<unsigned int>(ceil(255 * Green)) << 16;
        unsigned int blue = static_cast<unsigned int>(ceil(255 * Blue)) << 8;
        unsigned int alpha = static_cast<unsigned int>(ceil(255 * Alpha));
        return red | green | blue | alpha;
    }

    uint32_t to_uint32_abgr() const {
        unsigned int alpha = static_cast<unsigned int>(ceil(255 * Alpha)) << 24;
        unsigned int blue = static_cast<unsigned int>(ceil(255 * Blue)) << 16;
        unsigned int green = static_cast<unsigned int>(ceil(255 * Green)) << 8;
        unsigned int red = static_cast<unsigned int>(ceil(255 * Red));
        return red | green | blue | alpha;
    }

    color_rgb& operator+=(const color_rgb& rhs) {
        Red += rhs.Red;
        Green += rhs.Green;
        Blue += rhs.Blue;
        Alpha += rhs.Alpha;
        return *this;
    }

    color_rgb& operator-=(const color_rgb& rhs) {
        Red -= rhs.Red;
        Green -= rhs.Green;
        Blue -= rhs.Blue;
        Alpha -= rhs.Alpha;
        return *this;
    }

    color_rgb& operator*=(float k) {
        Red *= k;
        Green *= k;
        Blue *= k;
        Alpha *= k;
        return *this;
    }

    /*!
     *\brief Performs a componentwise multiplication between the two colors.
     */
    color_rgb& operator*=(const color_rgb& other) {
        Red *= other.Red;
        Green *= other.Green;
        Blue *= other.Blue;
        Alpha *= other.Alpha;
        return *this;
    }

    color_rgb& operator/=(float scalar) {
        float k = 1.0f / scalar;
        Red *= k;
        Green *= k;
        Blue *= k;
        Alpha *= k;
        return *this;
    }

    static const color_rgb C_AliceBlue;

    static const color_rgb C_AntiqueWhite;

    static const color_rgb C_Aqua;

    static const color_rgb C_Aquamarine;

    static const color_rgb C_Azure;

    static const color_rgb C_Beige;

    static const color_rgb C_Bisque;

    static const color_rgb C_Black;

    static const color_rgb C_BlanchedAlmond;

    static const color_rgb C_Blue;

    static const color_rgb C_BlueViolet;

    static const color_rgb C_Brown;

    static const color_rgb C_BurlyWood;

    static const color_rgb C_CadetBlue;

    static const color_rgb C_Chartreuse;

    static const color_rgb C_Chocolate;

    static const color_rgb C_Coral;

    static const color_rgb C_CornflowerBlue;

    static const color_rgb C_Cornsilk;

    static const color_rgb C_Crimson;

    static const color_rgb C_Cyan;

    static const color_rgb C_DarkBlue;

    static const color_rgb C_DarkCyan;

    static const color_rgb C_DarkGoldenRod;

    static const color_rgb C_DarkGray;

    static const color_rgb C_DarkGrey;

    static const color_rgb C_DarkGreen;

    static const color_rgb C_DarkKhaki;

    static const color_rgb C_DarkMagenta;

    static const color_rgb C_DarkOliveGreen;

    static const color_rgb C_Darkorange;

    static const color_rgb C_DarkOrchid;

    static const color_rgb C_DarkRed;

    static const color_rgb C_DarkSalmon;

    static const color_rgb C_DarkSeaGreen;

    static const color_rgb C_DarkSlateBlue;

    static const color_rgb C_DarkSlateGray;

    static const color_rgb C_DarkSlateGrey;

    static const color_rgb C_DarkTurquoise;

    static const color_rgb C_DarkViolet;

    static const color_rgb C_DeepPink;

    static const color_rgb C_DeepSkyBlue;

    static const color_rgb C_DimGray;

    static const color_rgb C_DimGrey;

    static const color_rgb C_DodgerBlue;

    static const color_rgb C_FireBrick;

    static const color_rgb C_FloralWhite;

    static const color_rgb C_ForestGreen;

    static const color_rgb C_Fuchsia;

    static const color_rgb C_Gainsboro;

    static const color_rgb C_GhostWhite;

    static const color_rgb C_Gold;

    static const color_rgb C_GoldenRod;

    static const color_rgb C_Gray;

    static const color_rgb C_Grey;

    static const color_rgb C_Green;

    static const color_rgb C_GreenYellow;

    static const color_rgb C_HoneyDew;

    static const color_rgb C_HotPink;

    static const color_rgb C_IndianRed;

    static const color_rgb C_Indigo;

    static const color_rgb C_Ivory;

    static const color_rgb C_Khaki;

    static const color_rgb C_Lavender;

    static const color_rgb C_LavenderBlush;

    static const color_rgb C_LawnGreen;

    static const color_rgb C_LemonChiffon;

    static const color_rgb C_LightBlue;

    static const color_rgb C_LightCoral;

    static const color_rgb C_LightCyan;

    static const color_rgb C_LightGoldenRodYellow;

    static const color_rgb C_LightGray;

    static const color_rgb C_LightGrey;

    static const color_rgb C_LightGreen;

    static const color_rgb C_LightPink;

    static const color_rgb C_LightSalmon;

    static const color_rgb C_LightSeaGreen;

    static const color_rgb C_LightSkyBlue;

    static const color_rgb C_LightSlateGray;

    static const color_rgb C_LightSlateGrey;

    static const color_rgb C_LightSteelBlue;

    static const color_rgb C_LightYellow;

    static const color_rgb C_Lime;

    static const color_rgb C_LimeGreen;

    static const color_rgb C_Linen;

    static const color_rgb C_Magenta;

    static const color_rgb C_Maroon;

    static const color_rgb C_MediumAquaMarine;

    static const color_rgb C_MediumBlue;

    static const color_rgb C_MediumOrchid;

    static const color_rgb C_MediumPurple;

    static const color_rgb C_MediumSeaGreen;

    static const color_rgb C_MediumSlateBlue;

    static const color_rgb C_MediumSpringGreen;

    static const color_rgb C_MediumTurquoise;

    static const color_rgb C_MediumVioletRed;

    static const color_rgb C_MidnightBlue;

    static const color_rgb C_MintCream;

    static const color_rgb C_MistyRose;

    static const color_rgb C_Moccasin;

    static const color_rgb C_NavajoWhite;

    static const color_rgb C_Navy;

    static const color_rgb C_OldLace;

    static const color_rgb C_Olive;

    static const color_rgb C_OliveDrab;

    static const color_rgb C_Orange;

    static const color_rgb C_OrangeRed;

    static const color_rgb C_Orchid;

    static const color_rgb C_PaleGoldenRod;

    static const color_rgb C_PaleGreen;

    static const color_rgb C_PaleTurquoise;

    static const color_rgb C_PaleVioletRed;

    static const color_rgb C_PapayaWhip;

    static const color_rgb C_PeachPuff;

    static const color_rgb C_Peru;

    static const color_rgb C_Pink;

    static const color_rgb C_Plum;

    static const color_rgb C_PowderBlue;

    static const color_rgb C_Purple;

    static const color_rgb C_Red;

    static const color_rgb C_RosyBrown;

    static const color_rgb C_RoyalBlue;

    static const color_rgb C_SaddleBrown;

    static const color_rgb C_Salmon;

    static const color_rgb C_SandyBrown;

    static const color_rgb C_SeaGreen;

    static const color_rgb C_SeaShell;

    static const color_rgb C_Sienna;

    static const color_rgb C_Silver;

    static const color_rgb C_SkyBlue;

    static const color_rgb C_SlateBlue;

    static const color_rgb C_SlateGray;

    static const color_rgb C_SlateGrey;

    static const color_rgb C_Snow;

    static const color_rgb C_SpringGreen;

    static const color_rgb C_SteelBlue;

    static const color_rgb C_Tan;

    static const color_rgb C_Teal;

    static const color_rgb C_Thistle;

    static const color_rgb C_Tomato;

    static const color_rgb C_Turquoise;

    static const color_rgb C_Violet;

    static const color_rgb C_Wheat;

    static const color_rgb C_White;

    static const color_rgb C_WhiteSmoke;

    static const color_rgb C_Yellow;

    static const color_rgb C_YellowGreen;

};

inline
color_rgb
operator+(const color_rgb& lhs, const color_rgb& rhs) {
    color_rgb result(lhs);
    result += rhs;
    return result;
}

inline
color_rgb
operator-(const color_rgb& lhs, const color_rgb& rhs) {
    color_rgb result(lhs);
    result -= rhs;
    return result;
}

inline
color_rgb
operator*(const color_rgb& lhs, const color_rgb& rhs) {
    color_rgb result(lhs);
    result *= rhs;
    return result;
}

inline
color_rgb
operator*(const color_rgb& lhs, float k) {
    color_rgb result(lhs);
    result *= k;
    return result;
}

inline
color_rgb
operator*(float k, const color_rgb& rhs) {
    return rhs * k;
}

inline
color_rgb
operator/(const color_rgb& lhs, float scalar) {
    float inv = 1.0f / scalar;
    return lhs * inv;
}

inline void rgb_to_cmy(const color_rgb* rgb, color_cmy* cmy) {
    cmy->Cyan = 1.0f - rgb->Red;
    cmy->Magenta = 1.0f - rgb->Green;
    cmy->Yellow = 1.0f - rgb->Blue;
}

inline void cmy_to_rgb(const color_cmy* cmy, color_rgb* rgb) {
    rgb->Red = 1.0f - cmy->Cyan;
    rgb->Green = 1.0f - cmy->Magenta;
    rgb->Blue = 1.0f - cmy->Yellow;
}

inline void rgb_to_yiq(const color_rgb* rgb, color_yiq* yiq) {
    yiq->Y = 0.30f * rgb->Red + 0.59f * rgb->Green + 0.11f * rgb->Blue;
    yiq->I = 0.60f * rgb->Red - 0.28f * rgb->Green - 0.32f * rgb->Blue;
    yiq->Q = 0.21f * rgb->Red - 0.52f * rgb->Green + 0.31f * rgb->Blue;
}

inline void yiq_to_rgb(const color_yiq* yiq, color_rgb* rgb) {
    rgb->Red = yiq->Y + 0.6240f * yiq->Q + 0.9482f * yiq->I;
    rgb->Green = yiq->Y - 0.6398f * yiq->Q - 0.2760f * yiq->I;
    rgb->Blue = yiq->Y + 1.7298f * yiq->Q - 1.1054f * yiq->I;
}

void
rgb_to_hsv(const color_rgb* rgb, color_hsv* hsv);

void
hsv_to_rgb(const color_hsv* hsv, color_rgb* rgb);

void
rgb_to_hls(const color_rgb* rgb, color_hls* hls);

void
hls_to_rgb(const color_hls* hls, color_rgb *rgb);

} // namespace math
} // namespace v8
