#pragma once

//#include <glad/glad.h>

//#include "entity.hpp"

class LifeBar
{
private:
    int m_lifeMax;
    int m_currentLength;
    int m_lengthMax;

public:
    LifeBar() = default;
    LifeBar(enemy* enemy); // entity* entity
    ~LifeBar() {}

    void drawBar(enemy* enemy); // entity* entity
};
