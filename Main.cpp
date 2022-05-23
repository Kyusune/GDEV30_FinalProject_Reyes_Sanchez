// Quick note: GLAD needs to be included first before GLFW.
// Otherwise, GLAD will complain about gl.h being already included.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// ---------------
// Function declarations
// ---------------

/**
 * @brief Creates a shader program based on the provided file paths for the vertex and fragment shaders.
 * @param[in] vertexShaderFilePath Vertex shader file path
 * @param[in] fragmentShaderFilePath Fragment shader file path
 * @return OpenGL handle to the created shader program
 */
GLuint CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

/**
 * @brief Creates a shader based on the provided shader type and the path to the file containing the shader source.
 * @param[in] shaderType Shader type
 * @param[in] shaderFilePath Path to the file containing the shader source
 * @return OpenGL handle to the created shader
 */
GLuint CreateShaderFromFile(const GLuint& shaderType, const std::string& shaderFilePath);

/**
 * @brief Creates a shader based on the provided shader type and the string containing the shader source.
 * @param[in] shaderType Shader type
 * @param[in] shaderSource Shader source string
 * @return OpenGL handle to the created shader
 */
GLuint CreateShaderFromSource(const GLuint& shaderType, const std::string& shaderSource);

/**
 * @brief Function for handling the event when the size of the framebuffer changed.
 * @param[in] window Reference to the window
 * @param[in] width New width
 * @param[in] height New height
 */
void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height);

/**
 * Struct containing data about a vertex
 */
struct Vertex
{
	GLfloat x, y, z;	// Position
	GLubyte r, g, b;	// Color
	GLfloat u, v;		// UV coordinates
	GLfloat nx, ny, nz; // Normal Vectors
};

//Global Variable Declarations for Rotation and Lighting
float seconds = 0.0f;
glm::vec3 c_lightPos(0.0f, 5.0f, 0.0f);
glm::vec3 camerapos(0.0f, 5.5f, 1.25f);
float ambientIntensity = 0.2f;
float diffuseIntensity = 0.6f;
float specularIntensity = 1.0f;
float diffuseMatComponent = 0.0f;
float specularMatComponent = 0.0f;
float shine = 2.0f;


/**
 * @brief Main function
 * @return An integer indicating whether the program ended successfully or not.
 * A value of 0 indicates the program ended succesfully, while a non-zero value indicates
 * something wrong happened during execution.
 */

void parameter(float) {

}
int main()
{
	// Initialize GLFW
	int glfwInitStatus = glfwInit();
	if (glfwInitStatus == GLFW_FALSE)
	{
		std::cerr << "Failed to initialize GLFW!" << std::endl;
		return 1;
	}

	// Tell GLFW that we prefer to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW that we prefer to use the modern OpenGL
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Tell GLFW to create a window
	int windowWidth = 800;
	int windowHeight = 600;
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Yae", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Tell GLFW to use the OpenGL context that was assigned to the window that we just created
	glfwMakeContextCurrent(window);

	// Register the callback function that handles when the framebuffer size has changed
	glfwSetFramebufferSizeCallback(window, FramebufferSizeChangedCallback);

	// Tell GLAD to load the OpenGL function pointers
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return 1;
	}

	// --- Vertex specification ---
	
	// Set up the data for each vertex of the triangle
	Vertex vertices[24];
	//Front Facing Square
	vertices[0].x = -3.0f;	vertices[0].y = 1.0f;	vertices[0].z = 2.0f;
	vertices[0].r = 255;	vertices[0].g = 0;		vertices[0].b = 0;
	vertices[0].u = 0.0f;   vertices[0].v = 0.0f;
	vertices[0].nx = 0.0f;	vertices[0].ny = 0.0f;	vertices[0].nz = 1.0f;

	vertices[1].x = -3.0f;	vertices[1].y = 3.0f;	vertices[1].z = 2.0f;
	vertices[1].r = 0;		vertices[1].g = 255;	vertices[1].b = 0;
	vertices[1].u = 1.0f;   vertices[1].v = 0.0f;
	vertices[1].nx = 0.0f;	vertices[1].ny = 0.0f;	vertices[1].nz = 1.0f;

	vertices[2].x = -5.0f;	vertices[2].y = 1.0f;	vertices[2].z = 2.0f;
	vertices[2].r = 0;		vertices[2].g = 0;		vertices[2].b = 255;
	vertices[2].u = 0.0f;   vertices[2].v = 1.0f;
	vertices[2].nx = 0.0f;	vertices[2].ny = 0.0f;	vertices[2].nz = 1.0f;

	vertices[3].x = -5.0f;	vertices[3].y = 3.0f;	vertices[3].z = 2.0f;
	vertices[3].r = 0;		vertices[3].g = 0;		vertices[3].b = 255;
	vertices[3].u = 1.0f;   vertices[3].v = 1.0f;
	vertices[3].nx = 0.0f;	vertices[3].ny = 0.0f;	vertices[3].nz = 1.0f;

	//Back Facing Square
	vertices[4].x = -3.0f;	vertices[4].y = 1.0f;	vertices[4].z = 0.0f;
	vertices[4].r = 0;		vertices[4].g = 0;		vertices[4].b = 0;
	vertices[4].u = 0.0f;   vertices[4].v = 0.0f;
	vertices[4].nx = 0.0f;	vertices[4].ny = 0.0f;	vertices[4].nz = -1.0f;

	vertices[5].x = -3.0f;	vertices[5].y = 3.0f;	vertices[5].z = 0.0f;
	vertices[5].r = 0;		vertices[5].g = 0;		vertices[5].b = 0;
	vertices[5].u = 1.0f;   vertices[5].v = 0.0f;
	vertices[5].nx = 0.0f;	vertices[5].ny = 0.0f;	vertices[5].nz = -1.0f;

	vertices[6].x = -5.0f;	vertices[6].y = 1.0f;	vertices[6].z = 0.0f;
	vertices[6].r = 0;		vertices[6].g = 0;		vertices[6].b = 255;
	vertices[6].u = 0.0f;   vertices[6].v = 1.0f;
	vertices[6].nx = 0.0f;	vertices[6].ny = 0.0f;	vertices[6].nz = -1.0f;

	vertices[7].x = -5.0f;	vertices[7].y = 3.0f;	vertices[7].z = 0.0f;
	vertices[7].r = 0;		vertices[7].g = 0;		vertices[7].b = 255;
	vertices[7].u = 1.0f;   vertices[7].v = 1.0f;
	vertices[7].nx = 0.0f;	vertices[7].ny = 0.0f;	vertices[7].nz = -1.0f;

	//Right Facing Square
	vertices[8].x = -3.0f;	vertices[8].y = 3.0f;	vertices[8].z = 0.0f;
	vertices[8].r = 255;	vertices[8].g = 0;		vertices[8].b = 0;
	vertices[8].u = 0.0f;   vertices[8].v = 0.0f;
	vertices[8].nx = 1.0f;	vertices[8].ny = 0.0f;	vertices[8].nz = 0.0f;

	vertices[9].x = -3.0f;	vertices[9].y = 1.0f;	vertices[9].z = 0.0f;
	vertices[9].r = 0;		vertices[9].g = 255;	vertices[9].b = 0;
	vertices[9].u = 1.0f;   vertices[9].v = 0.0f;
	vertices[9].nx = 1.0f;	vertices[9].ny = 0.0f;	vertices[9].nz = 0.0f;

	vertices[10].x = -3.0f;	vertices[10].y = 3.0f;	vertices[10].z = 2.0f;
	vertices[10].r = 255;	vertices[10].g = 0;		vertices[10].b = 0;
	vertices[10].u = 0.0f;  vertices[10].v = 1.0f;
	vertices[10].nx = 1.0f;	vertices[10].ny = 0.0f; vertices[10].nz = 0.0f;

	vertices[11].x = -3.0f;	vertices[11].y = 1.0f;	vertices[11].z = 2.0;
	vertices[11].u = 1.0f;  vertices[11].v = 1.0f;
	vertices[11].nx = 1.0f;	vertices[11].ny = 0.0f;vertices[11].nz = 0.0f;

	//Top Facing Square
	vertices[12].x = -3.0f;	vertices[12].y = 3.0f;	vertices[12].z = 0.0f;
	vertices[12].r = 0;		vertices[12].g = 0;		vertices[12].b = 255;
	vertices[12].u = 0.0f;  vertices[12].v = 0.0f;
	vertices[12].nx = 0.0f;	vertices[12].ny = 1.0f; vertices[12].nz = 0.0f;

	vertices[13].x = -3.0f;	vertices[13].y = 3.0f;	vertices[13].z = 2.0f;
	vertices[13].r = 0;		vertices[13].g = 0;		vertices[13].b = 255;
	vertices[13].u = 1.0f;   vertices[13].v = 0.0f;
	vertices[13].nx = 0.0f;	vertices[13].ny = 1.0f; vertices[13].nz = 0.0f;

	vertices[14].x = -5.0f;	vertices[14].y = 3.0f;	vertices[14].z = 0.0f;
	vertices[14].r = 0;		vertices[14].g = 255;	vertices[14].b = 0;
	vertices[14].u = 0.0f;   vertices[14].v = 1.0f;
	vertices[14].nx = 0.0f;	vertices[14].ny = 1.0f; vertices[14].nz = 0.0f;

	vertices[15].x = -5.0f;	vertices[15].y = 3.0f;	vertices[15].z = 2.0f;
	vertices[15].r = 0;		vertices[15].g = 255;	vertices[15].b = 0;
	vertices[15].u = 1.0f;   vertices[15].v = 1.0f;
	vertices[15].nx = 0.0f;	vertices[15].ny = 1.0f; vertices[15].nz = 0.0f;

	//Left Facing Square
	vertices[16].x = -5.0f;	vertices[16].y = 1.0f;	vertices[16].z = 2.0f;
	vertices[16].r = 0;		vertices[16].g = 0;		vertices[16].b = 255;
	vertices[16].u = 0.0f;   vertices[16].v = 0.0f;
	vertices[16].nx = -1.0f;	vertices[16].ny = 0.0f; vertices[16].nz = 0.0f;

	vertices[17].x = -5.0f;	vertices[17].y = 3.0f;	vertices[17].z = 2.0f;
	vertices[17].r = 0;		vertices[17].g = 0;		vertices[17].b = 255;
	vertices[17].u = 1.0f;   vertices[17].v = 0.0f;
	vertices[17].nx = -1.0f;	vertices[17].ny = 0.0f; vertices[17].nz = 0.0f;

	vertices[18].x = -5.0f;	vertices[18].y = 1.0f;	vertices[18].z = 0.0f;
	vertices[18].r = 0;		vertices[18].g = 0;		vertices[18].b = 255;
	vertices[18].u = 0.0f;   vertices[18].v = 1.0f;
	vertices[18].nx = -1.0f;	vertices[18].ny = 0.0f; vertices[18].nz = 0.0f;

	vertices[19].x = -5.0f;	vertices[19].y = 3.0f;	vertices[19].z = 0.0f;
	vertices[19].r = 0;		vertices[19].g = 0;		vertices[19].b = 255;
	vertices[19].u = 1.0f;   vertices[19].v = 1.0f;
	vertices[19].nx = -1.0f;	vertices[19].ny = 0.0f; vertices[19].nz = 0.0f;

	//Bottom Facing Square
	vertices[20].x = -3.0f;	vertices[20].y = 1.0f;	vertices[20].z = 2.0f;
	vertices[20].r = 0;		vertices[20].g = 0;		vertices[20].b = 0;
	vertices[20].u = 0.0f;   vertices[20].v = 0.0f;
	vertices[20].nx = 0.0f;	vertices[20].ny = 0.0f;	vertices[20].nz = -1.0f;

	vertices[21].x = -3.0f;	vertices[21].y = 1.0f;	vertices[21].z = 0.0f;
	vertices[21].r = 0;		vertices[21].g = 0;		vertices[21].b = 0;
	vertices[21].u = 1.0f;   vertices[21].v = 0.0f;
	vertices[21].nx = 0.0f;	vertices[21].ny = 0.0f;	vertices[21].nz = -1.0f;

	vertices[22].x = -5.0f;	vertices[22].y = 1.0f;	vertices[22].z = 2.0f;
	vertices[22].r = 0;		vertices[22].g = 0;		vertices[22].b = 255;
	vertices[22].u = 0.0f;   vertices[22].v = 1.0f;
	vertices[22].nx = 0.0f;	vertices[22].ny = 0.0f;	vertices[22].nz = -1.0f;

	vertices[23].x = -5.0f;	vertices[23].y = 1.0f;	vertices[23].z = 0.0f;
	vertices[23].r = 0;		vertices[23].g = 0;		vertices[23].b = 255;
	vertices[23].u = 1.0f;   vertices[23].v = 1.0f;
	vertices[23].nx = 0.0f;	vertices[23].ny = 0.0f;	vertices[23].nz = -1.0f;

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create a vertex array object that contains data on how to map vertex attributes
	// (e.g., position, color) to vertex shader properties.
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Vertex attribute 0 - Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Vertex attribute 1 - Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));

	// Vertex attribute 2 - UV coordinate
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, u)));

	// Vertex attribute 3 - Normal Vertex
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, nx)));

	glBindVertexArray(0);

	// Create a shader program
	GLuint program = CreateShaderProgram("main.vsh", "main.fsh");

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Tell OpenGL the dimensions of the region where stuff will be drawn.
	// For now, tell OpenGL to use the whole screen
	glViewport(0, 0, windowWidth, windowHeight);

	// Create a variable that will contain the ID for our texture,
	// and use glGenTextures() to generate the texture itself
	GLuint tex;
	glGenTextures(1, &tex);

	// --- Load our image using stb_image ---

	// Im image-space (pixels), (0, 0) is the upper-left corner of the image
	// However, in u-v coordinates, (0, 0) is the lower-left corner of the image
	// This means that the image will appear upside-down when we use the image data as is
	// This function tells stbi to flip the image vertically so that it is not upside-down when we use it
	stbi_set_flip_vertically_on_load(true);

	// 'imageWidth' and imageHeight will contain the width and height of the loaded image respectively
	int imageWidth, imageHeight, numChannels;

	// Read the image data and store it in an unsigned char array
	unsigned char* imageData = stbi_load("YaeMiko.jpg", &imageWidth, &imageHeight, &numChannels, 0);

	// Make sure that we actually loaded the image before uploading the data to the GPU
	if (imageData != nullptr)
	{
		// Our texture is 2D, so we bind our texture to the GL_TEXTURE_2D target
		glBindTexture(GL_TEXTURE_2D, tex);

		// Set the filtering methods for magnification and minification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Set the wrapping method for the s-axis (x-axis) and t-axis (y-axis)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Upload the image data to GPU memory
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		// If we set minification to use mipmaps, we can tell OpenGL to generate the mipmaps for us
		//glGenerateMipmap(GL_TEXTURE_2D);

		// Once we have copied the data over to the GPU, we can delete
		// the data on the CPU side, since we won't be using it anymore
		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear the colors in our off-screen framebuffer
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		//Transformation "Globals"
		glm::mat4 PerspectiveProj = glm::perspective(1.57f, 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 camera = glm::lookAt(glm::vec3(0.0f, 15.0f, 10.0f), glm::vec3(0.0f,10.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
		GLint uniformLocation = glGetUniformLocation(program, "mvp");
		GLint texUniformLocation = glGetUniformLocation(program, "tex");
		//GLint normalMuniformLocation = glGetUniformLocation(program, "normalM");
		//GLint modelUniformLocation = glGetUniformLocation(program, "model");
		//GLint lightPosUniformLocation = glGetUniformLocation(program, "lightPos");
		//GLint ambientIntensityUniformLocation = glGetUniformLocation(program, "ambientIntensity");
		//GLint diffuseIntensityUniformLocation = glGetUniformLocation(program, "diffuseIntensity");
		//GLint specularIntensityUniformLocation = glGetUniformLocation(program, "specularIntensity");
		//GLint cameraLocation = glGetUniformLocation(program, "camera");
		//GLint diffMatLocation = glGetUniformLocation(program, "diffMat");
		//GLint specMatLocation = glGetUniformLocation(program, "specMat");
		//GLint shineLocation = glGetUniformLocation(program, "shine");
		//glUniform3f(cameraLocation, camerapos.x, camerapos.y, camerapos.z);
		//glUniform3f(lightPosUniformLocation, c_lightPos.x, c_lightPos.y, c_lightPos.z);
		//glUniform1f(ambientIntensityUniformLocation, ambientIntensity);
		//glUniform1f(diffuseIntensityUniformLocation, diffuseIntensity);
		//glUniform1f(specularIntensityUniformLocation, specularIntensity);
		//glUniform1f(diffMatLocation, diffuseMatComponent);
		//glUniform1f(specMatLocation, specularMatComponent);
		//glUniform1f(shineLocation, shine);

		//Left Torii Base
		//Transformations
		glm::mat4 LTB = glm::mat4(1.0f);
		//mat = glm::scale(mat, glm::vec3(0.5f, 0.5f, 0.5f));
		//mat = glm::translate(mat, glm::vec3(0.5f, 0.0f, 0.0f));
		//glm::mat3 normalM1 = glm::transpose(glm::inverse(glm::mat3(mat)));
		//glm::mat4 model1 = mat;
		//glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model1));
		//glUniformMatrix3fv(normalMuniformLocation, 1 , GL_TRUE, glm::value_ptr(normalM1));
		LTB = PerspectiveProj * camera * LTB;
    	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(LTB));

		// Use the shader program that we created
		glUseProgram(program);

		// Use the vertex array object that we created
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glUniform1i(texUniformLocation, 0);

		// Draw the vertices
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

		// "Unuse" the vertex array object
		glBindVertexArray(0);

		//Left Torii Pillar
		//Transformations
		glm::mat4 LTP = glm::mat4(1.0f);
		LTP = glm::translate(LTP, glm::vec3(-1.0f, -3.5f, 0.125f));
		LTP = glm::scale(LTP, glm::vec3(0.75f, 6.0f, 0.75f));
		//glm::mat3 normalM1 = glm::transpose(glm::inverse(glm::mat3(mat)));
		//glm::mat4 model1 = mat;
		//glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model1));
		//glUniformMatrix3fv(normalMuniformLocation, 1 , GL_TRUE, glm::value_ptr(normalM1));
		LTP = PerspectiveProj * camera * LTP;
    	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(LTP));

		// Use the shader program that we created
		glUseProgram(program);

		// Use the vertex array object that we created
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glUniform1i(texUniformLocation, 0);

		// Draw the the vertices
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

		// "Unuse" the vertex array object
		glBindVertexArray(0);

		//Right Torii Base
		//Transformations
		glm::mat4 RTB = glm::mat4(1.0f);
		//mat = glm::scale(mat, glm::vec3(0.5f, 0.5f, 0.5f));
		RTB = glm::translate(RTB, glm::vec3(6.0f, 0.0f, 0.0f));
		//glm::mat3 normalM1 = glm::transpose(glm::inverse(glm::mat3(mat)));
		//glm::mat4 model1 = mat;
		//glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model1));
		//glUniformMatrix3fv(normalMuniformLocation, 1 , GL_TRUE, glm::value_ptr(normalM1));
		RTB = PerspectiveProj * camera * RTB;
    	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(RTB));

		// Use the shader program that we created
		glUseProgram(program);

		// Use the vertex array object that we created
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glUniform1i(texUniformLocation, 0);

		// Draw the vertices
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

		// "Unuse" the vertex array object
		glBindVertexArray(0);

		//Right Torii Pillar
		//Transformations
		glm::mat4 RTP = glm::mat4(1.0f);
		RTP = glm::scale(RTP, glm::vec3(0.75f, 6.0f, 0.75f));
		RTP = glm::translate(RTP, glm::vec3(6.0f, 0.0f, 0.0f));
		RTP = glm::translate(RTP, glm::vec3(0.625f, -0.575f, 0.125f));
		//glm::mat3 normalM1 = glm::transpose(glm::inverse(glm::mat3(mat)));
		//glm::mat4 model1 = mat;
		//glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model1));
		//glUniformMatrix3fv(normalMuniformLocation, 1 , GL_TRUE, glm::value_ptr(normalM1));
		RTP = PerspectiveProj * camera * RTP;
    	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(RTP));

		// Use the shader program that we created
		glUseProgram(program);

		// Use the vertex array object that we created
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glUniform1i(texUniformLocation, 0);

		// Draw the the vertices
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

		// "Unuse" the vertex array object
		glBindVertexArray(0);

		// Tell GLFW to swap the screen buffer with the offscreen buffer
		glfwSwapBuffers(window);

		// Tell GLFW to process window events (e.g., input events, window closed events, etc.)
		glfwPollEvents();

	}

	// --- Cleanup ---

	// Make sure to delete the shader program
	glDeleteProgram(program);

	// Delete the VBO that contains our vertices
	glDeleteBuffers(1, &vbo);

	// Delete the vertex array object
	glDeleteVertexArrays(1, &vao);

	// Remember to tell GLFW to clean itself up before exiting the application
	glfwTerminate();

	return 0;
}

/**
 * @brief Creates a shader program based on the provided file paths for the vertex and fragment shaders.
 * @param[in] vertexShaderFilePath Vertex shader file path
 * @param[in] fragmentShaderFilePath Fragment shader file path
 * @return OpenGL handle to the created shader program
 */
GLuint CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	GLuint vertexShader = CreateShaderFromFile(GL_VERTEX_SHADER, vertexShaderFilePath);
	GLuint fragmentShader = CreateShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderFilePath);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glDetachShader(program, vertexShader);
	glDeleteShader(vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(fragmentShader);

	// Check shader program link status
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		char infoLog[512];
		GLsizei infoLogLen = sizeof(infoLog);
		glGetProgramInfoLog(program, infoLogLen, &infoLogLen, infoLog);
		std::cerr << "program link error: " << infoLog << std::endl;
	}

	return program;
}

/**
 * @brief Creates a shader based on the provided shader type and the path to the file containing the shader source.
 * @param[in] shaderType Shader type
 * @param[in] shaderFilePath Path to the file containing the shader source
 * @return OpenGL handle to the created shader
 */
GLuint CreateShaderFromFile(const GLuint& shaderType, const std::string& shaderFilePath)
{
	std::ifstream shaderFile(shaderFilePath);
	if (shaderFile.fail())
	{
		std::cerr << "Unable to open shader file: " << shaderFilePath << std::endl;
		return 0;
	}

	std::string shaderSource;
	std::string temp;
	while (std::getline(shaderFile, temp))
	{
		shaderSource += temp + "\n";
	}
	shaderFile.close();

	return CreateShaderFromSource(shaderType, shaderSource);
}

/**
 * @brief Creates a shader based on the provided shader type and the string containing the shader source.
 * @param[in] shaderType Shader type
 * @param[in] shaderSource Shader source string
 * @return OpenGL handle to the created shader
 */
GLuint CreateShaderFromSource(const GLuint& shaderType, const std::string& shaderSource)
{
	GLuint shader = glCreateShader(shaderType);

	const char* shaderSourceCStr = shaderSource.c_str();
	GLint shaderSourceLen = static_cast<GLint>(shaderSource.length());
	glShaderSource(shader, 1, &shaderSourceCStr, &shaderSourceLen);
	glCompileShader(shader);

	// Check compilation status
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		char infoLog[512];
		GLsizei infoLogLen = sizeof(infoLog);
		glGetShaderInfoLog(shader, infoLogLen, &infoLogLen, infoLog);
		std::cerr << "shader compilation error: " << infoLog << std::endl;
	}

	return shader;
}

/**
 * @brief Function for handling the event when the size of the framebuffer changed.
 * @param[in] window Reference to the window
 * @param[in] width New width
 * @param[in] height New height
 */
void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height)
{
	// Whenever the size of the framebuffer changed (due to window resizing, etc.),
	// update the dimensions of the region to the new size
	glViewport(0, 0, width, height);
}
