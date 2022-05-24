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
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

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
glm::vec3 cameraPos = glm::vec3(0.0f, 15.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 10.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 ambient = glm::vec3(0.1f, 0.4f, 0.1f);
glm::vec3 diffuse = glm::vec3(0.9f, 0.9f, 0.9f);
glm::vec3 specular = glm::vec3(0.2f, 0.1f, 0.2f);
glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, 10.0f);
glm::vec3 specComp = glm::vec3(0.9f, 0.0f, 0.0f);


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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Tell GLAD to load the OpenGL function pointers
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return 1;
	}

	// --- Vertex specification ---
	
	// Set up the data for each vertex of the triangle
	Vertex vertices[40];
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
	vertices[20].nx = 0.0f;	vertices[20].ny = -1.0f;	vertices[20].nz = 0.0f;

	vertices[21].x = -3.0f;	vertices[21].y = 1.0f;	vertices[21].z = 0.0f;
	vertices[21].r = 0;		vertices[21].g = 0;		vertices[21].b = 0;
	vertices[21].u = 1.0f;   vertices[21].v = 0.0f;
	vertices[21].nx = 0.0f;	vertices[21].ny = -1.0f;	vertices[21].nz = 0.0f;

	vertices[22].x = -5.0f;	vertices[22].y = 1.0f;	vertices[22].z = 2.0f;
	vertices[22].r = 0;		vertices[22].g = 0;		vertices[22].b = 255;
	vertices[22].u = 0.0f;   vertices[22].v = 1.0f;
	vertices[22].nx = 0.0f;	vertices[22].ny = -1.0f;	vertices[22].nz = 0.0f;

	vertices[23].x = -5.0f;	vertices[23].y = 1.0f;	vertices[23].z = 0.0f;
	vertices[23].r = 0;		vertices[23].g = 0;		vertices[23].b = 255;
	vertices[23].u = 1.0f;   vertices[23].v = 1.0f;
	vertices[23].nx = 0.0f;	vertices[23].ny = -1.0f;	vertices[23].nz = 0.0f;

	//Back Panel
	vertices[24].x = 10.0f;	vertices[24].y = 1.0f;	vertices[24].z = -5.0f;
	vertices[24].r = 0;		vertices[24].g = 0;		vertices[24].b = 0;
	vertices[24].u = 0.0f;   vertices[24].v = 0.0f;
	vertices[24].nx = 0.0f;	vertices[24].ny = 0.0f;	vertices[24].nz = 1.0f;

	vertices[25].x = 10.0f;	vertices[25].y = 15.0f;	vertices[25].z = -5.0f;
	vertices[25].r = 0;		vertices[25].g = 0;		vertices[25].b = 0;
	vertices[25].u = 1.0f;   vertices[25].v = 0.0f;
	vertices[25].nx = 0.0f;	vertices[25].ny = 0.0f;	vertices[25].nz = 1.0f;

	vertices[26].x = -11.0f;	vertices[26].y = 1.0f;	vertices[26].z = -5.0f;
	vertices[26].r = 0;		vertices[26].g = 0;		vertices[26].b = 255;
	vertices[26].u = 0.0f;   vertices[26].v = 1.0f;
	vertices[26].nx = 0.0f;	vertices[26].ny = 0.0f;	vertices[26].nz = 1.0f;

	vertices[27].x = -11.0f;	vertices[27].y = 15.0f;	vertices[27].z = -5.0f;
	vertices[27].r = 0;		vertices[27].g = 0;		vertices[27].b = 255;
	vertices[27].u = 1.0f;   vertices[27].v = 1.0f;
	vertices[27].nx = 0.0f;	vertices[27].ny = 0.0f;	vertices[27].nz = 1.0f;

	//Left Side Panel
	vertices[28].x = -11.0f;	vertices[28].y = 15.0f;	vertices[28].z = -5.0f;
	vertices[28].r = 0;		vertices[28].g = 0;		vertices[28].b = 255;
	vertices[28].u = 0.0f;   vertices[28].v = 0.0f;
	vertices[28].nx = 1.0f;	vertices[28].ny = 0.0f;	vertices[28].nz = 0.0f;

	vertices[29].x = -11.0f;	vertices[29].y = 1.0f;	vertices[29].z = -5.0f;
	vertices[29].r = 0;		vertices[29].g = 0;		vertices[29].b = 255;
	vertices[29].u = 1.0f;   vertices[29].v = 0.0f;
	vertices[29].nx = 1.0f;	vertices[29].ny = 0.0f;	vertices[29].nz = 0.0f;

	vertices[30].x = -11.0f;	vertices[30].y = 15.0f;	vertices[30].z = 7.0f;
	vertices[30].r = 0;		vertices[30].g = 0;		vertices[30].b = 255;
	vertices[30].u = 0.0f;   vertices[30].v = 1.0f;
	vertices[30].nx = 1.0f;	vertices[30].ny = 0.0f;	vertices[30].nz = 0.0f;

	vertices[31].x = -11.0f;	vertices[31].y = 1.0f;	vertices[31].z = 7.0f;
	vertices[31].r = 0;		vertices[31].g = 0;		vertices[31].b = 255;
	vertices[31].u = 1.0f;   vertices[31].v = 1.0f;
	vertices[31].nx = 1.0f;	vertices[31].ny = 0.0f;	vertices[31].nz = 0.0f;

	//Right Side Panel

	vertices[32].x = 10.0f;	vertices[32].y = 15.0f;	vertices[32].z = -5.0f;
	vertices[32].r = 0;		vertices[32].g = 0;		vertices[32].b = 255;
	vertices[32].u = 0.0f;   vertices[32].v = 0.0f;
	vertices[32].nx = -1.0f;	vertices[32].ny = 0.0f;	vertices[32].nz = 0.0f;

	vertices[33].x = 10.0f;	vertices[33].y = 1.0f;	vertices[33].z = -5.0f;
	vertices[33].r = 0;		vertices[33].g = 0;		vertices[33].b = 255;
	vertices[33].u = 1.0f;   vertices[33].v = 0.0f;
	vertices[33].nx = -1.0f;	vertices[33].ny = 0.0f;	vertices[33].nz = 0.0f;

	vertices[34].x = 10.0f;	vertices[34].y = 15.0f;	vertices[34].z = 7.0f;
	vertices[34].r = 0;		vertices[34].g = 0;		vertices[34].b = 255;
	vertices[34].u = 0.0f;   vertices[34].v = 1.0f;
	vertices[34].nx = -1.0f;	vertices[34].ny = 0.0f;	vertices[34].nz = 0.0f;

	vertices[35].x = 10.0f;	vertices[35].y = 1.0f;	vertices[35].z = 7.0f;
	vertices[35].r = 0;		vertices[35].g = 0;		vertices[35].b = 255;
	vertices[35].u = 1.0f;   vertices[35].v = 1.0f;
	vertices[35].nx = -1.0f;	vertices[35].ny = 0.0f;	vertices[35].nz = 0.0f;

	//Floor Panel

	vertices[36].x = 10.0f;	vertices[36].y = 1.0f;	vertices[36].z = -5.0f;
	vertices[36].r = 0;		vertices[36].g = 0;		vertices[36].b = 255;
	vertices[36].u = 0.0f;   vertices[36].v = 0.0f;
	vertices[36].nx = 0.0f;	vertices[36].ny = 1.0f;	vertices[36].nz = 0.0f;

	vertices[37].x = 10.0f;	vertices[37].y = 1.0f;	vertices[37].z = 7.0f;
	vertices[37].r = 0;		vertices[37].g = 0;		vertices[37].b = 255;
	vertices[37].u = 1.0f;   vertices[37].v = 0.0f;
	vertices[37].nx = 0.0f;	vertices[37].ny = 1.0f;	vertices[37].nz = 0.0f;

	vertices[38].x = -11.0f;	vertices[38].y = 1.0f;	vertices[38].z = -5.0f;
	vertices[38].r = 0;		vertices[38].g = 0;		vertices[38].b = 255;
	vertices[38].u = 0.0f;   vertices[38].v = 1.0f;
	vertices[38].nx = 0.0f;	vertices[38].ny = 1.0f;	vertices[38].nz = 0.0f;

	vertices[39].x = -11.0f;	vertices[39].y = 1.0f;	vertices[39].z = 7.0f;
	vertices[39].r = 0;		vertices[39].g = 0;		vertices[39].b = 255;
	vertices[39].u = 1.0f;   vertices[39].v = 1.0f;
	vertices[39].nx = 0.0f;	vertices[39].ny = 1.0f;	vertices[39].nz = 0.0f;

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
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, nx)));

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
	unsigned char* imageData = stbi_load("toriigate_redwood.jpg", &imageWidth, &imageHeight, &numChannels, 0);

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
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;

		processInput(window);

		// Clear the colors in our off-screen framebuffer
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		//Transformation "Globals"
		glm::mat4 PerspectiveProj = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 camera = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		GLint uniformLocation = glGetUniformLocation(program, "mvp");
		GLint texUniformLocation = glGetUniformLocation(program, "tex");
		GLint ambientUniformLocation = glGetUniformLocation(program, "ambient");
		GLint diffuseUniformLocation = glGetUniformLocation(program, "diffuse");
		GLint specularUniformLocation = glGetUniformLocation(program, "specular");
		GLint lightUniformLocation = glGetUniformLocation(program, "lightPos");
		GLint modelUniformLocation = glGetUniformLocation(program, "model");
		GLint normalUniformLocation = glGetUniformLocation(program, "norm");
		GLint viewUniformLocation = glGetUniformLocation(program, "cameraPos");
		GLint specCompUniformLocation = glGetUniformLocation(program, "specComp");
		glUniform3f(lightUniformLocation, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(ambientUniformLocation, ambient.x, ambient.y, ambient.z);
		glUniform3f(diffuseUniformLocation, diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(specularUniformLocation, specular.x, specular.y, specular.z);
		glUniform3f(viewUniformLocation, cameraPos.x, cameraPos.y, cameraPos.z);
		glUniform3f(specCompUniformLocation, specComp.x, specComp.y, specComp.z);
		
		

		//Left Torii Base
		//Transformations
		glm::mat4 LTB = glm::mat4(1.0f);
		glm::mat3 normalLTB = glm::transpose(glm::inverse(glm::mat3(LTB)));
		glm::mat4 modelLTB = LTB;
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(modelLTB));
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
		glm::mat3 normalLTP = glm::transpose(glm::inverse(glm::mat3(LTP)));
		glm::mat4 modelLTP = LTP;
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(modelLTP));
		glUniformMatrix3fv(normalUniformLocation, 1 , GL_TRUE, glm::value_ptr(normalLTP));
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
		RTB = glm::translate(RTB, glm::vec3(6.0f, 0.0f, 0.0f));
		glm::mat3 normalRTB = glm::transpose(glm::inverse(glm::mat3(RTB)));
		glm::mat4 modelRTB = RTB;
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(modelRTB));
		glUniformMatrix3fv(normalUniformLocation, 1 , GL_TRUE, glm::value_ptr(normalRTB));
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
		RTP = glm::translate(RTP, glm::vec3(-1.0f, -3.5f, 0.125f));
		RTP = glm::scale(RTP, glm::vec3(0.75f, 6.0f, 0.75f));
		RTP = glm::translate(RTP, glm::vec3(8.0f, 0.0f, 0.0f));
		glm::mat3 normalRTP = glm::transpose(glm::inverse(glm::mat3(RTP)));
		glm::mat4 modelRTP = RTP;
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(modelRTP));
		glUniformMatrix3fv(normalUniformLocation, 1 , GL_TRUE, glm::value_ptr(normalRTP));
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

		//Middle Horizontal Pillar
		glm::mat4 MHP = glm::mat4(1.0f);
		MHP = glm::translate(MHP, glm::vec3(11.0f, 12.5f, 0.135f));
		MHP = glm::rotate(MHP, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		MHP = glm::scale(MHP, glm::vec3(0.3f, 6.0f, 0.7f));
		glm::mat3 normalMHP = glm::transpose(glm::inverse(glm::mat3(MHP)));
		glm::mat4 modelMHP = MHP;
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(modelMHP));
		glUniformMatrix3fv(normalUniformLocation, 1 , GL_TRUE, glm::value_ptr(normalMHP));
		MHP = PerspectiveProj * camera * MHP;
    	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(MHP));

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

		glBindVertexArray(0);

		//Middle Top Pillar
		glm::mat4 MTP = glm::mat4(1.0f);
		MTP = glm::translate(MTP, glm::vec3(11.0f, 15.999f, 0.135f));
		MTP = glm::rotate(MTP, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		MTP = glm::scale(MTP, glm::vec3(0.3f, 6.0f, 0.7f));
		glm::mat3 normalMTP = glm::transpose(glm::inverse(glm::mat3(MTP)));
		glm::mat4 modelMTP = MTP;
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(modelMTP));
		glUniformMatrix3fv(normalUniformLocation, 1 , GL_TRUE, glm::value_ptr(normalMTP));
		MTP = PerspectiveProj * camera * MTP;
    	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(MTP));

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

		glBindVertexArray(0);

		//Left Roof Wing
		glm::mat4 LRW = glm::mat4(1.0f);
		LRW = glm::translate(LRW, glm::vec3(-5.65f, 15.699f, 0.136f));
		LRW = glm::rotate(LRW, glm::radians(75.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		LRW = glm::scale(LRW, glm::vec3(0.3f, 1.0f, 0.69f));
		glm::mat3 normalLRW = glm::transpose(glm::inverse(glm::mat3(LRW)));
		glm::mat4 modelLRW = LRW;
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(modelLRW));
		glUniformMatrix3fv(normalUniformLocation, 1 , GL_TRUE, glm::value_ptr(normalLRW));
		LRW = PerspectiveProj * camera * LRW;
    	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(LRW));

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

		glBindVertexArray(0);

		//Right Roof Wing
		glm::mat4 RRW = glm::mat4(1.0f);
		RRW = glm::translate(RRW, glm::vec3(7.52f, 16.735f, 0.135f));
		RRW = glm::rotate(RRW, glm::radians(105.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		RRW = glm::scale(RRW, glm::vec3(0.3f, 1.0f, 0.69f));

		RRW = PerspectiveProj * camera * RRW;
    	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(RRW));
		glm::mat3 normalRRW = glm::transpose(glm::inverse(glm::mat3(RRW)));
		glm::mat4 modelRRW = RRW;
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(modelRRW));
		glUniformMatrix3fv(normalUniformLocation, 1 , GL_TRUE, glm::value_ptr(normalRRW));
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

		glBindVertexArray(0);

		//Back Panel
		glm::mat4 BackPanel = glm::mat4(1.0f);
		glm::mat3 normalBackPanel = glm::transpose(glm::inverse(glm::mat3(BackPanel)));
		glm::mat4 modelBackPanel = BackPanel;
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(modelBackPanel));
		glUniformMatrix3fv(normalUniformLocation, 1 , GL_TRUE, glm::value_ptr(normalBackPanel));
		BackPanel = PerspectiveProj * camera * BackPanel;
    	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(BackPanel));

		// Use the shader program that we created
		glUseProgram(program);

		// Use the vertex array object that we created
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glUniform1i(texUniformLocation, 0);

		// Draw the vertices
		glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);
		

		// "Unuse" the vertex array object
		glBindVertexArray(0);

		//Left Panel
		glm::mat4 LeftPanel = glm::mat4(1.0f);
		glm::mat3 normalLeftPanel = glm::transpose(glm::inverse(glm::mat3(LeftPanel)));
		glm::mat4 modelLeftPanel = LeftPanel;
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(modelLeftPanel));
		glUniformMatrix3fv(normalUniformLocation, 1 , GL_TRUE, glm::value_ptr(normalLeftPanel));
		LeftPanel = PerspectiveProj * camera * LeftPanel;
    	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(LeftPanel));

		// Use the shader program that we created
		glUseProgram(program);

		// Use the vertex array object that we created
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glUniform1i(texUniformLocation, 0);

		// Draw the vertices
		glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
		

		// "Unuse" the vertex array object
		glBindVertexArray(0);

		//Right Panel
		glm::mat4 RightPanel = glm::mat4(1.0f);
		glm::mat3 normalRightPanel = glm::transpose(glm::inverse(glm::mat3(RightPanel)));
		glm::mat4 modelRightPanel = RightPanel;
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(modelRightPanel));
		glUniformMatrix3fv(normalUniformLocation, 1 , GL_TRUE, glm::value_ptr(normalRightPanel));
		RightPanel = PerspectiveProj * camera * RightPanel;
    	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(RightPanel));

		// Use the shader program that we created
		glUseProgram(program);

		// Use the vertex array object that we created
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glUniform1i(texUniformLocation, 0);

		// Draw the vertices
		glDrawArrays(GL_TRIANGLE_STRIP, 32, 4);
		

		// "Unuse" the vertex array object
		glBindVertexArray(0);

		//Floor Panel
		glm::mat4 FloorPanel = glm::mat4(1.0f);
		glm::mat3 normalFloorPanel = glm::transpose(glm::inverse(glm::mat3(FloorPanel)));
		glm::mat4 modelFloorPanel = FloorPanel;
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(modelFloorPanel));
		glUniformMatrix3fv(normalUniformLocation, 1 , GL_TRUE, glm::value_ptr(normalFloorPanel));
		FloorPanel = PerspectiveProj * camera * FloorPanel;
    	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(FloorPanel));

		// Use the shader program that we created
		glUseProgram(program);

		// Use the vertex array object that we created
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glUniform1i(texUniformLocation, 0);

		// Draw the vertices
		glDrawArrays(GL_TRIANGLE_STRIP, 36, 4);
		

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

void processInput(GLFWwindow *window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}

	float cameraSpeed = static_cast<float>(0.005 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;;
	}
}
void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height)
{
	// Whenever the size of the framebuffer changed (due to window resizing, etc.),
	// update the dimensions of the region to the new size
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f){
		pitch = 89.0f;
	}
	if (pitch < -89.0f){
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	fov -= (float)yoffset;
	if (fov < 1.0f){
		fov = 1.0f;
	}
	if (fov > 45.0f){
		fov = 45.0f;
	}
}