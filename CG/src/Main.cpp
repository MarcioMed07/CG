#include <iostream>
#include "Renderer.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void processInput(GLFWwindow *window, float* pace, float* max);
GLFWwindow* initWindow();

int main()
{
	GLFWwindow* window = initWindow();

	glViewport(0, 0, 800, 600);
	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);	

	VertexArray vertexArray;
	VertexBuffer vertexBuffer(vertices, sizeof(vertices));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	vertexArray.AddBuffer(vertexBuffer, layout);
	
	ElementBuffer elementBuffer(indices, 6);

	glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	Shader shader("resources/shaders/Basic.shader");
	shader.Bind();
	shader.SetUniformMat4f("u_MVP", proj);

	Texture texture("resources/textures/wat.png");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	Texture displace("resources/textures/disp.png");
	texture.Bind();
	displace.Bind(1);
	shader.SetUniform1i("u_DisplaceMap", 1);

	shader.SetUniform1f("u_Time", 0.0f);
	shader.SetUniform1f("u_Maximum", 0.1f);

	vertexArray.Unbind();
	vertexBuffer.Unbind();
	elementBuffer.Unbind();
	shader.Unbind();
	
	Renderer renderer;
		
	float r, g, b, pace;
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
	pace = 0.000f;
	float count = 0.0f;
	float max = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		//Input
			processInput(window, &pace, &max);
		
		//Process
			count += pace;
			shader.Bind();
			shader.SetUniform1f("u_Time", count);
			shader.SetUniform1f("u_Maximum", max);

			/*
			if (r < 1.0f && r >= 0.0f) {
				r += pace;
			}
			else if (g < 1.0f && g >= 0.0f) {
				g += pace;
			}
			else if (b < 1.0f && b >= 0.0f) {
				b += pace;
			}
			else {
				pace = pace * -1;
				r += pace;
				g += pace;
				b += pace;
				std::cout << "mec";
			}*/

		//Render			
			renderer.Draw(vertexArray, elementBuffer, shader);
		//Buffer swapping
			glfwSwapBuffers(window);		
		//Poll and process events
			glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window, float* pace, float* max)
{
	float increasePace = 0.001;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		*pace += increasePace;
		std::cout << "pace: " << *pace << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		*pace -= increasePace;
		std::cout << "pace: " << *pace << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		*max += increasePace;
		std::cout << "max: " << *max << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		*max -= increasePace;
		std::cout << "max: " << *max << std::endl;
	}
}

GLFWwindow* initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "CG", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}

	return window;
}