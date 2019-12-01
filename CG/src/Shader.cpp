#include "Shader.h"


Shader::Shader(const std::string & filepath): m_FilePath(filepath), m_RendererID(0) {
	ShaderSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}
Shader::~Shader() {
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const{
	glUseProgram(m_RendererID);
}

void Shader::Unbind(){
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string & name, int v0){
	glUniform1i(GetUniformLocation(name), v0);
}

void Shader::SetUniform1f(const std::string & name, float v0) {
	glUniform1f(GetUniformLocation(name), v0);
}

void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3){
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string & name, glm::mat4 & v0){
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &v0[0][0]);
}

int Shader::GetUniformLocation(const std::string & name) {
	if (m_UniformLocationChache.find(name) != m_UniformLocationChache.end()) {
		return m_UniformLocationChache[name];
	}
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1) {
		std::cout << "uniform: " << name << " does not exist" << std::endl;
	}
	m_UniformLocationChache[name] = location;
	return location;
}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " SHADER COMPILATION FAILED\n" << infoLog << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

ShaderSource Shader::ParseShader(const std::string& filepath) {

	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}
