#include "Renderer.h"


void Renderer::Draw(const VertexArray& vertexArray, const ElementBuffer& elementBuffer, const Shader& shader) const{
	shader.Bind();
	vertexArray.Bind();
	elementBuffer.Bind();
	glDrawElements(GL_TRIANGLES, elementBuffer.GetCount(), GL_UNSIGNED_INT, 0);	
}