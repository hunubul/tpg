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
	GLuint Program, FrameBufferProgram, ComputerShaderProgram;
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


		//------------------------------------------
		GLuint vertexShader, fragmentShader;
		// Create and compile the vertex shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vFramebuffShaderCode, NULL);
		glCompileShader(vertexShader);
		// Print compile errors if any
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::BUFFERVERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Create and compile the fragment shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fFramebuffShaderCode, NULL);
		glCompileShader(fragmentShader);
		// Print compile errors if any
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::BUFFERFRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Link the vertex and fragment shader into a shader program
		FrameBufferProgram = glCreateProgram();
		glAttachShader(FrameBufferProgram, vertexShader);
		glAttachShader(FrameBufferProgram, fragmentShader);
		glBindFragDataLocation(FrameBufferProgram, 0, "outColor");
		glLinkProgram(FrameBufferProgram);

		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);


		//---Creating the compute shader, and the program object containing the shader---
		ComputerShaderProgram = glCreateProgram();
		GLuint cs = glCreateShader(GL_COMPUTE_SHADER);

		// In order to write to a texture, we have to introduce it as image2D.
		// local_size_x/y/z layout variables define the work group size.
		// gl_GlobalInvocationID is a uvec3 variable giving the global ID of the thread,
		// gl_LocalInvocationID is the local index within the work group, and
		// gl_WorkGroupID is the work group's index
		const char *csSrc[] = {
			"#version 430 core\n",
			"uniform float roll;\
         layout(r32f, location=0) writeonly uniform image2D destTex;\
         layout (local_size_x = 16, local_size_y = 16) in;\
         void main() {\
             ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);\
             float localCoef = length(vec2(ivec2(gl_LocalInvocationID.xy)-8)/8.0);\
             float globalCoef = sin(float(gl_WorkGroupID.x+gl_WorkGroupID.y)*0.1 + roll)*0.5;\
             imageStore(destTex, storePos, vec4(1.0, 0.0, 0.0, 0.0));\
         }"
		};

		glShaderSource(cs, 2, csSrc, NULL);
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

		//glUniform1i(glGetUniformLocation(ComputerShaderProgram, "destTex"), 0);// glGetUniformLocation(FrameBufferProgram, "srcTex"));
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
	void UseComputerShaderProgram()
	{
		glUseProgram(this->ComputerShaderProgram);
	}
};

#endif

