#pragma once

// From: https://www.shadertoy.com/view/NtcyWn

static const constexpr char* kProgressWidgetShaderFrag = R"(
#version 330 core

uniform float time;
uniform float width;
uniform float height;

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

void main() {
	mainImage(final_color, gl_FragCoord.xy);
}

)";