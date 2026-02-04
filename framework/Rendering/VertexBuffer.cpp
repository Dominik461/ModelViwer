#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void *vertices, GLsizei size, GLenum draw)
{
    // Generate a buffer ID
    glGenBuffers(1, &VertexBufferID);

    // Bind the buffer as GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);

    // Upload the vertex data to the buffer
    glBufferData(GL_ARRAY_BUFFER, size, vertices, draw);
}

VertexBuffer::~VertexBuffer()
{
    // Delete the buffer when the object is destroyed
    glDeleteBuffers(1, &VertexBufferID);
}

void VertexBuffer::Bind() const
{
    // Bind this buffer as the current GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
}

void VertexBuffer::Unbind() const
{
    // Unbind the current GL_ARRAY_BUFFER by binding 0
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, const void *data) const
{
    // Update a portion of the buffer with new data
    // Note: The buffer should be bound before calling this function
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}