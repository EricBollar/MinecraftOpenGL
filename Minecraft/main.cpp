#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <cstdio>
#include <chrono>
#include <math.h>

std::chrono::steady_clock::time_point windowStart;

GLFWwindow* initWindow(const int resX, const int resY, const char* title) {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4xAA

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(resX, resY, title, NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	//glfwSetKeyCallback(window, controls);

	// Get info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST); // Depth Testing
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	windowStart = std::chrono::steady_clock::now();
	return window;
}

double getCubeRotation(double speed) {
	std::chrono::steady_clock::duration d = std::chrono::steady_clock::now() - windowStart;
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(d);
	std::cout << ms.count() << std::endl;
	return ms.count() / speed;
}

void drawCube() {
	GLfloat vertices[] =
	{
		-1, -1, -1,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,
		 1, -1, -1,    1, -1,  1,    1,  1,  1,    1,  1, -1,
		-1, -1, -1,   -1, -1,  1,    1, -1,  1,    1, -1, -1,
		-1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
		-1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
		-1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
	};

	GLfloat colors[] =
	{
		0, 0, 0,   0, 0, 1,   0, 1, 1,   0, 1, 0,
		1, 0, 0,   1, 0, 1,   1, 1, 1,   1, 1, 0,
		0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0,
		0, 1, 0,   0, 1, 1,   1, 1, 1,   1, 1, 0,
		0, 0, 0,   0, 1, 0,   1, 1, 0,   1, 0, 0,
		0, 0, 1,   0, 1, 1,   1, 1, 1,   1, 0, 1
	};

	glScalef(0.5, 0.5, 0.5);
	glRotatef(getCubeRotation(10.0), 0, 1, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glDrawArrays(GL_QUADS, 0, 24);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void handleWorld() {
	drawCube();
}

void display(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window)) {
		// Scale to window size
		GLint windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);

		// Draw
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION_MATRIX);
		glLoadIdentity();
		gluPerspective(60, (double)windowWidth / (double)windowHeight, 0.1, 100);
		glMatrixMode(GL_MODELVIEW_MATRIX);
		glTranslatef(0, -0.25, -4);
		glRotatef(25, 1, 0, 0);

		// Update Screen
		handleWorld();
		glfwSwapBuffers(window);

		// Check for inputs or window movement
		glfwPollEvents();
	}
}

int main() {
	GLFWwindow* window = initWindow(1280, 720, "Hello World!");
	if (window != NULL) {
		display(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}