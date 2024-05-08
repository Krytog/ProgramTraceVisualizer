#pragma once

static const constexpr char* kPlot2DShaderVert = R"(
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
)";

static const constexpr char* kPlot2DShaderGeom = R"(
#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in GeomArgs {
	float temperature;
} geomargs_in[];

uniform float grid_size;

out float temperature;

void main() {
	temperature = geomargs_in[0].temperature;
    gl_Position = gl_in[0].gl_Position + vec4(-grid_size, grid_size, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(grid_size, grid_size, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(-grid_size, -grid_size, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(grid_size, -grid_size, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}  
)";

static const constexpr char* kPlot2DShaderFrag = R"(
#version 330 core

in float temperature;
out vec4 color;

uniform vec4 high_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
uniform vec4 low_color = vec4(0.0f, 0.0f, 1.0f, 1.0f);

vec4 get_color(float temp) {
	vec4 interpolated = temp * high_color + (1 - temp) * low_color;
	return interpolated;
}

void main() {
	color = get_color(temperature);
}
)";
