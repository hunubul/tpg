#pragma once

// GLEW
#include <GL/glew.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
// Cube (with 5 sides)
class CubeVertices {
	public:
		static const int ArrSize = 30;
		static const int CubeSides = 5;
		static GLfloat Front[];
		static GLfloat Left[];
		static GLfloat Right[];
		static GLfloat Floor[];
		static GLfloat Ceiling[];

		static GLfloat &index(unsigned int i) {
			// 6 points for 1 side and 5 parameters for 1 point
			if (6 * 5 * 0 <= i && i < 6 * 5 * 1) return Front[i%ArrSize];
			if (6 * 5 * 1 <= i && i < 6 * 5 * 2) return Left[i%ArrSize];
			if (6 * 5 * 2 <= i && i < 6 * 5 * 3) return Right[i%ArrSize];
			if (6 * 5 * 3 <= i && i < 6 * 5 * 4) return Floor[i%ArrSize];
			if (6 * 5 * 4 <= i && i < 6 * 5 * 5) return Ceiling[i%ArrSize];
			return Front[2]; // 0 value
		}
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