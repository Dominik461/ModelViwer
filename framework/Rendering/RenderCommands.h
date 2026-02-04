#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include "VertexArray.h"

namespace RenderCommands
{
    inline void Clear(GLuint mode = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    {
        glClear(mode);
    }

    inline void SetPolygonMode(GLenum face, GLenum mode)
    {
        glPolygonMode(face, mode);
    }

    inline void DrawIndex(const std::shared_ptr<VertexArray> &vao, GLenum primitive)
    {
        glDrawElements(primitive, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    inline void DrawInstance(const std::shared_ptr<VertexArray>& vao, GLenum primitive, GLuint instances)
    {
        glDrawElementsInstanced(primitive, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, instances);
    }

    inline void SetClearColor(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    inline void SetWireframeMode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    inline void SetSolidMode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    inline void EnableBackFaceCulling()
    {
        glEnable(GL_CULL_FACE);
    }

    inline void DisbaleBackFaceCulling()
    {
        glDisable(GL_CULL_FACE);
    }
}
