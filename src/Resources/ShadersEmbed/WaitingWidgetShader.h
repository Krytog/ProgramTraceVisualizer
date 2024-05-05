#pragma once

// From: https://www.shadertoy.com/view/3tdSRn

static const constexpr char* kWaitingWidgetShaderFrag = R"(
#version 330 core

uniform float time;
uniform float width;
uniform float height;

out vec4 final_color;

vec3 drawCircle(vec2 pos, float radius, float width, float power, vec4 color) {
    float dist1 = length(pos);
    dist1 = fract((dist1 * 5.0) - fract(time));
    float dist2 = dist1 - radius;
    float intensity = pow(radius / abs(dist2), width); 
    vec3 col = color.rgb * intensity * power * max((0.8- abs(dist2)), 0.0);
    return col;
}

vec3 hsv2rgb(float h, float s, float v) {
    vec4 t = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(vec3(h) + t.xyz) * 6.0 - vec3(t.w));
    return v * mix(vec3(t.x), clamp(p - vec3(t.x), 0.0, 1.0), s);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    vec2 pos = (fragCoord.xy * 2.0 - vec2(width, height)) / min(width, height);
    
    float h = mix(0.5, 0.65, length(pos));
    vec4 color = vec4(hsv2rgb(h, 1.0, 1.0), 1.0);
    float radius = 0.5;
    float _width = 0.8;
    float power = 0.1;
    vec3 finalColor = drawCircle(pos, radius, _width, power, color);

    pos = abs(pos);
    // vec3 finalColor = vec3(pos.x, 0.0, pos.y);

    fragColor = vec4(finalColor, 1.0);
}

void main() {
	mainImage(final_color, gl_FragCoord.xy);
}

)";