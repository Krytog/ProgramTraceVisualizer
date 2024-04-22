#version 330 core

in float temperature;
out vec4 color;

uniform vec4 newest_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
uniform vec4 oldest_color = vec4(0.0f, 0.0f, 1.0f, 1.0f);

vec4 get_color(float temp) {
	vec4 interpolated = temp * newest_color + (1 - temp) * oldest_color;
	return interpolated;
}

void main() {
	color = get_color(temperature);
}