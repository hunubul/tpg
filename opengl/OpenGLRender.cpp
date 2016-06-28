// Std. Includes
#include <string>
#include <cmath>
#include "Source.h"

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Vertices.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>

// Properties
const GLuint screenWidth = 1584, screenHeight = 852;
bool ASCIION = true;

//Shaders location
std::string texturesVertPath = "./shaders/textures.vert";
std::string texturesFragPath = "./shaders/textures.frag";
std::string framebufferVertPath = "./shaders/framebuffer.vert";
std::string framebufferFragPath = "./shaders/framebuffer.frag";
std::string computeASCIIShaderPath = "./shaders/computeASCII.shd";

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// The MAIN function, from here we start our application and run our Game loop
void initAll()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	//glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

	/*GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);*/

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();
	GLenum err = glGetError(); //Catch error code 0x500 (1280)

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders
	Shader defaultShader(texturesVertPath, texturesFragPath);
	Shader framebufferShader(framebufferVertPath, framebufferFragPath);
	ComputeShader computeASCIIShader(computeASCIIShaderPath);

	GLuint VAO, VBO;
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO
	}

	GLuint VAO_FrameBuff, VBO_FrameBuff;
	{
		glGenVertexArrays(1, &VAO_FrameBuff);
		glGenBuffers(1, &VBO_FrameBuff);
		// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
		glBindVertexArray(VAO_FrameBuff);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_FrameBuff);
		glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

		GLint posAttrib = glGetAttribLocation(framebufferShader.Program, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

		GLint texAttrib = glGetAttribLocation(framebufferShader.Program, "texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

		glBindVertexArray(0); // Unbind VAO
	}

	// Load and create a texture
	// --== TEXTURE 1 == --
	GLuint texture1;
	{
		int width, height;
		unsigned char* image;
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
		// Set our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Load, create texture and generate mipmaps
		image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	}

	// --== TEXTURE 2 == --
	GLuint texture2;
	{
		int width, height;
		unsigned char* image;
		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// Set our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Load, create texture and generate mipmaps
		image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//--=Framebuffer=--
	GLuint frameBuffer, texColorBuffer, rboDepthStencil;
	{
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		//Texture for framebuffer
		glGenTextures(1, &texColorBuffer);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
			);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0
			);
		glBindTexture(GL_TEXTURE_2D, 0);
		// Depth stencil for framebuffer
		glGenRenderbuffers(1, &rboDepthStencil);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
		glFramebufferRenderbuffer(
			GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil
			);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//--=Downsampled framebuffer=--
	GLuint frameDownBuffer, texColorDownBuffer, rboDownDepthStencil;
	{
		glGenFramebuffers(1, &frameDownBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameDownBuffer);
		//Downsampled Texture for framebuffer
		glGenTextures(1, &texColorDownBuffer);
		glBindTexture(GL_TEXTURE_2D, texColorDownBuffer);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA, screenWidth / FontX, screenHeight / FontY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
			);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorDownBuffer, 0
			);
		glBindTexture(GL_TEXTURE_2D, 0);
		// Depth stencil for framebuffer
		glGenRenderbuffers(1, &rboDownDepthStencil);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDownDepthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth / FontX, screenHeight / FontY);
		glFramebufferRenderbuffer(
			GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDownDepthStencil
			);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int fontSize = fontChar.size();
	//--=ASCII as texture=--
	GLuint ASCIITexture;
	{
		glGenTextures(1, &ASCIITexture);
		glBindTexture(GL_TEXTURE_2D, ASCIITexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FontX, FontY*fontSize, 0, GL_RED, GL_UNSIGNED_BYTE, fontChar.ASCIIChar.data());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//--=ASCII Brightness as texture=--
	GLuint ASCIIBrightness;
	{
		glGenTextures(1, &ASCIIBrightness);
		glBindTexture(GL_TEXTURE_2D, ASCIIBrightness);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fontChar.ASCIIBrightness.size(), 1, 0, GL_RED, GL_UNSIGNED_BYTE, fontChar.ASCIIBrightness.data());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//--=ASCII Weights as texture=--
	float* asciiScreenWeights = new float[fontSize * screenWidth / FontX * screenHeight / FontY];
	GLuint ASCIIScreenWeights;
	{
		glGenTextures(1, &ASCIIScreenWeights);
		glBindTexture(GL_TEXTURE_2D, ASCIIScreenWeights);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, fontSize/4 * screenWidth / FontX, screenHeight / FontY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//--=ASCII Max Indexes as texture=--
	float* asciiMaxIndexes = new float[screenWidth / FontX * screenHeight / FontY];
	GLuint ASCIIMaxIndexes;
	{
		glGenTextures(1, &ASCIIMaxIndexes);
		glBindTexture(GL_TEXTURE_2D, ASCIIMaxIndexes);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, screenWidth / FontX, screenHeight / FontY, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//std::vector<std::vector<std::vector<unsigned short>>> screenHistogram;
	//unsigned short brightHistogram = 0;

	// Game loop
	while (!glfwWindowShouldClose(window)) {
		// Set frame time
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw our first triangle
		defaultShader.UseProgram();

		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(defaultShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(defaultShader.Program, "ourTexture2"), 1);

		// Create camera transformation
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(defaultShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(defaultShader.Program, "view");
		GLint projLoc = glGetUniformLocation(defaultShader.Program, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		for (GLuint i = 0; i < 10; i++) {
			// Calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);


		glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);	 // READ: Supersampled
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameDownBuffer); // WRITE

		// Downsample the supersampled FBO using LINEAR interpolation
		glBlitFramebuffer(
			0, 0, screenWidth, screenHeight,
			0, 0, screenWidth / FontX, screenHeight / FontY,
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR
		);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*GLenum err = GL_NO_ERROR;
		while ((err = glGetError()) != GL_NO_ERROR) {
			printf("Error: %x\n", err);
		}*/

		if (ASCIION) {
			computeASCIIShader.UseComputerShaderProgram();
			glBindImageTexture(0, texColorBuffer, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
			glBindImageTexture(1, texColorDownBuffer, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
			glBindImageTexture(2, ASCIITexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
			glBindImageTexture(3, ASCIIBrightness, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
			glBindImageTexture(4, ASCIIScreenWeights, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
			glUniform1f(glGetUniformLocation(computeASCIIShader.ComputerShaderProgram, "FontSize"), (GLfloat)fontSize);

			glDispatchCompute(1, screenWidth / FontX, screenHeight / FontY); // screenWidth/FontX screenHeight/FontY threads in blocks of FontX*FontY

			glMemoryBarrier(GL_ALL_BARRIER_BITS);

			glBindTexture(GL_TEXTURE_2D, ASCIIScreenWeights);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, asciiScreenWeights);
			glBindTexture(GL_TEXTURE_2D, 0);

			float maxWeight = 0.0;
			int index = 0;

			for (GLuint y = 0; y < screenHeight / FontY; y++) {
				for (GLuint x = 0; x < screenWidth / FontX; x++) {
					for (int i = 0; i < fontSize; i++) {
						if (i == 0) {
							index = 0;
							maxWeight = asciiScreenWeights[y*(fontSize * screenWidth / FontX) + x * fontSize + i];
						}
						if (maxWeight < asciiScreenWeights[y*(fontSize * screenWidth / FontX) + x * fontSize + i]) {
							maxWeight = asciiScreenWeights[y*(fontSize * screenWidth / FontX) + x * fontSize + i];
							index = i;
						}
					}
					asciiMaxIndexes[y*screenWidth / FontX + x] = (float)index;
				}
			}

			glBindTexture(GL_TEXTURE_2D, ASCIIMaxIndexes);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, screenWidth / FontX, screenHeight / FontY, 0, GL_RED, GL_FLOAT, asciiMaxIndexes);
			glBindTexture(GL_TEXTURE_2D, 0);
		}


		glBindVertexArray(VAO_FrameBuff);

		framebufferShader.UseProgram();
		glBindImageTexture(0, texColorBuffer, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindImageTexture(1, ASCIITexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindImageTexture(2, ASCIIMaxIndexes, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16F);
		glUniform1i(glGetUniformLocation(framebufferShader.Program, "ASCIION"),ASCIION);
		glUniform1f(glGetUniformLocation(framebufferShader.Program, "screenWidth"), (GLfloat)screenWidth);
		glUniform1f(glGetUniformLocation(framebufferShader.Program, "screenHeight"), (GLfloat)screenHeight);
		glUniform1f(glGetUniformLocation(framebufferShader.Program, "FontX"), (GLfloat)FontX);
		glUniform1f(glGetUniformLocation(framebufferShader.Program, "FontY"), (GLfloat)FontY);
		glUniform1f(glGetUniformLocation(framebufferShader.Program, "FontSize"), (GLfloat)fontSize);
		glUniform1f(glGetUniformLocation(framebufferShader.Program, "FPS"), (GLfloat)1.0/deltaTime);


		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteVertexArrays(1, &VAO_FrameBuff);
	glDeleteBuffers(1, &VBO_FrameBuff);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		if (ASCIION) ASCIION = false;
		else		ASCIION = true;
	}
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)(xpos - lastX);
	GLfloat yoffset = (GLfloat)(lastY - ypos);  // Reversed since y-coordinates go from bottom to left

	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((GLfloat)yoffset);
}