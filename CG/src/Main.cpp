#include "Main.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main()
{
	GLFWwindow* window = initWindow();

	glViewport(0, 0, 800, 600);
	
	Shader shader("resources//shaders//Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_color", 1.0f,1.0f,0.0f,1.0f);	

	float vertices[] = {
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	VertexArray va;	
	VertexBuffer vb(vertices, sizeof(vertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	va.AddBuffer(vb, layout);
	ElementBuffer eb(indices, 6);
			
	va.Bind();
	eb.Bind();
		
	float pace = 0.02f;
	float color = 0.1f;

	while (!glfwWindowShouldClose(window))
	{
		//Input
			processInput(window);
		
		//Process
			pace = color >= 1.0f || color <= 0.0f ? pace * -1.0f : pace;
			color += pace;

		//Render
			glClearColor(0.0f, color, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);		
		
			shader.SetUniform4f("u_color", 1.0f, 1.0f, color, 1.0f);
			glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT,0);

		//Buffer swapping
			glfwSwapBuffers(window);
		
		//Poll and process events
			glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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