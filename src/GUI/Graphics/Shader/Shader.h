#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <string>

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

	/* Sets the given values as the given uniform argument in the shader */
	void SetUniform(const std::string& name, GLfloat value);
	void SetUniform(const std::string& name, GLint value);
	void SetUniform(const std::string& name, GLuint value);
	void SetUniform(const std::string& name, const glm::vec2& value);
	void SetUniform(const std::string& name, const glm::vec3& value);
	void SetUniform(const std::string& name, const glm::vec4& value);
	void SetUniform(const std::string& name, const glm::mat2& value);
	void SetUniform(const std::string& name, const glm::mat3& value);
	void SetUniform(const std::string& name, const glm::mat4& value);

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

	[[nodiscard]] GLint GetUniformLocation(const std::string& name);

	GLuint descriptor_;
	std::unordered_map<std::string, GLint> cached_uniforms_locations_;
};