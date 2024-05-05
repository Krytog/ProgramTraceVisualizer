#pragma once

// From: https://www.shadertoy.com/view/NtcyWn

static const constexpr char* kProgressWidgetShaderFrag = R"(
#version 330 core

uniform float time;
uniform float width;
uniform float height;
uniform bool progress_mode = true;

out vec4 final_color;

#define THICKNESS 0.05
#define RADIUS 0.4
#define AA (1.0 / height)

float DrawCircle(vec2 uv, vec2 p, float r)
{
    uv += p;
    return smoothstep(r+AA, r-AA, length(uv));
}

vec2 RotationUV(vec2 uv, float d)
{
    float s = sin(d);
    float c = cos(d);
    mat2 rotMatrix = mat2(c, -s, s, c);
    return uv * rotMatrix;
}

vec4 InterpolateColor(vec4 raw_color, float alpha) {
    vec4 low_color = vec4(55.0f/255, 235.0f/255, 243.0f/255, 1.0f);
    vec4 high_color = vec4(243.0f/255, 55.0f/255, 141.0f/255, 1.0f);
    vec4 interpolated_color = alpha * high_color + (1 - alpha) * low_color;
    return raw_color * interpolated_color;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    //float aspect = width / height;
    //vec2 uv = fragCoord/vec2(widht, height);
    //uv -= 0.5;
    //uv.x *= aspect;
    
    //float outCircle = DrawCircle(uv, vec2(0,0), RADIUS + THICKNESS);
    //float innerCircle = DrawCircle(uv, vec2(0,0), RADIUS - THICKNESS);
    //float circle = outCircle - innerCircle;
    
    // Thanks to FabriceNeyret2
    vec2 uv = (fragCoord - 0.5 * vec2(width, height)) / height;
    float circle = smoothstep(RADIUS+AA, RADIUS-AA, abs(length(uv) - RADIUS) + RADIUS - THICKNESS);
    float radial = (atan(uv.x, -uv.y) / 3.14159265359 * 0.5) + 0.5;
    float alpha = time / 100.0f;
    alpha = min(alpha, 0.99999f);
    radial += alpha;
    circle *= step(fract(radial), fract(alpha));

    vec4 raw_color = vec4(circle, circle, circle, 1.0);
    vec4 interpolated_color = InterpolateColor(raw_color, alpha);

    fragColor = interpolated_color;
}

#define PI 3.14159265359

vec3 hsb2rgb( in vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix( vec3(1.0), rgb, c.y);
}

vec2 rotate2D (vec2 _st, float _angle) {
    _st =  mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle)) * _st;
    return _st;
}

void mainImage2( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 p = (2.0*fragCoord.xy-vec2(width, height))/width;
    p *= 1.6;
    vec3 color = hsb2rgb(vec3(fract(time*.1),1.,1.));
    float r = length(p);
    float w = .3;
    p = rotate2D(p,(r*PI*6.-time*2.));
    color *= smoothstep(-w,.0,p.x)*smoothstep(w,.0,p.x);
    color *= abs(1./(sin(pow(r,2.)*2.-time*1.3)*6.))*.4;
	fragColor = vec4(color, 1.);
}

void main() {
    if (progress_mode) {
        mainImage(final_color, gl_FragCoord.xy);
    } else {
        mainImage2(final_color, gl_FragCoord.xy);
    }
}

)";