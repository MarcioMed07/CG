#include "ElementBuffer.h"
#include "Renderer.h"

ElementBuffer::ElementBuffer(const unsigned int* data, unsigned int count) : m_Count(count){
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer(){
	glDeleteBuffers(1, &m_RendererID);
}

void ElementBuffer::Bind()  const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void ElementBuffer::Unbind()  const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
