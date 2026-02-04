#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <vector>

class Shader
{
public:
    Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;
    void UploadUniformInt1(const std::string &name, const glm::i16 &i);
    void UploadUniformFloat1(const std::string &name, const glm::float32 &f);
    void UploadUniformFloat2(const std::string &name, const glm::vec2 &vector);
    void UploadUniformFloat3(const std::string& name, const glm::vec3& vector);
    void UploadUniformMat4(const std::string& name, const glm::mat4& mat);

private:
    GLuint VertexShader;
    GLuint FragmentShader;
    GLuint ShaderProgram;

    void CompileShader(GLenum shaderType, const std::string &shaderSrc);
};
