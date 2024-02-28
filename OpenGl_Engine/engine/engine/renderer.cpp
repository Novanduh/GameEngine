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

GLuint VertexArrayID;
GLuint vertexbuffer;
GLuint uvbuffer;
GLuint programID;
GLuint MatrixID;
GLuint Texture;
GLuint TextureID;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> vertices2;
std::vector<glm::vec2> uvs;
std::vector<glm::vec2> uvs2;
std::vector<glm::vec3> normals;
std::vector<glm::vec3> normals2;




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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

bool closeWindow() {
	return !(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

void render(const char* path, const char* imagepath) {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertices2;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec2> uvs2;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> normals2;

	// Load the texture
	Texture = loadDDS(imagepath);

	// Read our .obj file
	 // Won't be used at the moment.

	bool res = loadOBJ(path, vertices, uvs, normals);

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i] += vec3(2, 0, 2);
	}

	res = loadOBJ("cube.obj", vertices2, uvs2, normals2);

	for (int i = 0; i < vertices2.size(); i++) {
		vertices2[i] += vec3(0, 0, 0);
	}

	// Load it into a VBO


	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, (vertices.size() + vertices2.size()) * sizeof(glm::vec3), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), &vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices2.size() * sizeof(glm::vec3), &vertices2[0]);



	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	//glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, (uvs.size() * sizeof(glm::vec2)) + uvs2.size() * sizeof(glm::vec2), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, uvs.size() * sizeof(glm::vec2), &uvs[0]);
	glBufferSubData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs2.size() * sizeof(glm::vec2), &uvs2[0]);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		cameraControl();

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
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

		// 2nd attribute buffer : UVs
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

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size()+vertices2.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (!closeWindow());
		
}

void cleanUp() {
	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

void main(void)
{
	initRenderer();

	render("viking_room.obj", "3DLABbg_UV_Map_Checker_05_1024_1024.dds");

	cleanUp();
}
