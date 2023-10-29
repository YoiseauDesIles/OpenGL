#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned* data, unsigned count) :m_count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_RendererId));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererId));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
}

void IndexBuffer::Unbind() const 
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}