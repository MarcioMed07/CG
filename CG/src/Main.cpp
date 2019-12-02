#include <iostream>
#include "Renderer.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
GLFWwindow* initWindow();

//Camera
glm::vec3 cameraPos = glm::vec3(0.0f, -1.2f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 90.0f;
float cameraSpeed = 2.5f;
float mouseSensitivity = 0.1f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
	GLFWwindow* window = initWindow();
	glViewport(0, 0, glfwGetVideoMode(glfwGetPrimaryMonitor())->width,glfwGetVideoMode(glfwGetPrimaryMonitor())->height );

	float vertices[] = {
		//position			  //texCoord	
		-10.5f, -1.5f, -10.5f,  0.0f, 0.0f,
		 10.5f, -1.5f, -10.5f,  10.0f, 0.0f,
		 10.5f,  -1.5f, 10.5f,  10.0f, 10.0f,
		-10.5f,  -1.5f, 10.5f,  0.0f, 10.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,		
	};

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);	

	GLint viewPort[4];
	glGetIntegerv(GL_VIEWPORT, viewPort);

	VertexArray vertexArray;
	VertexBuffer vertexBuffer(vertices, sizeof(vertices));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	vertexArray.AddBuffer(vertexBuffer, layout);
	
	ElementBuffer elementBuffer(indices, 12);

	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	Shader shader("resources/shaders/Displace.shader");
	shader.Bind();
	shader.SetUniformMat4f("u_ProjectionM", projectionMatrix);
	shader.SetUniformMat4f("u_ViewM", viewMatrix);
	shader.SetUniformMat4f("u_ModelM", modelMatrix);

	Texture texture("resources/textures/cartoonWater2.png");
	Texture displace("resources/textures/displacement.jpg");
	shader.SetUniform1i("u_Texture", 0);
	shader.SetUniform1i("u_DisplaceMap", 1);
	texture.Bind();
	displace.Bind(1);

	
	shader.SetUniform1f("u_TimeTexture", 0.0f);
	shader.SetUniform1f("u_TimeDisplace", 0.0f);
	shader.SetUniform1f("u_Maximum", 0.1f);
	shader.SetUniform1f("u_TextureAlpha", 1.0f);


	vertexArray.Unbind();
	vertexBuffer.Unbind();
	elementBuffer.Unbind();
	shader.Unbind();
	
	Renderer renderer;

	float countTex = 0.0f;
	float countDis = 0.0f;
	float paceTex = -0.0007f;
	float paceDis = 0.0007f;
	float max = 0.06f;
	float alpha = 0.6f;
	float bgColor[] = {1.0f,1.0f,1.0f};
	int waterTextureOption = 0;
	int lastWaterTextureOption = waterTextureOption;
	
	//imGui stuff
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	while (!glfwWindowShouldClose(window)){
		//TIMING
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
		//imGui
			ImGui_ImplGlfwGL3_NewFrame();
		//Input
			processInput(window);
		
		//Process
			countTex += paceTex;
			countDis += paceDis;
			shader.Bind();
			shader.SetUniform1f("u_TimeTexture", countTex);
			shader.SetUniform1f("u_TimeDisplace", countDis);
			shader.SetUniform1f("u_Maximum", max);
			shader.SetUniform1f("u_TextureAlpha", alpha);

		//MVP AND CAMERA
			glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			projectionMatrix = glm::mat4(1.0f);
			viewMatrix = glm::mat4(1.0f);
			modelMatrix = glm::mat4(1.0f);

			glGetIntegerv(GL_VIEWPORT, viewPort);
			projectionMatrix = glm::perspective(glm::radians(fov), (float)viewPort[2] / (float)viewPort[3], 0.1f, 100.0f);
			viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			shader.SetUniformMat4f("u_ProjectionM", projectionMatrix);
			shader.SetUniformMat4f("u_ViewM", viewMatrix);

			shader.SetUniformMat4f("u_ModelM", modelMatrix);
			renderer.Draw(vertexArray, elementBuffer, shader);

			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.00f, 0.1f, 0.0f));
			shader.SetUniformMat4f("u_ModelM", modelMatrix);
			shader.SetUniform1f("u_TimeTexture", -1.0f*countTex);
			renderer.Draw(vertexArray, elementBuffer, shader);

		//Render
			{
				ImGui::Text("Camera Options");
				ImGui::SliderFloat("Mouse Sensitivity", &mouseSensitivity, 0.001f, 1.0f);
				ImGui::SliderFloat("Move Speed", &cameraSpeed, 0.1f, 10.0f);
				ImGui::SliderFloat("fov", &fov, 0.1f, 180.0f);
				ImGui::Text("Water Options");
				ImGui::SliderFloat("Texture Movement Pace", &paceTex, -0.005f, 0.005f,"%.5f");
				ImGui::SliderFloat("Displacement Movement Pace", &paceDis, -0.005f, 0.005f, "%.5f");
				ImGui::SliderFloat("Maximum", &max, -0.2f, 0.2f, "%.5f");
				ImGui::SliderFloat("Texture Alpha", &alpha, 0.0f, 1.0f);
				ImGui::SliderFloat3("background Color", bgColor, 0.0f, 1.0f);
				ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
			}
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		//Buffer swapping
			glfwSwapBuffers(window);

		//Poll and process events
			glfwPollEvents();

	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	glfwTerminate();
	return 0;
}

GLFWwindow* initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(
		glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
		glfwGetVideoMode(glfwGetPrimaryMonitor())->height,
		"CG",
		glfwGetPrimaryMonitor(),
		NULL);

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

	glEnable(GL_DEPTH_TEST);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}

void processInput(GLFWwindow *window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	float cameraCurrSpeed = cameraSpeed * deltaTime;
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraCurrSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraCurrSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraCurrSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraCurrSpeed;
	}
	
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; 
	lastX = xpos;
	lastY = ypos;

	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}