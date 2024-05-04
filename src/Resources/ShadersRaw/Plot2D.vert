#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in float _temperature;

out GeomArgs {
	float temperature;
} geomargs_out;

void main() {
	gl_Position = vec4(position, 0.0f, 1.0f);
	geomargs_out.temperature = _temperature;
}