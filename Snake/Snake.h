#pragma once
#include "Object.h"
#include "Apple.h"

#include <map>
#include <queue>

class Snake :
    public Object
{
public:
    Snake(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID, glm::vec4 color,
        int gridSize, GLenum keyUp, GLenum keyDown, GLenum keyLeft, GLenum keyRight, 
        float bufferHeight, float bufferWidth,
        float timeAccThresh);

    bool Update(bool* keys, GLfloat& timeAcc, std::shared_ptr<Apple> apple);

    virtual void Render();

    void SpawnApple(std::shared_ptr<Apple> apple);

    glm::vec2 GetDirection() { return m_Direction; }

private:
    int m_GridSize;
    glm::vec2 m_Direction;

    std::map<GLenum, bool> m_ViableKeyPresses;

    GLenum m_KeyUp, m_KeyDown, m_KeyLeft, m_KeyRight;

    GLfloat m_BufferWidth, m_BufferHeight;

    float m_TimeAccThresh;
    bool m_HasMoved;

    std::deque<Object> m_SnakeNodes;
};

