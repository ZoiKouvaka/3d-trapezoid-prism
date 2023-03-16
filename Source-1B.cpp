

//*********************************

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <time.h>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

//************************
// Βοηθητικές συναρτήσεις

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;


float currentZoom = 45.0f; //initialization of currentZoom. Acts like a counter and used to zoom in and zoom out

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

///****************************************************************
//  Εδω θα υλοποιήσετε την συνάρτηση της κάμερας
//****************************************************************

void camera_function()
{
	float zTurn = 0;//the angle of z rotation
	float xTurn = 0;//the angle of x rotation
	 
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) //if <<X>> is pressed rotation at x axis
	{
		xTurn += 0.001; 
		ViewMatrix = glm::rotate(ViewMatrix, xTurn, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //if <<W>> is pressed rotation at x axis
	{
		xTurn-=0.001; 
		ViewMatrix = glm::rotate(ViewMatrix, xTurn, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) //if <<Q>> is pressed rotation at z axis
	{
		zTurn+=0.001; 
		ViewMatrix = glm::rotate(ViewMatrix, zTurn, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) //if <<Z>> is pressed rotation at z axis
	{
		zTurn-=0.001; 
		ViewMatrix = glm::rotate(ViewMatrix, zTurn, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) //if <<+>> is pressed zoom out
	{
		currentZoom -= 0.01; 
	}
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) //if <<->> is pressed zoom in
	{
		currentZoom += 0.01; 
	}
	ProjectionMatrix= glm::perspective(glm::radians(currentZoom), 4.0f / 4.0f, 0.1f, 100.0f); //new ProjectionMatrix

}


//************************************
// Η LoadShaders είναι black box για σας
//************************************
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1000, 1000, u8"Εργασία 1Β – Τραπεζοειδές Πρίσμα", NULL, NULL);// creation of 1000*1000 and right title window


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);// Dark blue background
	glEnable(GL_DEPTH_TEST);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders

	GLuint programID = LoadShaders("ProjBVertexShader.vertexshader", "ProjBFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	//********************
	// **Δείγμα κλήσης των συναρτήσεων για δημιουργία του MVP - είναι τυχαίες οι ρυθμίσεις και δεν ανταποκρίνονται στην άσκησή σας

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(10.0f, 50.0f, 0.0f), //position of the camera
		glm::vec3(0.0f, 0.0f, 0.0f), //looks at
		glm::vec3(0.0f, 0.0f, 1.0f) //upvector
	);
	ViewMatrix = View;
	ProjectionMatrix = Projection;
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
	srand(time(0));
	float h = rand() % 10 + 2;//generates random value between 2 and 10

	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,3.0f, h / 2,
		-4.0f,-3.0f, h / 2,
		1.0f, 3.0f, h / 2, // no1
		1.0f, 3.0f,h / 2,
		-4.0f,-3.0f, h / 2,
		4.0f,-3.0f, h / 2, //no2 //here ends the up polygon base: base a
		 -1.0f,3.0f,-h / 2,
		-4.0f,-3.0f,-h / 2,
		1.0f, 3.0f,-h / 2, // no3
		1.0f, 3.0f,-h / 2,
		-4.0f,-3.0f,-h / 2,
		4.0f,-3.0f,-h / 2, //no4 //here ends the down polygon base: base b
		-1.0f,3.0f,-h / 2,
		-1.0f, 3.0f, h / 2,
		1.0f, 3.0f,-h / 2,//no5 
		 1.0f,3.0f,-h / 2,
		-1.0f,3.0f,h / 2,
		1.0f,3.0f,h / 2,//no6 //here ends up polygon side rectangle base : base c
		-4.0f,-3.0f,-h / 2,
		-4.0f,-3.0f,h / 2,
		4.0f,-3.0f,-h / 2,//no7
		4.0f,-3.0f,-h / 2,
		-4.0f,-3.0f,h / 2,
		4.0f,-3.0f,h / 2,//no8 //here ends down polygon side rectangle base : base d
		4.0f,-3.0f,h / 2,
		4.0f,-3.0f,-h / 2,
		1.0f,3.0f,h / 2,//no9 
		1.0f,3.0f,h / 2,
		4.0f,-3.0f,-h / 2,
		1.0f,3.0f,-h / 2,//no10 //here ends right diagonal polygon side rectangle base : base e 
		-4.0f,-3.0f,h / 2,
		-4.0f,-3.0f,-h / 2,
		-1.0f,3.0f,h / 2,//no11 
		-1.0f,3.0f,h / 2,
		-4.0f,-3.0f,-h / 2,
		-1.0f,3.0f,-h / 2 //no12 //here ends left diagonal polygon side rectangle base :base f
	};
	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,// that was the colour for base a 
		0.195f,  0.548f,  0.859f,
		0.195f,  0.548f,  0.859f,
		0.195f,  0.548f,  0.859f,
		0.195f,  0.548f,  0.859f,
		0.195f,  0.548f,  0.859f,
		0.195f,  0.548f,  0.859f,//that one for base b
		0.014f,  0.184f,  0.576f,
		0.014f,  0.184f,  0.576f,
		0.014f,  0.184f,  0.576f,
		0.014f,  0.184f,  0.576f,
		0.014f,  0.184f,  0.576f,
		0.014f,  0.184f,  0.576f,//that one for base c
		0.997f,  0.513f,  0.064f,
		0.997f,  0.513f,  0.064f,
		0.997f,  0.513f,  0.064f,
		0.997f,  0.513f,  0.064f,
		0.997f,  0.513f,  0.064f,
		0.997f,  0.513f,  0.064f,//that one for base d
		0.055f,  0.953f,  0.042f,
		0.055f,  0.953f,  0.042f,
		0.055f,  0.953f,  0.042f,
		0.055f,  0.953f,  0.042f,
		0.055f,  0.953f,  0.042f,
		0.055f,  0.953f,  0.042f,//that one for base e 
		0.393f,  0.621f,  0.362f,
		0.393f,  0.621f,  0.362f,
		0.393f,  0.621f,  0.362f,
		0.393f,  0.621f,  0.362f,
		0.393f,  0.621f,  0.362f,
		0.393f,  0.621f,  0.362f //that one for base f
	};



	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	float z = 1;//initial z value, no scale
	
	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		//*************************************************
		// Να προστεθεί κώδικας για τον υπολογισμό του νέο MVP
		// 
		// **************************************************/
		
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		{
			z += 0.01; //if <<u>> is pressed then scaling up is happened 
		}
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		{
			z -= 0.01; //if <<p>> is pressed then scaling down is happened
			if (z < 0) //if z is lower than 0, it stops scaling
			{
				z += 0.02;
			}
			
		}

		glm::mat4 NewModel = glm::scale(glm::mat4(1.0f),glm::vec3(1.0f, 1.0f, z));//model matrix: here we need to scale the object on z axes to adjust its height
		camera_function(); //we call camera_function
		MVP = getProjectionMatrix()* getViewMatrix() * NewModel; //create MVP
		/////////////////////////////////////////////

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 3 indices starting at 0 -> 1 triangle


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the space key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

