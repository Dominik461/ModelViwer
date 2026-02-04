#include "Shader.h"

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc)
    : VertexShader(0), FragmentShader(0), ShaderProgram(0)
{
    // Compile vertex shader
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    CompileShader(GL_VERTEX_SHADER, vertexSrc);

    // Compile fragment shader
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    // Create shader program
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    // Check for linking errors
    GLint success;
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(ShaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }

    // Clean up individual shaders (they're linked into the program now)
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    VertexShader = 0;
    FragmentShader = 0;
}

Shader::~Shader()
{
    if (ShaderProgram != 0)
    {
        glDeleteProgram(ShaderProgram);
    }
    if (VertexShader != 0)
    {
        glDeleteShader(VertexShader);
    }
    if (FragmentShader != 0)
    {
        glDeleteShader(FragmentShader);
    }
}

void Shader::Bind() const
{
    glUseProgram(ShaderProgram);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::UploadUniformInt1(const std::string& name, const glm::i16 &i)
{
    GLint location = glGetUniformLocation(ShaderProgram, name.c_str());
    if (location == -1)
    {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader" << std::endl;
        return;
    }
    glUniform1i(location, i);
}

void Shader::UploadUniformFloat1(const std::string& name, const glm::float32 &f)
{
    GLint location = glGetUniformLocation(ShaderProgram, name.c_str());
    if (location == -1)
    {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader" << std::endl;
        return;
    }
    glUniform1f(location, f);
}


void Shader::UploadUniformFloat2(const std::string &name, const glm::vec2 &vector)
{
    GLint location = glGetUniformLocation(ShaderProgram, name.c_str());
    if (location == -1)
    {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader" << std::endl;
        return;
    }
    glUniform3f(location, vector.x, vector.y,0.0f);
}

void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& vector)
{
    GLint location = glGetUniformLocation(ShaderProgram, name.c_str());
    if (location == -1)
    {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader" << std::endl;
        return;
    }
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& mat)
{
    auto location = glGetUniformLocation(ShaderProgram, name.c_str());
    if (location == -1)
    {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader" << std::endl;
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::CompileShader(GLenum shaderType, const std::string &shaderSrc)
{
    GLuint shader = (shaderType == GL_VERTEX_SHADER) ? VertexShader : FragmentShader;

    const char *source = shaderSrc.c_str();
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        const char *shaderTypeStr = (shaderType == GL_VERTEX_SHADER) ? "vertex" : "fragment";
        std::cerr << "Shader compilation failed (" << shaderTypeStr << "): " << infoLog << std::endl;
    }
}