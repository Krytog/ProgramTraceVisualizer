#pragma once

static const constexpr char* kCubeShaderVert = R"(
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 frag_color;

uniform mat4 transform;

void main() {
	gl_Position = transform * vec4(position, 1.0f);
	frag_color = color;
}
)";

static const constexpr char* kCubeShaderFrag = R"(
#version 330 core

in vec3 frag_color;
out vec4 final_color;

void main() {
	final_color = vec4(frag_color, 1.0f);
}
)";
