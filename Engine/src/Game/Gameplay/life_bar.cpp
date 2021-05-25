#include "life_bar.hpp"

LifeBar::LifeBar(enemy* enemy)
{
    m_lifeMax = enemy->life;
    //m_lengthMax = ENEMY_SIZE;
    m_currentLength = m_lifeMax;
}

void LifeBar::drawBar(enemy* enemy)
{
    if (m_lifeMax == 0 || m_lengthMax == 0)
        m_currentLength = 0;
    else
        m_currentLength = ((enemy->life) / m_lifeMax) * m_lengthMax;

    //glDrawRectFilled(game->m_gp, { enemy->m_position.X(), enemy->m_position.Y() - 12, m_currentLength, 5 }, { 0.f, 1.f, 0.f, 1.f });
}