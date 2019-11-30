#include "VertexArray.h"

VertexArray::VertexArray(){
	glGenVertexArrays(1, &m_RendererID);
	
}

VertexArray::~VertexArray(){
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout){
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (size_t i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		offset+= element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	
}

void VertexArray::Bind(){
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() {
	glBindVertexArray(0);
}
