#pragma once
#include "Object.h"

#include <time.h>

class Apple :
    public Object
{
public:
    Apple(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID, glm::vec4 color, 
        float bufferHeight, float bufferWidth,
        unsigned int gridSize);

    void Start();

private:
    unsigned int m_GridSize;

    unsigned int m_BufferWidth, m_BufferHeight;
};

