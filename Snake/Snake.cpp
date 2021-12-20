#include "Snake.h"

Snake::Snake(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID, glm::vec4 color,
	int gridSize, GLenum keyUp, GLenum keyDown, GLenum keyLeft, GLenum keyRight, 
	float bufferWidth, float bufferHeight,
	float timeAccThresh)
	: Object(mesh, position, scale, modelLocation, shaderID, color)
{
	m_GridSize = gridSize;
	m_Direction = glm::vec2(0.0f, 0.0f);

	m_KeyUp = keyUp;
	m_KeyDown = keyDown;
	m_KeyLeft = keyLeft;
	m_KeyRight = keyRight;

	m_ViableKeyPresses[m_KeyUp] = true;
	m_ViableKeyPresses[m_KeyDown] = true;
	m_ViableKeyPresses[m_KeyLeft] = true;
	m_ViableKeyPresses[m_KeyRight] = true;

	m_BufferHeight = bufferHeight;
	m_BufferWidth = bufferWidth;

	m_TimeAccThresh = timeAccThresh;

	m_HasMoved = false;

	m_SnakeNodes.push_front(Object(m_Mesh, m_Direction * (float)m_GridSize, m_Scale, m_ModelLocation, m_ShaderID, m_Color));
}

bool Snake::Update(bool* keys, GLfloat& timeAcc, std::shared_ptr<Apple> apple)
{
	if (keys[m_KeyUp] && m_ViableKeyPresses[m_KeyUp] && m_HasMoved == true)
	{
		m_ViableKeyPresses[m_KeyUp] = true;
		m_ViableKeyPresses[m_KeyDown] = false;
		m_ViableKeyPresses[m_KeyLeft] = true;
		m_ViableKeyPresses[m_KeyRight] = true;
		m_Direction = glm::vec2(0.0f, 1.0f);
		m_HasMoved = false;
	}
	if (keys[m_KeyDown] && m_ViableKeyPresses[m_KeyDown] && m_HasMoved == true)
	{
		m_ViableKeyPresses[m_KeyDown] = true;
		m_ViableKeyPresses[m_KeyUp] = false;
		m_ViableKeyPresses[m_KeyLeft] = true;
		m_ViableKeyPresses[m_KeyRight] = true;
		m_Direction = glm::vec2(0.0f, -1.0f);
		m_HasMoved = false;
	}
	if (keys[m_KeyLeft] && m_ViableKeyPresses[m_KeyLeft] && m_HasMoved == true)
	{
		m_ViableKeyPresses[m_KeyLeft] = true;
		m_ViableKeyPresses[m_KeyRight] = false;
		m_ViableKeyPresses[m_KeyUp] = true;
		m_ViableKeyPresses[m_KeyDown] = true;
		m_Direction = glm::vec2(-1.0f, 0.0f);
		m_HasMoved = false;
	}
	if (keys[m_KeyRight] && m_ViableKeyPresses[m_KeyRight] && m_HasMoved == true)
	{
		m_ViableKeyPresses[m_KeyRight] = true;
		m_ViableKeyPresses[m_KeyLeft] = false;
		m_ViableKeyPresses[m_KeyUp] = true;
		m_ViableKeyPresses[m_KeyDown] = true;
		m_Direction = glm::vec2(1.0f, 0.0f);
		m_HasMoved = false;
	}

	if (timeAcc > m_TimeAccThresh)
	{
		m_Position += m_Direction * (float)m_GridSize;
		m_SnakeNodes.push_front(Object(m_Mesh, m_Position, m_Scale, m_ModelLocation, m_ShaderID, m_Color));
		m_SnakeNodes.pop_back();
		timeAcc = 0.0f;
		m_HasMoved = true;
	}

	if (m_Position.x > ( m_BufferWidth / 2) || m_Position.y > ( m_BufferHeight / 2) ||
		m_Position.x < (-m_BufferWidth / 2) || m_Position.y < (-m_BufferHeight / 2))
		return false;

	if (m_SnakeNodes.size() > 3)
		for (std::deque<Object>::iterator i = m_SnakeNodes.begin() + 1; i < m_SnakeNodes.end(); i++)
			if (i->GetPosition() == m_Position)
				return false;

	if (m_Position == apple->GetPosition())
	{
		m_SnakeNodes.push_back(Object(m_Mesh, 
			m_SnakeNodes.back().GetPosition(),
			m_Scale, m_ModelLocation, m_ShaderID, m_Color));
		SpawnApple(apple);
	}

	return true;
}

void Snake::SpawnApple(std::shared_ptr<Apple> apple)
{
	apple->Start();
	if (m_SnakeNodes.size() > 3)
	{
		for (std::deque<Object>::iterator i = m_SnakeNodes.begin() + 1; i < m_SnakeNodes.end(); i++)
		{
			if (i->GetPosition() == apple->GetPosition())
			{
					SpawnApple(apple);
					return;
			}
		}
	}
}

void Snake::Render()
{
	for (int i = 0; i < m_SnakeNodes.size(); i++)
		m_SnakeNodes[i].Render();
}