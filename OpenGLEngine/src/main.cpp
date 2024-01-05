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
	unsigned int fragmentShaders[2];

	//Fragment Shader 1
	fragmentShaders[0] = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragShaderSrc = loadShaderSrc("assets/fragment_core.glsl");
	const GLchar* fragShader = fragShaderSrc.c_str();
	glShaderSource(fragmentShaders[0], 1, &fragShader, NULL);
	glCompileShader(fragmentShaders[0]);

	//Catch error
	glGetShaderiv(fragmentShaders[0], GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShaders[0], 512, NULL, infolog);
		std::cout << "ERROR WITH SHADER COMPILATION: " << std::endl << infolog << std::endl;
	}

	//Fragment Shader 2
	fragmentShaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
	fragShaderSrc = loadShaderSrc("assets/fragment_core2.glsl");
	fragShader = fragShaderSrc.c_str();
	glShaderSource(fragmentShaders[1], 1, &fragShader, NULL);
	glCompileShader(fragmentShaders[1]);

	//Catch error
	glGetShaderiv(fragmentShaders[1], GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShaders[1], 512, NULL, infolog);
		std::cout << "ERROR WITH SHADER COMPILATION: " << std::endl << infolog << std::endl;
	}

	//Create Program
	unsigned int shaderPrograms[2];
	shaderPrograms[0] = glCreateProgram();

	//Linking shaders with program
	glAttachShader(shaderPrograms[0], vextexShader);
	glAttachShader(shaderPrograms[0], fragmentShaders[0]);
	glLinkProgram(shaderPrograms[0]);

	//Catch error
	glGetProgramiv(shaderPrograms[0], GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderPrograms[0], 512, NULL, infolog);
		std::cout << "LINKING ERROR: " << std::endl << infolog << std::endl;
	}

	shaderPrograms[1] = glCreateProgram();

	//Linking shaders with program
	glAttachShader(shaderPrograms[1], vextexShader);
	glAttachShader(shaderPrograms[1], fragmentShaders[1]);
	glLinkProgram(shaderPrograms[1]);

	//Catch error
	glGetProgramiv(shaderPrograms[1], GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderPrograms[1], 512, NULL, infolog);
		std::cout << "LINKING ERROR: " << std::endl << infolog << std::endl;
	}

	//Delete shaders
	glDeleteShader(vextexShader);
	glDeleteShader(fragmentShaders[0]);
	glDeleteShader(fragmentShaders[1]);

	//Vertex Array
	// 
	//// RECTANGLE
	//float vertices[] =
	//{
	//	0.5f, 0.5f,0.0f, //Top Right
	//	0.5f, -0.5f, 0.0f, //Bottom Right
	//	-0.5f, 0.5f, 0.0f, //Top Left
	//	-0.5f, -0.5f, 0.0f, //Bottom Left
	//};

	//unsigned int indices[] =
	//{
	//	2, 3, 0, //First Triangle
	//	3, 1, 0 //Second Triangle
	//};

	float vertices[] =
	{
		//First triangle
		-0.5f, -0.5f, 0.0f,
		-0.25f, 0.5f, 0.0f,
		-0.1f, -0.5f, 0.0f,

		//Seconde Triangle

		0.5, -0.5f, 0.0f,
		0.25f, 0.5f, 0.0f,
		0.1f, -0.5f, 0.0f
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		3, 4, 5
	};

	//Vertex Array Object(VAO), Vertex Buffer Object(VBO)
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bind VAO
	glBindVertexArray(VAO);

	//Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Set attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//MAIN RENDER WHILE LOOP
	while (!glfwWindowShouldClose(window) )
	{
		//process input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw shape
		glBindVertexArray(VAO);

		//First Triangle
		glUseProgram(shaderPrograms[0]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		//Second Triangle
		glUseProgram(shaderPrograms[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));


		//Generate and Send New Frame to Window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &EBO);

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
