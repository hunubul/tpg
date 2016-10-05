// GLEW
#include <GL/glew.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertices.h"
#include <string>

// Set up our vertex data (and buffer(s)) and attribute pointers
GLfloat screenVertices[] = {
	// First triangle
	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f,  1.0f,  1.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	// Second triangle
	1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

GLfloat CubeVertices::Front[] = {
	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	-1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f
};
/*-1.0f, -1.0f,  0.0f,  0.0f, 0.0f,
1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
-1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
-1.0f, -1.0f,  0.0f,  0.0f, 0.0f,*/
GLfloat CubeVertices::Left[] = {
	-1.0f,  1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	-1.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f, -1.0f,  1.0f, 0.0f
};
GLfloat CubeVertices::Right[] = {
	1.0f,  1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	1.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
	1.0f,  1.0f, -1.0f,  1.0f, 0.0f
};
GLfloat CubeVertices::Floor[] = {
	-1.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
	1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f, -1.0f,  0.0f,  0.0f, 1.0f
};
GLfloat CubeVertices::Ceiling[] = {
	-1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	1.0f,  1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
GLfloat CubeVertices::nullvalue = 0;

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, -8.0)
	/*glm::vec3(1.5f, 0.0f, -3.0f),
	glm::vec3(-1.5f, 0.0f, -3.0f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)*/
};

// UI triangles
GLfloat UIElements::_GL<UIElements::Compass>::vertices[] = {
	// First triangle
	 0.0f,  1.0f,  0.0f, 1.0f,
	 0.5f,  1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  1.0f, 0.0f,
	// Second triangle
	 0.5f,  0.5f,  1.0f, 0.0f,
	 0.0f,  0.5f,  0.0f, 0.0f,
	 0.0f,  1.0f,  0.0f, 1.0f
};
std::string UIElements::_GL<UIElements::Compass>::locationPath = "./images/UI/200px-Compass.png";
