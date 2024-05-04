#pragma once

static const constexpr char* kDefaultShaderVert = R"(
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 _base_color;

out vec2 base_color;

void main() {
	gl_Position = vec4(position, 1.0f);
	base_color = _base_color;
}
)";

static const constexpr char* kDefaultShaderFrag = R"(
#version 330 core

in vec2 base_color;
out vec4 color;

void main() {
	color = vec4(1.0f, base_color, 1.0f);
}
)";
