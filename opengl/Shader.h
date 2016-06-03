#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

//Shaders location
const char* texturesVertPath = "./shaders/textures.vert";
const char* texturesFragPath = "./shaders/textures.frag";
const char* framebufferVertPath = "./shaders/framebuffer.vert";
const char* framebufferFragPath = "./shaders/framebuffer.frag";

class Shader
{
public:
	GLuint Program, FrameBufferProgram;
	// Constructor generates the shader on the fly
	Shader()
	{
		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string vFramebuffCode;
		std::string fFramebuffCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream vFramebuffShaderFile;
		std::ifstream fFramebuffShaderFile;
		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		vFramebuffShaderFile.exceptions(std::ifstream::badbit);
		fFramebuffShaderFile.exceptions(std::ifstream::badbit);
		try {
			// Open files
			vShaderFile.open(texturesVertPath);
			fShaderFile.open(texturesFragPath);
			vFramebuffShaderFile.open(framebufferVertPath);
			fFramebuffShaderFile.open(framebufferFragPath);
			std::stringstream vShaderStream, fShaderStream, vFramebuffShaderStream, fFramebuffShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			vFramebuffShaderStream << vFramebuffShaderFile.rdbuf();
			fFramebuffShaderStream << fFramebuffShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			vFramebuffShaderFile.close();
			fFramebuffShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			vFramebuffCode = vFramebuffShaderStream.str();
			fFramebuffCode = fFramebuffShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();
		const GLchar* vFramebuffShaderCode = vFramebuffCode.c_str();
		const GLchar* fFramebuffShaderCode = fFramebuffCode.c_str();
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


		GLuint vertexShader, fragmentShader;
		// Create and compile the vertex shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vFramebuffShaderCode, NULL);
		glCompileShader(vertexShader);

		// Create and compile the fragment shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fFramebuffShaderCode, NULL);
		glCompileShader(fragmentShader);

		// Link the vertex and fragment shader into a shader program
		FrameBufferProgram = glCreateProgram();
		glAttachShader(FrameBufferProgram, vertexShader);
		glAttachShader(FrameBufferProgram, fragmentShader);
		glBindFragDataLocation(FrameBufferProgram, 0, "outColor");
		glLinkProgram(FrameBufferProgram);

		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	// Uses the current shader
	void Use()
	{
		glUseProgram(this->Program);
	}
	void UseFrameBufferProgram()
	{
		glUseProgram(this->FrameBufferProgram);
	}
};

#endif

