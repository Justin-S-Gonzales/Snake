#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>
#include <vector>
#include <memory>

#include "Window.h"
#include "Shader.h"
#include "Rectangle.h"
#include "Object.h"
#include "Snake.h"

int main()
{
	// Delta time
	GLfloat now;
	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = 0.0f;

	GLfloat timeAcc = 0.0f;

	Window window;

	float width = 525;
	float height = 325;

	// Windowwwwwwdd
	window = Window(width, height);
	window.CreateWindow();

	// Shader
	Shader shader = Shader();
	shader.CreateFromFiles("Shaders/shader.vert", "Shaders/shader.frag");
	shader.UseShader();
	shader.Validate();

	// Projection matrix uniform
	glm::mat4 projection = glm::ortho((GLfloat)(-(window.GetBufferWidth() / 2)), (GLfloat)(window.GetBufferWidth() / 2),
		(GLfloat)(-(window.GetBufferHeight() / 2)), (GLfloat)(window.GetBufferHeight() / 2));

	GLuint uProjection = shader.GetProjectionLocation();

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

	// Get model matrix location
	GLuint uModel = shader.GetModelLocation();

	std::shared_ptr<Rectangle> rect(new Rectangle());

	int gridSize = 25;

	GLenum keyUp = GLFW_KEY_W;
	GLenum keyDown = GLFW_KEY_S;
	GLenum keyRight = GLFW_KEY_D;
	GLenum keyLeft = GLFW_KEY_A;

	std::shared_ptr<Snake> snake(new Snake(rect, glm::vec2(0.0f, 0.0f), 
		glm::vec2((float)gridSize, (float)gridSize), uModel, shader.GetShaderID(),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 25, keyUp, keyDown, keyLeft, keyRight, 
		window.GetWidth(), window.GetHeight(), 0.23f));

	std::shared_ptr<Apple> apple(new Apple(rect, glm::vec3(0.0f, 100.0f, 0.0f), glm::vec2((float)gridSize, (float)gridSize), uModel,
		shader.GetShaderID(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 
		window.GetWidth(), window.GetHeight(), gridSize));

	// Main loop
	window.GetKeys()[keyUp] = false;
	window.GetKeys()[keyDown] = false;
	window.GetKeys()[keyLeft] = false;
	window.GetKeys()[keyRight] = false;

	//apple->Start();
	while (!window.GetShouldClose())
	{
			now = glfwGetTime();
			deltaTime = now - lastTime;
			lastTime = now;

			timeAcc += deltaTime;

			glfwPollEvents();

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			if (!snake->Update(window.GetKeys(), timeAcc, apple))
			{
				std::cout << "GAME OVER" << std::endl;
				return 0;
			}

			snake->Render();
			apple->Render();

			window.SwapBuffers();
	}
}