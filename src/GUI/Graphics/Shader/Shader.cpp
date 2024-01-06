#include "Shader.h"
#include "Utils/Logger/ConsoleLogger.h"

#include <string>
#include <sstream>
#include <fstream>

#define UNKNONW_SHADER_TYPE_MESSAGE "Shader: unknown shader type"
#define FILE_READING_SHADER_MESSAGE "Shader: error when reading file"
#define SHADER_COMPILE_ERROR_MESSAGE "Shader: compile error"
#define SHADER_LINKING_ERROR_MESSAGE "Shader: linking error"

#define LOGGER_CLASS ConsoleLogger

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
        LOGGER_CLASS::GetInstance().AddToLog(error_info_buffer);
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
            LOGGER_CLASS::GetInstance().AddToLog(error_message);
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
        LOGGER_CLASS::GetInstance().AddToLog(error_info_buffer);
        throw std::runtime_error(SHADER_COMPILE_ERROR_MESSAGE);
    }
    return shader;
}

bool Shader::IsShaderCompiledSuccessfully(GLuint shader) {
    GLint output;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &output);
    return output;
}

bool Shader::IsShaderLinkedSuccessfully(GLuint shader_program)
{
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