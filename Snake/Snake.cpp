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

	m_SnakeNodes.push_back(std::shared_ptr<Object>(new Object(m_Mesh, m_Direction * (float)m_GridSize, m_Scale, m_ModelLocation, m_ShaderID, m_Color)));
}

bool Snake::Update(bool* keys, GLfloat& timeAcc, std::shared_ptr<Apple> apple)
{
	if (keys[m_KeyUp] && m_ViableKeyPresses[m_KeyUp])
	{
		m_ViableKeyPresses[m_KeyUp] = true;
		m_ViableKeyPresses[m_KeyDown] = false;
		m_ViableKeyPresses[m_KeyLeft] = true;
		m_ViableKeyPresses[m_KeyRight] = true;
		m_Direction = glm::vec2(0.0f, 1.0f);
	}
	else if (keys[m_KeyDown] && m_ViableKeyPresses[m_KeyDown])
	{
		m_ViableKeyPresses[m_KeyDown] = true;
		m_ViableKeyPresses[m_KeyUp] = false;
		m_ViableKeyPresses[m_KeyLeft] = true;
		m_ViableKeyPresses[m_KeyRight] = true;
		m_Direction = glm::vec2(0.0f, -1.0f);
	}
	else if (keys[m_KeyLeft] && m_ViableKeyPresses[m_KeyLeft])
	{
		m_ViableKeyPresses[m_KeyLeft] = true;
		m_ViableKeyPresses[m_KeyRight] = false;
		m_ViableKeyPresses[m_KeyUp] = true;
		m_ViableKeyPresses[m_KeyDown] = true;
		m_Direction = glm::vec2(-1.0f, 0.0f);
	}
	else if (keys[m_KeyRight] && m_ViableKeyPresses[m_KeyRight])
	{
		m_ViableKeyPresses[m_KeyRight] = true;
		m_ViableKeyPresses[m_KeyLeft] = false;
		m_ViableKeyPresses[m_KeyUp] = true;
		m_ViableKeyPresses[m_KeyDown] = true;
		m_Direction = glm::vec2(1.0f, 0.0f);
	}

	if (timeAcc > m_TimeAccThresh)
	{
		m_Position += m_Direction * (float)m_GridSize;
		m_SnakeNodes.push_back(std::shared_ptr<Object>(new Object(m_Mesh, m_Position, m_Scale, m_ModelLocation, m_ShaderID, m_Color)));
		m_SnakeNodes.pop_front();
		timeAcc = 0.0f;
	}

	if (m_Position.x > ( m_BufferWidth / 2) + m_Scale.x || m_Position.y > ( m_BufferHeight / 2) + m_Scale.y ||
		m_Position.x < (-m_BufferWidth / 2) - m_Scale.x || m_Position.y < (-m_BufferHeight / 2) - m_Scale.y )
		return false;

	for (auto &i : m_SnakeNodes)
		if(i->GetPosition() == m_Position && i != m_SnakeNodes.back())
			return false;

	if (m_Position == apple->GetPosition())
	{
		m_SnakeNodes.push_front(std::shared_ptr<Object>(new Object(m_Mesh, 
			m_SnakeNodes.front()->GetPosition() - (m_Direction * (float)m_GridSize),
			m_Scale, m_ModelLocation, m_ShaderID, m_Color)));
		SpawnApple(apple);
	}

	return true;
}

void Snake::SpawnApple(std::shared_ptr<Apple> apple)
{
	apple->Start();
	for (auto& i : m_SnakeNodes)
	{
		if (i->GetPosition() == apple->GetPosition() && i != m_SnakeNodes.back())
		{
			SpawnApple(apple);
			return;
		}
	}
}

void Snake::Render()
{
	for (int i = 0; i < m_SnakeNodes.size(); i++)
		m_SnakeNodes[i]->Render();
}