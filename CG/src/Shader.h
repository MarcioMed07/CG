#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "glm/glm.hpp"


struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	unsigned int m_RendererID;
	const std::string& m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationChache;
	
public:


	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind();

	void SetUniform1i(const std::string& name, int v0);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, glm::mat4& v0);

private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderSource ParseShader(const std::string& filepath);
	int GetUniformLocation(const std::string& name);

};