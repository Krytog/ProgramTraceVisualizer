#version 330 core

in vec2 base_color;
out vec4 color;

void main() {
	color = vec4(1.0f, base_color, 1.0f);
}