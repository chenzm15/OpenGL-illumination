#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "shader.h"
#include "Camera.h"

#include <iostream>

#define BUFFER_OFFSET(offset) ((void *)(offset))

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), Projection_Type::PERSPECTIVE);

// lamp
glm::vec3 lampPos(0.0f, 3.8f, -3.5f);

bool mousePressed = false;
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback); // 注册光标移动回调函数
	glfwSetScrollCallback(window, scroll_callback);   // 注册鼠标滚轮回调函数
	glfwSetMouseButtonCallback(window, mouse_button_callback); // 注册鼠标按键回调函数

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	Shader objShader("object.vs", "object.fs"), lampShader("lamp.vs", "lamp.fs"), windowShader("window.vs", "window.fs");

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,

		/*-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,*/

		-0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f
	};

	float window_vertices[] = {
		-0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		 //0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
		 //0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
		 //0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
		 //0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
		 //0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
		 //0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
	};

	GLuint objVAO, VBO;
	glGenVertexArrays(1, &objVAO);
	glBindVertexArray(objVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float))); // 法向量参数配置
	glEnableVertexAttribArray(1);

	GLuint lampVAO;
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // 立方体和灯共用一个顶点缓存对象(VBO)，数据在前面已经传输到这个VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	GLuint windowVAO, windowVBO;
	glGenVertexArrays(1, &windowVAO);
	glBindVertexArray(windowVAO);
	glGenBuffers(1, &windowVBO);
	glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(window_vertices), window_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	objShader.use();
	objShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	objShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	objShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
	objShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
	objShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	objShader.setFloat("material.shininess", 32.0f);
	objShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	objShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
	objShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	objShader.setVec3("light.position", lampPos);

	unsigned int diffuseMap = loadTexture("window6.jpg");

	windowShader.use();
	windowShader.setInt("material.diffuse", 0);
	windowShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	windowShader.setFloat("material.shininess", 64.0f);
	windowShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	windowShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
	windowShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	windowShader.setVec3("light.position", lampPos);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		glm::vec3 cameraPosition = camera.getCameraPosition();
		objShader.use();
		objShader.setVec3("viewPos", cameraPosition);
		glm::mat4 projection;
		// 正交投影需保持图形纵横比
		if (camera.getProjectionType() == Projection_Type::PERSPECTIVE)
			projection = glm::perspective(glm::radians(camera.getFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		else
			projection = glm::ortho(-1.0f * (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f * (float)SCR_WIDTH / (float)SCR_HEIGHT, -1.0f, 1.0f, 0.1f, 100.0f);

		objShader.setMat4("projection", projection);
		glm::mat4 view = camera.getViewMatrix();
		objShader.setMat4("view", view);
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(10.0f));
		objShader.setMat4("model", model);

		glBindVertexArray(objVAO);
		glDrawArrays(GL_TRIANGLES, 0, 30);

		windowShader.use();
		windowShader.setVec3("viewPos", camera.getCameraPosition());

		windowShader.setMat4("projection", projection);
		windowShader.setMat4("view", view);

		model = glm::mat4();
		model = glm::scale(model, glm::vec3(10.0f, 4.0f, 4.0f));
		model = glm::translate(model, glm::vec3(0.005f, 0.0f, 0.0f));
		windowShader.setMat4("model", model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glBindVertexArray(windowVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::mat4();
		model = glm::translate(model, lampPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lampShader.setMat4("model", model);

		glBindVertexArray(lampVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // 相机上移
		camera.ProcessKeyPressed(Camera_Movement::UP, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // 相机下移
		camera.ProcessKeyPressed(Camera_Movement::DOWN, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // 相机左移
		camera.ProcessKeyPressed(Camera_Movement::LEFT, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // 相机右移
		camera.ProcessKeyPressed(Camera_Movement::RIGHT, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // 相机前移
		camera.ProcessKeyPressed(Camera_Movement::FORWARD, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // 相机后移
		camera.ProcessKeyPressed(Camera_Movement::BACKWARD, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (mousePressed)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseDragging(xoffset, yoffset);
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// glfw: whenever the mouse buttons are pressed/released, this callback is called
// ----------------------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			mousePressed = true;
		}
		else
		{
			mousePressed = false;
			firstMouse = true;
		}
	}
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		printf("%d", nrComponents);
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
