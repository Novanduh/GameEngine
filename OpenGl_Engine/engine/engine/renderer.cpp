//Base code from https://www.opengl-tutorial.org/download/
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>


// Include GLEW
#include <glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;
#define GLM_ENABLE_EXPERIMENTAL
// Include GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm;

#include <shader.hpp>
#include <texture.hpp>
#include <controls.hpp>
#include <objloader.hpp>
#include "LightingSystem.hpp"
#include "Light.hpp"

GLuint VertexArrayID;
GLuint vertexbuffer;
GLuint uvbuffer;
GLuint normalbuffer;
GLuint programID;
GLuint MatrixID;
GLuint Texture;
GLuint TextureID;

GLuint AmbientID;
GLuint DiffuseID;
GLuint SpecularID;
GLuint LightID;
GLuint ShineID;

std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;
std::vector<const char*> paths;
std::vector<glm::vec3> positions;
std::vector<const char*> textures;
LightingSystem lightSystem;


void initRenderer() {
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		//getchar();
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make macOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1240, 780, "House Explorer 3D Deluxe Ultimate Edition", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		//getchar();
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		//getchar();
		glfwTerminate();
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("VertexShader.vertexshader", "FragmentShader.fragmentshader");
	// Get a handle for our "myTextureSampler" uniform
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");

	AmbientID = glGetUniformLocation(programID, "ambient");
	DiffuseID = glGetUniformLocation(programID, "diffuse");
	SpecularID = glGetUniformLocation(programID, "specular");
	LightID = glGetUniformLocation(programID, "lightPosition");
	ShineID = glGetUniformLocation(programID, "shininess");

	Light light1(vec3(20, 20, 20), vec3(0.1, 0.0, 0.1), vec3(0.0, 0.3, 0.5), vec3(0.0, 0.0, 0.5));
	Light light2(vec3(5, 10, 5), vec3(0.5, 0.2, 0.7), vec3(0.5, 0.2, 0.7), vec3(0.5, 0.2, 0.7));

	lightSystem.addLight(light1);
	lightSystem.addLight(light2);
}

void cameraControl() {
	// Compute the MVP matrix from keyboard and mouse input
	computeMatricesFromInputs();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
}

void lightControl() {
	lightSystem.calcLight();
	glUniform3fv(AmbientID, 1, &lightSystem.getAmbientProduct()[0][0]);
	glUniform3fv(DiffuseID, 1, &lightSystem.getDiffuseProduct()[0][0]);
	glUniform3fv(SpecularID, 1, &lightSystem.getSpecularProduct()[0][0]);
	glUniform3fv(LightID, 1, &lightSystem.lights[0].getLightPosition()[0]);
	glUniform1f(ShineID, 50.0f);
}

bool closeWindow() {
	return !(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

void render(const char* path, glm::vec3 position, const char* texture) {

	glUseProgram(programID);

	cameraControl();

	vertices.clear();
	uvs.clear();
	normals.clear();

	lightControl();

		bool res = loadOBJ(path, vertices, uvs, normals);

		for (int i = 0; i < vertices.size(); i++) {
			vertices[i] += position;
		}

		Texture = loadDDS(texture);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(TextureID, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void GameLoop() {
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (int i = 0; i < paths.size(); i++)
			render(paths[i], positions[i], textures[i]);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	while (!closeWindow());
		
}

void cleanUp() {
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();
}

void main()
{ 
	initRenderer();

	//test scene
	paths.push_back("bed.obj");
	positions.push_back(vec3(2, 0, -2));
	textures.push_back("CustomUVChecker_byValle_2K.dds");
	paths.push_back("cube.obj");
	positions.push_back(vec3(10, 0, -2));
	textures.push_back("uvmap.dds");
	paths.push_back("viking_room.obj");
	positions.push_back(vec3(2, 0, -10));
	textures.push_back("viking_room.dds");
	/////////////
	
	GameLoop();

	cleanUp();
}
