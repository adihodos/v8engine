inline 
const v8::math::light 
v8::math::light::make_directional_light(
    const v8::math::rgb_color& ambient_color, 
    const v8::math::rgb_color& diffuse_color, 
    const v8::math::rgb_color& specular_color,
    const v8::math::vector3F& direction
    ) {
    light l;
    l.set_type(light::type::directional);
    l.set_ambient_color(ambient_color);
    l.set_diffuse_color(diffuse_color);
    l.set_specular_color(specular_color);
    l.set_direction(direction);
    return l;
}

inline 
const v8::math::light 
v8::math::light::make_point_light(
    const math::rgb_color& ambient_color, 
    const math::rgb_color& diffuse_color, 
    const math::rgb_color& specular_color,
    const math::vector3F& position,
    const math::vector3F& attenuation,
    const float max_range
    ) {
    light l;
    l.set_type(v8::math::light::type::point);
    l.set_ambient_color(ambient_color);
    l.set_diffuse_color(diffuse_color);
    l.set_specular_color(specular_color);
    l.set_position(position);
    l.set_max_range(max_range);
    l.set_attenuation_factors(attenuation); 
    return l;
}

inline 
const v8::math::light 
v8::math::light::make_spot_light(
    const math::rgb_color& ambient_color, 
    const math::rgb_color& diffuse_color, 
    const math::rgb_color& specular_color,
    const math::vector3F& position,
    const math::vector3F& direction,
    const math::vector3F& attenuation,
    const float max_range,
    const float spot_power,
    const float spot_cone_theta,
    const float spot_cone_phi
    ) {
    light l;
    l.set_type(v8::math::light::type::spot);
    l.set_ambient_color(ambient_color);
    l.set_diffuse_color(diffuse_color);
    l.set_specular_color(specular_color);
    l.set_direction(direction);
    l.set_position(position);
    l.set_max_range(max_range);
    l.set_attenuation_factors(attenuation);
    l.set_spot_cone_angle_theta(spot_cone_theta);
    l.set_spot_cone_angle_phi(spot_cone_phi);
    l.set_spot_power(spot_power); 
    return l;
}


