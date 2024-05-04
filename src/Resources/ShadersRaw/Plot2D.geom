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