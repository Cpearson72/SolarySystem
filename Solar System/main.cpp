// Third party
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

// Helper Classes
#include "shader.h"
#include "camera.h"
#include "Sphere.h"
#include "common.h"
#include "solar_system.h"

// std
#include <iostream>
#include <string>
#include<filesystem>
namespace fs = std::filesystem;

// Function Prototypes
// --------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void setup_VBO(Sphere& body);
void step_simulation(Shader& planetShader, Shader& sunShader);
void gravity();
void draw_skybox(Shader& skyBoxShader);
void skybox_VBO();
void load_skybox_textures();
void load_sphere_texture(Sphere& body, Shader& planetShader);
void load_texture_paths();


// sphere stuff. TODO: MOVE THIS
// -----------------------------
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
Planets solarSystem;

// skybox stuff. TODO: MOVE THIS
// ----------------------------
GLuint vaoSkyBox;
GLuint vboSkyBox;
GLuint eboSkyBox;



// Main
// -----
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
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	//GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Solar System", monitor, NULL);
	//SCR_WIDTH = mode->width;
	//SCR_HEIGHT = mode->height;
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System", NULL, NULL);
	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;
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

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DITHER);
	glEnable(GL_MULTISAMPLE);

	// build and compile our shader zprogram
	// -----------------------------
	Shader planetShader("planetShader.vs", "planetShader.fs");
	Shader sunShader("sunShader.vs", "sunShader.fs");
	Shader skyBoxShader("skyBox.vs", "skyBox.fs");

	// set planet masses
	// -----------------
	// Scales from https://cass.ucsd.edu/archive/personal/susan/origins/1.5_1.html
	set_mass(solarSystem);
	set_position(solarSystem);
	calc_initial_orbital_velocities(solarSystem);

	// Textures
	// --------
	load_skybox_textures();
	load_texture_paths();
	
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
		setup_VBO(solarSystem.earth);
		step_simulation(planetShader, sunShader);
		skybox_VBO();
		draw_skybox(skyBoxShader);

		// Swap buffers
		// --------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// Render skybox and apply textures
// --------------------------------
void draw_skybox(Shader& skyBoxShader)
{
	glDepthFunc(GL_LEQUAL);
	skyBoxShader.use();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 10000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	skyBoxShader.setMat4("projection", projection);
	skyBoxShader.setMat4("view", view);

	glBindVertexArray(vaoSkyBox);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	glDepthFunc(GL_LESS);

}

// Process gravity and draw simulation
// -----------------------------------
void step_simulation(Shader& planetShader, Shader& sunShader)
{
	// Activate Shader
	planetShader.use();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 10000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	planetShader.setMat4("projection", projection);
	planetShader.setMat4("view", view);

	// Light uniforms
	glm::vec4 sunPos = view * glm::vec4(solarSystem.sun.position, 1.0f);
	planetShader.setVec4("lightPosition", sunPos);
	planetShader.setVec4("lightAmbient", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	planetShader.setVec4("lightDiffuse",glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

	// Process Gravity
	// ---------------
	gravity();
	glBindVertexArray(vaoId1);
	
	// Planets
	// -------
	std::vector<Sphere> bodyVector = solarSystem.planet_to_vector();
	for (auto& body: bodyVector)
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), body.position);
		float radius = body.getRadius();
		model = glm::scale(model, glm::vec3(radius));
		planetShader.setMat4("model", model);

		// texture
		load_sphere_texture(body, planetShader);
		glDrawElements(GL_TRIANGLES, body.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
	}

	// Sun
	// -------
	sunShader.use();
	sunShader.setMat4("projection", projection);
	sunShader.setMat4("view", view);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), solarSystem.sun.position);
	float radius = solarSystem.sun.getRadius();
	model = glm::scale(model, glm::vec3(radius));
	sunShader.setMat4("model", model);
	load_sphere_texture(solarSystem.sun, planetShader);
	glDrawElements(GL_TRIANGLES, solarSystem.sun.getIndexCount(), GL_UNSIGNED_INT, (void*)0);

}

// Process gravity calculations
const float G = 6.6743e-11f;
double TIME_SCALE = 31557600; // 1 day per second
void gravity() {
	std::vector<Sphere> planetVector = solarSystem.planet_to_vector();
	for (auto& planet : planetVector) {

		// Convert AU scale to meters for Gravity Calculations
		// ---------------------------------------------------
		double radius_AU = glm::distance(solarSystem.sun.position, planet.position); 
		double radius_meters = radius_AU * AU;
		// Planet Acceleration
		// --------------------
		double netForce = (G * solarSystem.sun.mass * planet.mass) / (radius_meters * radius_meters); 
		double planet_scalar = netForce / planet.mass;
		double acceleration_AU = planet_scalar / AU;
		// glm units
		// ----------
		glm::vec3 gravityDir = glm::normalize(solarSystem.sun.position - planet.position); 
		glm::vec3 acceleration_planet = gravityDir * static_cast<float>(acceleration_AU); 

		
		float dt = deltaTime * TIME_SCALE;
		planet.velocity += acceleration_planet * dt; planet.position += planet.velocity * dt;
	}
	solarSystem.from_planet_vector(planetVector);
}

// process all input
// ----------------------
void processInput(GLFWwindow* window)
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

	// gravity speed
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.MovementSpeed += 0.02;
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		TIME_SCALE += 30000;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		if (camera.MovementSpeed > 0)
			camera.MovementSpeed -= 0.02;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if ((TIME_SCALE - 30000) < 0)
		{
			TIME_SCALE = 0;
		}
		else 
		{
			TIME_SCALE -= 30000;
		}
	}

	// reset planets
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		calc_initial_orbital_velocities(solarSystem);
		set_position(solarSystem);

	}

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

void skybox_VBO()
{	
	// VAO
	// ---
	if (!vaoSkyBox)
		glGenVertexArrays(1, &vaoSkyBox);
	glBindVertexArray(vaoSkyBox);

	// VBO
	// ---
	if (!vboSkyBox)
		glGenBuffers(1, &vboSkyBox);
	glBindBuffer(GL_ARRAY_BUFFER, vboSkyBox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	// EBO
	// ---
	if (!eboSkyBox)
		glGenBuffers(1, &eboSkyBox);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboSkyBox);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(attribVertexPosition);
	glVertexAttribPointer(attribVertexPosition, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);

	// Unbined
	// -------
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void load_skybox_textures()
{
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string facesCubemap[6] =
	{
		parentDir + "/Resources/sky_box/right.png",
		parentDir + "/Resources/sky_box/left.png",
		parentDir + "/Resources/sky_box/top.png",
		parentDir + "/Resources/sky_box/bottom.png",
		parentDir + "/Resources/sky_box/front.png",
		parentDir + "/Resources/sky_box/back.png",
	};
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	stbi_set_flip_vertically_on_load(false);
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		if (data)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGBA,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load teture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}
}

void load_sphere_texture(Sphere& body, Shader& planetShader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, body.textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glUniform1i(glGetUniformLocation(planetShader.ID, "sphereMap"), 0);

}

void load_texture_paths()
{
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string bodyDir[10] =
	{
		parentDir + "/Resources/2k_sun.jpg",
		parentDir + "/Resources/8k_mercury.jpg",
		parentDir + "/Resources/8k_venus_surface.jpg",
		parentDir + "/Resources/earth/8k_earth_daymap.jpg",
		parentDir + "/Resources/8k_mars.jpg",
		parentDir + "/Resources/8k_jupiter.jpg",
		parentDir + "/Resources/8k_saturn.jpg",
		parentDir + "/Resources/2k_uranus.jpg",
		parentDir + "/Resources/2k_neptune.jpg",
		parentDir + "/Resources/4k_pluto_fake.jpg"
	};
	solarSystem.sun.texturePath = bodyDir[0];
	solarSystem.mercury.texturePath = bodyDir[1];
	solarSystem.venus.texturePath = bodyDir[2];
	solarSystem.earth.texturePath = bodyDir[3];
	solarSystem.mars.texturePath = bodyDir[4];
	solarSystem.jupiter.texturePath = bodyDir[5];
	solarSystem.saturn.texturePath = bodyDir[6];
	solarSystem.uranus.texturePath = bodyDir[7];
	solarSystem.neptune.texturePath = bodyDir[8];
	solarSystem.pluto.texturePath = bodyDir[9];
	stbi_set_flip_vertically_on_load(true);
	std::vector<Sphere> bodies = solarSystem.to_vector();
	for (auto& body : bodies) {
		int width, height, nrChannels;
		unsigned char* data = stbi_load(body.texturePath.c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			glGenTextures(1, &body.textureID);
			glBindTexture(GL_TEXTURE_2D, body.textureID);
			GLenum format = GL_RGB;
			if (nrChannels == 1) format = GL_RED;
			else if (nrChannels == 3) format = GL_RGB;
			else if (nrChannels == 4) format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture at path: " << body.texturePath.c_str() << std::endl;
			std::cout << "Reason: " << stbi_failure_reason() << std::endl;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	solarSystem.from_vector(bodies);


}