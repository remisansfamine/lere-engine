#pragma once
#include <string>

#include "entity_life.hpp"

namespace Gameplay
{
	class PlayerLife : public EntityLife
	{
	private:

		std::string lifeBarName;

	public:
		PlayerLife(Engine::GameObject& gameObject);

		void start() override;
		void update() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}