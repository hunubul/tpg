#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>

class Shader
{
public:
	GLuint Program;
	// Constructor generates the shader on the fly
	Shader(std::string vertexPath, std::string fragmentPath, std::vector<int> fragmentParams)
	{
		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try {
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode;
		std::string completedFragmentCode;
		// Replace $ with given vector params
		if (fragmentParams.size() > 0) {
			std::stringstream tokenizedFragmentCode(fragmentCode);
			std::string temp;
			for (size_t i = 0; i < fragmentParams.size(); i++) {
				std::getline(tokenizedFragmentCode, temp, '$');
				completedFragmentCode.append(temp);
				completedFragmentCode.append(std::to_string(fragmentParams[i]));
			}
			std::getline(tokenizedFragmentCode, temp, '$');
			completedFragmentCode.append(temp);
			fShaderCode = completedFragmentCode.c_str();
		} else {
			fShaderCode = fragmentCode.c_str();
		}
		// 2. Compile shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];
		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// Print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// Print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Shader Program
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glBindFragDataLocation(this->Program, 0, "outColor");
		glLinkProgram(this->Program);
		// Print linking errors if any
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	// Uses the current shader
	void UseProgram()
	{
		glUseProgram(this->Program);
	}
};

class ComputeShader {
public:
	GLuint ComputerShaderProgram;
	ComputeShader(std::string computeShaderPath, std::vector<int> computeParams) {
		// Retrieve the vertex/fragment source code from filePath
		std::string computeCode;
		std::ifstream computeFile;
		// ensures ifstream objects can throw exceptions:
		computeFile.exceptions(std::ifstream::badbit);
		try {
			// Open files
			computeFile.open(computeShaderPath);
			std::stringstream computeShaderStream;
			// Read file's buffer contents into streams
			computeShaderStream << computeFile.rdbuf();
			// close file handlers
			computeFile.close();
			// Convert stream into string
			computeCode = computeShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar* computeShaderCode;
		std::string completedComputeCode;
		if (computeParams.size() > 0) {
			std::stringstream tokenizedFragmentCode(computeCode);
			std::string temp;
			for (size_t i = 0; i < computeParams.size(); i++) {
				std::getline(tokenizedFragmentCode, temp, '$');
				completedComputeCode.append(temp);
				completedComputeCode.append(std::to_string(computeParams[i]));
			}
			std::getline(tokenizedFragmentCode, temp, '$');
			completedComputeCode.append(temp);
			computeShaderCode = completedComputeCode.c_str();
		} else {
			computeShaderCode = computeCode.c_str();
		}

		//---Creating the compute shader, and the program object containing the shader---
		ComputerShaderProgram = glCreateProgram();
		GLuint cs = glCreateShader(GL_COMPUTE_SHADER);

		// In order to write to a texture, we have to introduce it as image2D.
		// local_size_x/y/z layout variables define the work group size.
		// gl_GlobalInvocationID is a uvec3 variable giving the global ID of the thread,
		// gl_LocalInvocationID is the local index within the work group, and
		// gl_WorkGroupID is the work group's index

		glShaderSource(cs, 1, &computeShaderCode, NULL);
		glCompileShader(cs);
		int rvalue;
		glGetShaderiv(cs, GL_COMPILE_STATUS, &rvalue);
		if (!rvalue) {
			fprintf(stderr, "Error in compiling the compute shader\n");
			GLchar log[10240];
			GLsizei length;
			glGetShaderInfoLog(cs, 10239, &length, log);
			fprintf(stderr, "Compiler log:\n%s\n", log);
			//exit(40);
		}
		glAttachShader(ComputerShaderProgram, cs);

		glLinkProgram(ComputerShaderProgram);
		glGetProgramiv(ComputerShaderProgram, GL_LINK_STATUS, &rvalue);
		if (!rvalue) {
			fprintf(stderr, "Error in linking compute shader program\n");
			GLchar log[10240];
			GLsizei length;
			glGetProgramInfoLog(ComputerShaderProgram, 10239, &length, log);
			fprintf(stderr, "Linker log:\n%s\n", log);
			//exit(41);
		}
		glDeleteShader(cs);
	}

	void UseComputerShaderProgram()
	{
		glUseProgram(this->ComputerShaderProgram);
	}
};

#endif

