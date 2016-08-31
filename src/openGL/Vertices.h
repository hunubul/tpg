#pragma once

// GLEW
#include <GL/glew.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern GLfloat screenVertices[6*4];
extern glm::vec3 cubePositions[1];

// Cube (with 5 sides)
class CubeVertices {
public:
	static const int ArrSize = 30;
	static const int CubeSides = 5;
	static GLfloat Front[ArrSize];
	static GLfloat Left[ArrSize];
	static GLfloat Right[ArrSize];
	static GLfloat Floor[ArrSize];
	static GLfloat Ceiling[ArrSize];

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