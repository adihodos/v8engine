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

#include <cmath>
#include <v8/v8.hpp>

namespace v8 { namespace math {

class   rgb_color;
struct  color_hsv;
struct  color_hls;
struct  color_xyz;
struct  color_lab;
struct  color_hcl;

void
rgb_to_hsv(const rgb_color* rgb, color_hsv* hsv);

void
hsv_to_rgb(const color_hsv* hsv, rgb_color* rgb);

void
rgb_to_hls(const rgb_color& rgb, color_hls* hls);

void
hls_to_rgb(const color_hls& hls, rgb_color *rgb);

void
rgb_to_xyz(const rgb_color*     rgb,
           color_xyz*           xyz);

void
rgb_to_lab(const rgb_color*     rgb,
           color_lab*           lab);

void
rgb_to_hcl(const rgb_color&     rgb,
           color_hcl*           lab);

void
xyz_to_rgb(const color_xyz&     xyz,
           rgb_color*           rgb);

void
xyz_to_lab(const color_xyz*     xyz,
           color_lab*           lab);

void
lab_to_xyz(const color_lab*     lab,
           color_xyz*           xyz);

void
lab_to_rgb(const color_lab&     lab,
           rgb_color*           rgb);

void
hcl_to_lab(const color_hcl&     hcl,
           color_lab*           lab);

void
lab_to_hcl(const color_lab&     lab,
           color_hcl*           hcl);


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

    color_hls(const float hue, const float lightness, const float saturation)
        :       Hue(hue)
            ,   Lightness(lightness)
            ,   Saturation(saturation) {}

    color_hls(const rgb_color& rgb) {
        rgb_to_hls(rgb, this);
    }
};

struct color_xyz {
    float   X;
    float   Y;
    float   Z;

    color_xyz() {}

    color_xyz(const float x, const float y, const float z)
        :       X(x)
            ,   Y(y)
            ,   Z(z)
    {}
};

struct color_lab {
    union {
        struct {
            float   L;
            float   A;
            float   B;
        };

        float Elements[3];
    };

    color_lab() {}

    color_lab(const float l, const float a, const float b)
        :       L(l)
            ,   A(a)
            ,   B(b)
    {}
};

struct color_hcl {
    union {
        struct {
            float   C;
            float   S;
            float   L;
        };

        float Elements[3];
    };

    color_hcl() {}

    color_hcl(const float chroma, const float sat, const float lum)
        :       C(chroma)
            ,   S(sat)
            ,   L(lum)
    {}

    color_hcl(const rgb_color& rgb) {
        from_rgb(rgb);
    }

    color_hcl& from_rgb(const rgb_color& rgb) {
        rgb_to_hcl(rgb, this);
        return *this;
    }
};

/*!
 *\brief Represents a 4 component (red, green, blue, alpha) 
 *       normalized color vector (128 bits).
 *\remarks Color operations (addition, substraction, 
 *         component-wise multiplication, scalar multiplication) can result
 *         in individual components having values out of the [0, 1] range, so
 *         some form of normalization should be used, to correct those situations.
 */
class rgb_color {
public :
    union {

        struct {
            float Red; /*!<< Red component intensity */
            float Green; /*!<< Green component intensity */
            float Blue; /*!<< Blue component intensity */
            float Alpha; /*!<< Alpha component (opacity) */
        };

        float Elements[4];
    };

    rgb_color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
        : Red(r), Green(g), Blue(b), Alpha(a) {}

    explicit rgb_color(const color_hls& hls) {
        from_hsl(hls);
    }

    explicit rgb_color(const color_xyz& xyz) {
        from_xyz(xyz);
    }

    explicit rgb_color(const color_lab& lab) {
        from_lab(lab);
    }

    rgb_color& from_hsl(const color_hls& hsl) {
        hls_to_rgb(hsl, this);
        return *this;
    }

    rgb_color& from_xyz(const color_xyz& xyz) {
        xyz_to_rgb(xyz, this);
        return *this;
    }

    rgb_color& from_lab(const color_lab& lab) {
        lab_to_rgb(lab, this);
        return *this;
    }

    static rgb_color from_rgba(const v8_uint32_t u32color) {
        unsigned char red = (u32color >> 24) & 0xFF;
        unsigned char green = (u32color >> 16) & 0xFF;
        unsigned char blue = (u32color >> 8) & 0xFF;
        unsigned char alpha = u32color & 0xFF;
        return rgb_color(
            static_cast<float>(red) / 255.0f, 
            static_cast<float>(green) / 255.0f, 
            static_cast<float>(blue) / 255.0f, 
            static_cast<float>(alpha) / 255.0f
            );
    }

    static rgb_color from_bgra(const v8_uint32_t u32bgra) {
        unsigned char blue = (u32bgra >> 24) & 0xFF;
        unsigned char green = (u32bgra >> 16) & 0xFF;
        unsigned char red = (u32bgra >> 8) & 0xFF;
        unsigned char alpha = u32bgra & 0xFF;
        return rgb_color(
            static_cast<float>(red) / 255.0f, 
            static_cast<float>(green) / 255.0f, 
            static_cast<float>(blue) / 255.0f, 
            static_cast<float>(alpha) / 255.0f
            );
    }

    static rgb_color from_argb(const v8_uint32_t u32argb) {
        unsigned char alpha = (u32argb >> 24) & 0xFF;
        unsigned char red = (u32argb >> 16) & 0xFF;
        unsigned char green = (u32argb >> 8) & 0xFF;
        unsigned char blue = u32argb & 0xFF;
        return rgb_color(
            static_cast<float>(red) / 255.0f, 
            static_cast<float>(green) / 255.0f, 
            static_cast<float>(blue) / 255.0f, 
            static_cast<float>(alpha) / 255.0f
            );
    }

    v8_uint32_t to_rgba() const {
        unsigned int red = static_cast<unsigned int>(ceil(255 * Red)) << 24;
        unsigned int green = static_cast<unsigned int>(ceil(255 * Green)) << 16;
        unsigned int blue = static_cast<unsigned int>(ceil(255 * Blue)) << 8;
        unsigned int alpha = static_cast<unsigned int>(ceil(255 * Alpha));
        return red | green | blue | alpha;
    }

    v8_uint32_t to_bgra() const {
        unsigned int alpha = static_cast<unsigned int>(ceil(255 * Alpha)) << 24;
        unsigned int blue = static_cast<unsigned int>(ceil(255 * Blue)) << 16;
        unsigned int green = static_cast<unsigned int>(ceil(255 * Green)) << 8;
        unsigned int red = static_cast<unsigned int>(ceil(255 * Red));
        return alpha | blue | green | red ;
    }

    rgb_color& operator+=(const rgb_color& rhs) {
        Red += rhs.Red;
        Green += rhs.Green;
        Blue += rhs.Blue;
        Alpha += rhs.Alpha;
        return *this;
    }

    rgb_color& operator-=(const rgb_color& rhs) {
        Red -= rhs.Red;
        Green -= rhs.Green;
        Blue -= rhs.Blue;
        Alpha -= rhs.Alpha;
        return *this;
    }

    rgb_color& operator*=(float k) {
        Red *= k;
        Green *= k;
        Blue *= k;
        Alpha *= k;
        return *this;
    }

    /*!
     *\brief Performs a componentwise multiplication between the two colors.
     */
    rgb_color& operator*=(const rgb_color& other) {
        Red *= other.Red;
        Green *= other.Green;
        Blue *= other.Blue;
        Alpha *= other.Alpha;
        return *this;
    }

    rgb_color& operator/=(const float scalar) {
        float k = 1.0f / scalar;
        Red *= k;
        Green *= k;
        Blue *= k;
        Alpha *= k;
        return *this;
    }

    static const rgb_color C_AliceBlue;

    static const rgb_color C_AntiqueWhite;

    static const rgb_color C_Aqua;

    static const rgb_color C_Aquamarine;

    static const rgb_color C_Azure;

    static const rgb_color C_Beige;

    static const rgb_color C_Bisque;

    static const rgb_color C_Black;

    static const rgb_color C_BlanchedAlmond;

    static const rgb_color C_Blue;

    static const rgb_color C_BlueViolet;

    static const rgb_color C_Brown;

    static const rgb_color C_BurlyWood;

    static const rgb_color C_CadetBlue;

    static const rgb_color C_Chartreuse;

    static const rgb_color C_Chocolate;

    static const rgb_color C_Coral;

    static const rgb_color C_CornflowerBlue;

    static const rgb_color C_Cornsilk;

    static const rgb_color C_Crimson;

    static const rgb_color C_Cyan;

    static const rgb_color C_DarkBlue;

    static const rgb_color C_DarkCyan;

    static const rgb_color C_DarkGoldenRod;

    static const rgb_color C_DarkGray;

    static const rgb_color C_DarkGrey;

    static const rgb_color C_DarkGreen;

    static const rgb_color C_DarkKhaki;

    static const rgb_color C_DarkMagenta;

    static const rgb_color C_DarkOliveGreen;

    static const rgb_color C_Darkorange;

    static const rgb_color C_DarkOrchid;

    static const rgb_color C_DarkRed;

    static const rgb_color C_DarkSalmon;

    static const rgb_color C_DarkSeaGreen;

    static const rgb_color C_DarkSlateBlue;

    static const rgb_color C_DarkSlateGray;

    static const rgb_color C_DarkSlateGrey;

    static const rgb_color C_DarkTurquoise;

    static const rgb_color C_DarkViolet;

    static const rgb_color C_DeepPink;

    static const rgb_color C_DeepSkyBlue;

    static const rgb_color C_DimGray;

    static const rgb_color C_DimGrey;

    static const rgb_color C_DodgerBlue;

    static const rgb_color C_FireBrick;

    static const rgb_color C_FloralWhite;

    static const rgb_color C_ForestGreen;

    static const rgb_color C_Fuchsia;

    static const rgb_color C_Gainsboro;

    static const rgb_color C_GhostWhite;

    static const rgb_color C_Gold;

    static const rgb_color C_GoldenRod;

    static const rgb_color C_Gray;

    static const rgb_color C_Grey;

    static const rgb_color C_Green;

    static const rgb_color C_GreenYellow;

    static const rgb_color C_HoneyDew;

    static const rgb_color C_HotPink;

    static const rgb_color C_IndianRed;

    static const rgb_color C_Indigo;

    static const rgb_color C_Ivory;

    static const rgb_color C_Khaki;

    static const rgb_color C_Lavender;

    static const rgb_color C_LavenderBlush;

    static const rgb_color C_LawnGreen;

    static const rgb_color C_LemonChiffon;

    static const rgb_color C_LightBlue;

    static const rgb_color C_LightCoral;

    static const rgb_color C_LightCyan;

    static const rgb_color C_LightGoldenRodYellow;

    static const rgb_color C_LightGray;

    static const rgb_color C_LightGrey;

    static const rgb_color C_LightGreen;

    static const rgb_color C_LightPink;

    static const rgb_color C_LightSalmon;

    static const rgb_color C_LightSeaGreen;

    static const rgb_color C_LightSkyBlue;

    static const rgb_color C_LightSlateGray;

    static const rgb_color C_LightSlateGrey;

    static const rgb_color C_LightSteelBlue;

    static const rgb_color C_LightYellow;

    static const rgb_color C_Lime;

    static const rgb_color C_LimeGreen;

    static const rgb_color C_Linen;

    static const rgb_color C_Magenta;

    static const rgb_color C_Maroon;

    static const rgb_color C_MediumAquaMarine;

    static const rgb_color C_MediumBlue;

    static const rgb_color C_MediumOrchid;

    static const rgb_color C_MediumPurple;

    static const rgb_color C_MediumSeaGreen;

    static const rgb_color C_MediumSlateBlue;

    static const rgb_color C_MediumSpringGreen;

    static const rgb_color C_MediumTurquoise;

    static const rgb_color C_MediumVioletRed;

    static const rgb_color C_MidnightBlue;

    static const rgb_color C_MintCream;

    static const rgb_color C_MistyRose;

    static const rgb_color C_Moccasin;

    static const rgb_color C_NavajoWhite;

    static const rgb_color C_Navy;

    static const rgb_color C_OldLace;

    static const rgb_color C_Olive;

    static const rgb_color C_OliveDrab;

    static const rgb_color C_Orange;

    static const rgb_color C_OrangeRed;

    static const rgb_color C_Orchid;

    static const rgb_color C_PaleGoldenRod;

    static const rgb_color C_PaleGreen;

    static const rgb_color C_PaleTurquoise;

    static const rgb_color C_PaleVioletRed;

    static const rgb_color C_PapayaWhip;

    static const rgb_color C_PeachPuff;

    static const rgb_color C_Peru;

    static const rgb_color C_Pink;

    static const rgb_color C_Plum;

    static const rgb_color C_PowderBlue;

    static const rgb_color C_Purple;

    static const rgb_color C_Red;

    static const rgb_color C_RosyBrown;

    static const rgb_color C_RoyalBlue;

    static const rgb_color C_SaddleBrown;

    static const rgb_color C_Salmon;

    static const rgb_color C_SandyBrown;

    static const rgb_color C_SeaGreen;

    static const rgb_color C_SeaShell;

    static const rgb_color C_Sienna;

    static const rgb_color C_Silver;

    static const rgb_color C_SkyBlue;

    static const rgb_color C_SlateBlue;

    static const rgb_color C_SlateGray;

    static const rgb_color C_SlateGrey;

    static const rgb_color C_Snow;

    static const rgb_color C_SpringGreen;

    static const rgb_color C_SteelBlue;

    static const rgb_color C_Tan;

    static const rgb_color C_Teal;

    static const rgb_color C_Thistle;

    static const rgb_color C_Tomato;

    static const rgb_color C_Turquoise;

    static const rgb_color C_Violet;

    static const rgb_color C_Wheat;

    static const rgb_color C_White;

    static const rgb_color C_WhiteSmoke;

    static const rgb_color C_Yellow;

    static const rgb_color C_YellowGreen;

};

inline
rgb_color
operator+(const rgb_color& lhs, const rgb_color& rhs) {
    rgb_color result(lhs);
    result += rhs;
    return result;
}

inline
rgb_color
operator-(const rgb_color& lhs, const rgb_color& rhs) {
    rgb_color result(lhs);
    result -= rhs;
    return result;
}

inline
rgb_color
operator*(const rgb_color& lhs, const rgb_color& rhs) {
    rgb_color result(lhs);
    result *= rhs;
    return result;
}

inline
rgb_color
operator*(const rgb_color& lhs, float k) {
    rgb_color result(lhs);
    result *= k;
    return result;
}

inline
rgb_color
operator*(float k, const rgb_color& rhs) {
    return rhs * k;
}

inline
rgb_color
operator/(const rgb_color& lhs, float scalar) {
    float inv = 1.0f / scalar;
    return lhs * inv;
}

inline void rgb_to_cmy(const rgb_color& rgb, color_cmy* cmy) {
    cmy->Cyan = 1.0f - rgb.Red;
    cmy->Magenta = 1.0f - rgb.Green;
    cmy->Yellow = 1.0f - rgb.Blue;
}

inline void cmy_to_rgb(const color_cmy& cmy, rgb_color* rgb) {
    rgb->Red = 1.0f - cmy.Cyan;
    rgb->Green = 1.0f - cmy.Magenta;
    rgb->Blue = 1.0f - cmy.Yellow;
}

inline void rgb_to_yiq(const rgb_color& rgb, color_yiq* yiq) {
    yiq->Y = 0.30f * rgb.Red + 0.59f * rgb.Green + 0.11f * rgb.Blue;
    yiq->I = 0.60f * rgb.Red - 0.28f * rgb.Green - 0.32f * rgb.Blue;
    yiq->Q = 0.21f * rgb.Red - 0.52f * rgb.Green + 0.31f * rgb.Blue;
}

inline void yiq_to_rgb(const color_yiq& yiq, rgb_color* rgb) {
    rgb->Red = yiq.Y + 0.6240f * yiq.Q + 0.9482f * yiq.I;
    rgb->Green = yiq.Y - 0.6398f * yiq.Q - 0.2760f * yiq.I;
    rgb->Blue = yiq.Y + 1.7298f * yiq.Q - 1.1054f * yiq.I;
}

} // namespace math
} // namespace v8
