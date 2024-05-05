#pragma once

// From: https://www.shadertoy.com/view/sscyDB

static const constexpr char* kWaitingWidgetShaderFrag = R"(
#version 330 core

uniform float time;
uniform float width;
uniform float height;

out vec4 final_color;

const float PI = 3.14159;

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = fragCoord / vec2(width, height);
    float d = 0.0;
    int n = 9;
    vec3 col = vec3(0.);
    for (int i = 0; i < n; i++) {
        float findex = float(i) / float(n);
        float speed = 1.0 + 1.0 - findex;
        vec2 uv2 = vec2(uv.x, min(1., max(0., uv.y + 0.1 * sin(uv.x * 2. * PI + time * speed + findex * 10.1))));
        float offset = 3. * findex * PI + 0.3 * time * (0.2 + findex * 0.3);
        float f = 0.5 * (1. + sin(offset));
        float d = abs(f - uv2.y);
        float fcol = pow(1. - pow(d, 0.4), 3.5 + 4. * 0.5 * (1. + sin(time * (1. + findex) + 4. * findex * PI)));
        float r = 360. * findex * fcol;
        float b = i % 2 == 1 ? fcol * 0.9 : 0.;
        col += vec3(b, fcol * findex, fcol);
    }
    fragColor = vec4( col, 1.0 );
}

void main() {
	mainImage(final_color, gl_FragCoord.xy);
}

)";