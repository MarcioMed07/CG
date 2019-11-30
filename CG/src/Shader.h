#pragma once
#include <string>
#include <fstream>
#include <fstream>
#include <unordered_map>

struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	unsigned int m_RendererID;
	std::unordered_map<const std::string, int> m_UniformLocationChache;
	const std::string& m_FilePath;
	int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderSource ParseShader(const std::string& filepath);
public:


	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind();

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);


};