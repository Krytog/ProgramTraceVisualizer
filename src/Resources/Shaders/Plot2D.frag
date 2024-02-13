#version 330 core

in float temperature;
out vec4 color;

vec4 get_color(float temp) {
	vec3 right = vec3(1.0f, 0.0f, 0.0f);
	vec3 left = vec3(0.0f, 0.0f, 1.0f);
	vec3 interpolated = temp * right + (1 - temp) * left;
	return vec4(interpolated, 1.0f);
}

void main() {
	color = get_color(temperature);
}