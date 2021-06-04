#pragma once

#pragma once

#include <memory>
#include <vector>

#include "component.hpp"
#include "transform.hpp"
#include "sprite_renderer.hpp"

namespace Gameplay
{
    class AmmoCounter : public Engine::Component
    {
    private:
        std::vector<Physics::Transform*> ammoTransforms;
        std::vector<Physics::Transform*> ammoShooted;

        float animationSpeed = 5.f;
        float initialXPos = 0.f;

        int previousCount = 0;

    public:
        AmmoCounter(Engine::GameObject& gameObject);

        void updateHud(int ammoCount);
        void start() override;
        void update() override;

        void reload();

        std::string toString() const override;
        static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
    };
}
