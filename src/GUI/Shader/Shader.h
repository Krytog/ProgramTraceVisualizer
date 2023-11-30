#pragma once

#include <GL/glew.h>

class Shader {
public:
	enum class SourceType {
		FILE,
		STRING,
		NONE
	};

	struct ShaderConfig {
		SourceType VertexShaderSourceType;
		const char* VertexShaderSource;
		SourceType FragmentShaderSourceType;
		const char* FragmentShaderSource;
		SourceType GeometryShaderSourceType;
		const char* GeometryShaderSource;
	};

	Shader(const ShaderConfig& config);
	
	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

	/* Tells OpenGL to use this shader for future draw calls */
	void Use() const;

	~Shader();

private:
	enum class ShaderType {
		Vertex,
		Fragment,
		Geometry
	};

	[[nodiscard]] GLuint CreateSingleShader(SourceType source_type, const char* source, ShaderType type);
	[[nodiscard]] bool IsShaderCompiledSuccessfully(GLuint shader);
	[[nodiscard]] bool IsShaderLinkedSuccessfully(GLuint shader_program);

	GLuint descriptor_;
};