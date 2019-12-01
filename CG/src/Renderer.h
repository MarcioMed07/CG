#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Shader.h"

class Renderer {
public:
	void Draw(const VertexArray& vertexArray, const ElementBuffer& elementBuffer, const Shader& shader) const;
};