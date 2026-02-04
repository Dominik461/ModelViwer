#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(GLuint *indices, GLsizei count) : Count(count)
{
    // Generate a buffer ID
    glGenBuffers(1, &IndexBufferID);

    // Bind the buffer as GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);

    // Upload the index data to the buffer
    // count is the number of elements, so we multiply by sizeof(GLuint) to get bytes
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    // Delete the buffer when the object is destroyed
    glDeleteBuffers(1, &IndexBufferID);
}

void IndexBuffer::Bind() const
{
    // Bind this buffer as the current GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
}

void IndexBuffer::Unbind() const
{
    // Unbind the current GL_ELEMENT_ARRAY_BUFFER by binding 0
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}