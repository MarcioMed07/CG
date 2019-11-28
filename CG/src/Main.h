#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

void processInput(GLFWwindow *window);
GLFWwindow* initWindow();
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
static ShaderSource parseShader(const std::string& filepath);