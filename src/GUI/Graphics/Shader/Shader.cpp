#include "Shader.h"
#include "Utils/Logger/ConsoleLogger.h"
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <sstream>
#include <fstream>

#define UNKNONW_SHADER_TYPE_MESSAGE "Shader: unknown shader type"
#define FILE_READING_SHADER_MESSAGE "Shader: error when reading file"
#define SHADER_COMPILE_ERROR_MESSAGE "Shader: compile error"
#define SHADER_LINKING_ERROR_MESSAGE "Shader: linking error"
#define SHADER_SET_UNIFORM_ERROR_MESSAGE "Shader: set unifrom error"

#define ADD_TO_LOG(x) ConsoleLogger::GetInstance().AddToLog(x)

Shader::Shader(const Shader::ShaderConfig& config) {
    descriptor_ = -1;
    GLuint descriptor = glCreateProgram();
    GLuint vertex_shader = -1;
    GLuint fragment_shader = -1;
    GLuint geometry_shader = -1;
    if (config.VertexShaderSourceType != Shader::SourceType::NONE) {
        vertex_shader = CreateSingleShader(config.VertexShaderSourceType, config.VertexShaderSource, Shader::ShaderType::Vertex);
        glAttachShader(descriptor, vertex_shader);
    }
    if (config.FragmentShaderSourceType != Shader::SourceType::NONE) {
        fragment_shader = CreateSingleShader(config.FragmentShaderSourceType, config.FragmentShaderSource, Shader::ShaderType::Fragment);
        glAttachShader(descriptor, fragment_shader);
    }
    if (config.GeometryShaderSourceType != Shader::SourceType::NONE) {
        geometry_shader = CreateSingleShader(config.GeometryShaderSourceType, config.GeometryShaderSource, Shader::ShaderType::Geometry);
        glAttachShader(descriptor, geometry_shader);
    }
    glLinkProgram(descriptor);
    if (!IsShaderLinkedSuccessfully(descriptor)) {
        const size_t buffer_size = 1024;
        GLchar error_info_buffer[buffer_size];
        glGetProgramInfoLog(descriptor, buffer_size, NULL, error_info_buffer);
        ADD_TO_LOG(error_info_buffer);
        throw std::runtime_error(SHADER_LINKING_ERROR_MESSAGE);
    }
    if (vertex_shader != -1) {
        glDeleteShader(vertex_shader);
    }
    if (fragment_shader != -1) {
        glDeleteShader(fragment_shader);
    }
    if (geometry_shader != -1) {
        glDeleteShader(geometry_shader);
    }
    descriptor_ = descriptor;
}

GLuint Shader::CreateSingleShader(SourceType source_type, const char* source, ShaderType type) {
    std::string source_buffer;
    if (source_type == SourceType::FILE) {
        std::ifstream shader_file;
        shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            shader_file.open(source);
            std::stringstream shader_stream;
            shader_stream << shader_file.rdbuf();
            shader_file.close();
            source_buffer = shader_stream.str();
            source = source_buffer.c_str();
        } catch (std::ifstream::failure& e) {
            std::string error_message = "Shader: error when reading source: ";
            error_message += std::string(source);
            error_message += std::string(" with exceptions: ");
            error_message += e.what();
            ADD_TO_LOG(error_message);
            throw std::runtime_error(FILE_READING_SHADER_MESSAGE);
        }
    }
    GLuint shader;
    switch (type) {
        case ShaderType::Vertex: {
            shader = glCreateShader(GL_VERTEX_SHADER);
            break;
        }
        case ShaderType::Fragment: {
            shader = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        }
        case ShaderType::Geometry: {
            shader = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        }
        default: {
            throw std::runtime_error(UNKNONW_SHADER_TYPE_MESSAGE);
        }
    }
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    if (!IsShaderCompiledSuccessfully(shader)) {
        const size_t buffer_size = 1024;
        GLchar error_info_buffer[buffer_size];
        glGetShaderInfoLog(shader, buffer_size, NULL, error_info_buffer);
        ADD_TO_LOG(error_info_buffer);
        throw std::runtime_error(SHADER_COMPILE_ERROR_MESSAGE);
    }
    return shader;
}

bool Shader::IsShaderCompiledSuccessfully(GLuint shader) {
    GLint output;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &output);
    return output;
}

bool Shader::IsShaderLinkedSuccessfully(GLuint shader_program) {
    GLint output;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &output);
    return output;
}

Shader::~Shader() {
    if (descriptor_ != -1) {
        glDeleteProgram(descriptor_);
    }
}

void Shader::Use() const {
    glUseProgram(descriptor_);
}

GLint Shader::GetUniformLocation(const std::string& name) {
    if (cached_uniforms_locations_.contains(name)) {
        return cached_uniforms_locations_[name];
    }
    GLint location = glGetUniformLocation(descriptor_, static_cast<const GLchar*>(name.c_str()));
    if (location == -1) {
        std::string error_message = "Shader: error when settting an uniform: ";
        error_message += std::string(name);
        ADD_TO_LOG(error_message);
        throw std::runtime_error(SHADER_SET_UNIFORM_ERROR_MESSAGE);
    }
    cached_uniforms_locations_[name] = location;
    return location;
}

void Shader::SetUniform(const std::string& name, GLfloat value) {
    GLint location = GetUniformLocation(name);
    glUniform1f(location, value);
}

void Shader::SetUniform(const std::string& name, GLint value) {
    GLint location = GetUniformLocation(name);
    glUniform1i(location, value);
}

void Shader::SetUniform(const std::string& name, GLuint value) {
    GLint location = GetUniformLocation(name);
    glUniform1ui(location, value);
}

void Shader::SetUniform(const std::string& name, const glm::vec2& value) {
    GLint location = GetUniformLocation(name);
    glUniform2fv(location, 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::vec3& value) {
    GLint location = GetUniformLocation(name);
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::vec4& value) {
    GLint location = GetUniformLocation(name);
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::mat2& value) {
    GLint location = GetUniformLocation(name);
    glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::mat3& value) {
    GLint location = GetUniformLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::mat4& value) {
    GLint location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
