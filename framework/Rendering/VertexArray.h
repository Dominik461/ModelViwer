#ifndef VERTEXARRAY_H_
#define VERTEXARRAY_H_

#include <glad/glad.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>
#include <memory>
#include <cassert>
#include <glm/glm.hpp>

#define ASSERT(exp, msg) assert((void(msg), exp))

class VertexArray {
public:
    // Constructor & Destructor
    VertexArray();
    ~VertexArray();

    // Bind vertex array
    void Bind() const;
    // Unbind vertex array
    void Unbind() const;

    // Add vertex buffer. This method utilizes the BufferLayout internal to
    // the vertex buffer to set up the vertex attributes. Notice that
    // this function opens for the definition of several vertex buffers.
    void AddNormalVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer);
    void AddPerInstanceVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer, GLuint everyXInstance);
    // Set index buffer
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

    void EnableVertexArray(GLuint i);
    int GetNumVBOs();

    // Get the index buffer
    const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return IdxBuffer; }
    const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return VertexBuffers; };

    // New method: Setup instance buffer properly
    void SetupInstanceBuffer(const std::vector<float>& instanceData, int maxCapacity = 0);

    // New method: Add instances dynamically
    void AddInstances(const std::vector<glm::vec3>& colors,
        const std::vector<glm::mat4>& modelMatrices);

    // New method: Update all instance data
    void UpdateInstanceData(const std::vector<float>& instanceData);

    // Get current instance count
    size_t GetInstanceCount() const { return m_instanceCount; }

private:
    GLuint m_vertexArrayID;
    std::vector<std::shared_ptr<VertexBuffer>> VertexBuffers;
    std::shared_ptr<IndexBuffer> IdxBuffer;
    GLuint m_indexOfAttributes = 0;

    // Store instance buffer separately
    std::shared_ptr<VertexBuffer> m_instanceBuffer;
    size_t m_instanceCount = 0;
    size_t m_maxInstanceCapacity = 0;

    // Get the vertex buffers

    void SetVertexAttributePointer(GLuint bufferIndex);
    void SetVertexAttributeDivisor(GLuint bufferIndex, GLuint everyInstance);
};


#endif // !VERTEXARRAY_H_
