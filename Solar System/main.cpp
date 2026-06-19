// Third party
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Helper Classes
#include "shader.h"
#include "camera.h"
#include "Sphere.h"
#include "common.h"
#include "solar_system.h"

// std
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void setup_VBO(Sphere& body);
void step_simulation(Shader& planetShader, Shader& sunShader);
void gravity();
void draw_sphere(Sphere& sphere);


// sphere
int nrRows = 7;
int nrColumns = 7;
float spacing = 2.5;
GLuint vaoId1, vaoId2;      // IDs of VAO for vertex array states
GLuint vboId1, vboId2;      // IDs of VBO for vertex arrays
GLuint iboId1, iboId2;      // IDs of VBO for index array
GLint attribVertexPosition = 0;
GLint attribVertexNormal = 1;
GLint attribVertexTexCoord = 2;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
Sphere sphere1(3.0f, 72, 36, true, 2);
Sphere sphere2(1.0f, 36, 18, true, 2);  // radius, sectors, stacks, smooth(default), Y-up

Planets solarSystem;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// -----------------------------
	Shader planetShader("planetShader.vs", "planetShader.fs");
	Shader sunShader("sunShader.vs", "sunShader.fs");

	// set planet masses
	// -----------------
	solarSystem.mercury.mass = 50.0;
	solarSystem.mars.mass = 75.0;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Process & Display Simulation
		// ----------------------------
		step_simulation(planetShader, sunShader);

		// Swap buffers
		// --------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// Process gravity and draw simulation
// -----------------------------------
void step_simulation(Shader& planetShader, Shader& sunShader)
{
	// Activate Shader
	planetShader.use();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	planetShader.setMat4("projection", projection);
	planetShader.setMat4("view", view);

	// Light uniforms
	glm::vec3 lightPos = glm::vec3(2.0f, 1.0f, 0.0f);
	glm::vec4 lightPosEye = view * glm::vec4(lightPos, 1.0f);
	planetShader.setVec4("lightPosition", lightPosEye);
	planetShader.setVec4("lightAmbient", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	planetShader.setVec4("lightDiffuse",glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
	planetShader.setVec4("lightSpecular",glm::vec4(0.0f));

	// Process Gravity
	// ---------------
	glm::vec3 mercuryPos = glm::vec3(-5.0f, -3.0f, -10.0f);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), mercuryPos);
	planetShader.setMat4("model", model);
	setup_VBO(solarSystem.mercury);
	glBindVertexArray(vaoId1);
	glDrawElements(GL_TRIANGLES, solarSystem.mercury.getIndexCount(), GL_UNSIGNED_INT, (void*)0);

	glm::vec3 marsPos = glm::vec3(5.0f, -3.0f, -10.0f);
	model = glm::translate(glm::mat4(1.0f), marsPos);
	planetShader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, solarSystem.mars.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
}

// Process gravity calculations
void gravity()
{
	
}

void draw_sphere(Sphere& sphere)
{

}


// process all input
// ----------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
// glfw: whenever the window size changed this is called
// ----------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// ----------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast <float>(yposIn);

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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

//////////////////////////////////////////////////////////////////////////////
// copy vertex data to VBO and VA state to VAO
///////////////////////////////////////////////////////////////////////////////
void setup_VBO(Sphere& body)
{
	// sphere1
	// create vertex array object to store all vertex array states only once
	if (!vaoId1)
		glGenVertexArrays(1, &vaoId1);
	glBindVertexArray(vaoId1);

	// create vertex buffer objects
	if (!vboId1)
		glGenBuffers(1, &vboId1);

	glBindBuffer(GL_ARRAY_BUFFER, vboId1);
	glBufferData(GL_ARRAY_BUFFER, body.getInterleavedVertexSize(), body.getInterleavedVertices(), GL_STATIC_DRAW);

	if (!iboId1)
		glGenBuffers(1, &iboId1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, body.getIndexSize(), body.getIndices(), GL_STATIC_DRAW);

	// enable vertex array attributes for bound VAO
	glEnableVertexAttribArray(attribVertexPosition);
	glEnableVertexAttribArray(attribVertexNormal);
	

	// store vertex array pointers to bound VAO
	int stride = body.getInterleavedStride();
	glVertexAttribPointer(attribVertexPosition, 3, GL_FLOAT, false, stride, 0);
	glVertexAttribPointer(attribVertexNormal, 3, GL_FLOAT, false, stride, (void*)(3 * sizeof(float)));
	glVertexAttribPointer(attribVertexTexCoord, 2, GL_FLOAT, false, stride, (void*)(6 * sizeof(float)));


	// unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
