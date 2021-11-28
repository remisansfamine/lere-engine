#pragma once

#include <memory>

#include "component.hpp"
#include "transform.hpp"

namespace Gameplay
{
    class LifeBar : public Engine::Component
    {
    private:
        Physics::TransformComponent* pivot = nullptr;

    public:
        LifeBar(Engine::Entity& owner);

        void start() override;
        void updateSprite(int life, int maxLife);

        std::string toString() const;

        static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
    };
}