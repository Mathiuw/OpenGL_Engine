#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShaderSrc(const char* filename);

int main() 
{
	std::cout << "Hello World" << std::endl;

	int success;
	char infolog[512];

	//Initialize GLFW library
	glfwInit();

	//OpenGL Version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//MAC OS Foward Compatibility
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Engine Test", NULL, NULL);

	//Failed to create Window
	if (window == NULL)
	{
		std::cout << "CREATE WINDOW FAILED" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//GLAD Failed to initialize
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress ) )
	{
		std::cout << "FAILED TO INITIALIZE GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//SHADERS
	//Compile Vertex Shader
	unsigned int vextexShader;
	vextexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertShaderSrc = loadShaderSrc("assets/vertex_core.glsl");
	const GLchar* vertShader = vertShaderSrc.c_str();
	glShaderSource(vextexShader, 1, &vertShader, NULL);
	glCompileShader(vextexShader);

	//Catch error
	glGetShaderiv(vextexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vextexShader, 512, NULL, infolog);
		std::cout << "ERROR WITH SHADER COMPILATION: " << std::endl << infolog << std::endl;
	}

	//Compile Fragment Shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragShaderSrc = loadShaderSrc("assets/fragment_core.glsl");
	const GLchar* fragShader = fragShaderSrc.c_str();
	glShaderSource(fragmentShader, 1, &fragShader, NULL);
	glCompileShader(fragmentShader);

	//Catch error
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		std::cout << "ERROR WITH SHADER COMPILATION: " << std::endl << infolog << std::endl;
	}

	//Create Program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//Linking shaders with program
	glAttachShader(shaderProgram, vextexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Catch error
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		std::cout << "LINKING ERROR: " << std::endl << infolog << std::endl;
	}

	//Delete shaders
	glDeleteShader(vextexShader);
	glDeleteShader(fragmentShader);

	//Vertex Array Object(VAO), Vertex Buffer Object(VBO)
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Bind VAO
	glBindVertexArray(VAO);

	//Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	//MAIN RENDER WHILE LOOP
	while (!glfwWindowShouldClose(window) )
	{
		//process input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Generate and Send New Frame to Window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

std::string loadShaderSrc(const char* filename)
{
	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";

	file.open(filename);

	if (file.is_open())
	{
		buf << file.rdbuf();
		ret = buf.str();
	}
	else
	{
		std::cout << "COULD NOT OPEN " << filename << std::endl;
	}

	file.close();

	return ret;
}
