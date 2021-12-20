#include "Apple.h"

Apple::Apple(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID, glm::vec4 color,
	float bufferWidth, float bufferHeight,
	unsigned int gridSize)
	: Object(mesh, position, scale, modelLocation, shaderID, color)
{
	m_GridSize = gridSize;
	m_BufferWidth = bufferWidth;
	m_BufferHeight = bufferHeight;
}

void Apple::Start()
{
	srand(time(NULL));

	float randNum1 = (float)(rand() % (int)((m_BufferWidth / (m_GridSize * 2)) + 1));
	float randNum2 = (float)(rand() % (int)((m_BufferHeight / (m_GridSize * 2)) + 1));

	if (rand() % 10 > 5)
	{
		randNum1 = -randNum1;
	}

	if (rand() % 10 > 5)
	{
		randNum2 = -randNum2;
	}

	m_Position = glm::vec2(randNum1 * m_GridSize, randNum2 * m_GridSize);
}