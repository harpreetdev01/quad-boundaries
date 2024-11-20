#include <fmt/core.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

// Window global settings
GLFWwindow* window;
auto closeWindow = false;
const int WIN_WIDTH = 1920;
const int WIN_HEIGHT = 1080;

// Global variables
unsigned int program;
unsigned int VBO;
// Set up vertex data and buffers
float vertices[] =
{
	// Quad 1
	// Triangle 1
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left - Red
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Right - Red
	0.5f, 0.5f, 0.0f, 1.0f,  0.0f, 0.0f, // Top Right - Red

	// Triangle 2
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left - Red
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// Top Right - Red
	-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Top Left - Red


	// Quad 2
	// Triangle 1
	-0.05f, -0.05f, 0.0f, 1.0f, 0.0f, 1.0f,// Bottom Left - Purple
	0.05f, -0.05f, 0.0f, 1.0f, 0.0f, 1.0f,// Bottom Right - Purple
	0.05f, 0.05f, 0.0f, 1.0f, 0.0f, 1.0f, // Top Right - Purple

	//Triangle 2
	-0.05f, -0.05f, 0.0f, 1.0f, 0.0f, 1.0f,// Bottom Left - Purple
	0.05f, 0.05f, 0.0f, 1.0f, 0.0f, 1.0f, // Top Right - Purple
	-0.05f, 0.05f, 0.0f, 1.0f, 0.0f, 1.0f,// Top Left - Purple
};

// Shaders
// Vertex
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // Vertex position
"layout (location = 1) in vec3 aColor;\n" // Vertex color
"out vec3 vertexColor;\n" // Pass color to the fragment shader
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0);\n"
"	vertexColor = aColor;\n"
"}\0";

// Fragment
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 vertexColor; \n"
"out vec4 FragColor; \n"
"void main()\n"
"{\n"
"	FragColor = vec4(vertexColor, 1.0f);\n"
"}\0";

// Error callback
static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
};

// Build and compile Shader program
static void shaderProgram()
{
	// Vertex shader - create, link, and compile
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check for Vertex Shader errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SUCCESS::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}


	// Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check for Fragment Shader errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SUCCESS::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// Link shaders
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// Check for linking shaders
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// Delete shaders - once linked to the program, they are not required because opengl doesnt need them anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	

	// Generate ID for buffer object
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
};

void createWindow()
{
	if (GLFW_TRUE != glfwInit())
	{
		cout << "Failed to init glfw" << '\n';
	}
	else 
	{
		cout << "GLFW init success" << '\n';
	}

	glfwSetErrorCallback(error_callback);

	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Step 1", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwShowWindow(window);

	auto gint = glewInit();
	if (gint != GLEW_OK)
	{
		cout << "Failed init glew" << '\n';
		return;
	}
	else
	{
		auto glVersionBytes = (char*)glGetString(GL_VERSION);
		auto glVendorBytes = (char*)glGetString(GL_VERSION);
		string glVersion((char*)glVersionBytes);
		string glVendor((char*)glVendorBytes);

		cout << "glew init success" << '\n';
		cout << glVersion << '\n';
		cout << glVendor << '\n';
		cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
	}
}

void setupOpenGL()
{
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
}


int main()
{
	createWindow();
	setupOpenGL();
	shaderProgram();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // 24 bytes stride and read at every 12 bytes for color
		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 3, 3);
		glDrawArrays(GL_TRIANGLES, 6, 3);
		glDrawArrays(GL_TRIANGLES, 9, 3);

		// Update triangle position
		float time = glfwGetTime();
		float offsetX = 0.5f * sin(time); // Oscillate along X-axis
		vertices[0] = -0.5f + offsetX; // Update bottom-left vertex
		vertices[1] = 0.5f + offsetX; // Update bottom-left vertex
		vertices[3] = 0.5f + offsetX;  // Update bottom-right vertex
		vertices[6] = 0.0f + offsetX;  // Update top-center vertex

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}