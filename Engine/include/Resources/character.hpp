#pragma once

#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "maths.hpp"
#include "component.hpp"
#include "shader.hpp"

namespace Resources
{
	struct Character
	{
		unsigned int ID;
		Core::Maths::vec2 size;
		Core::Maths::vec2 bearing;
		unsigned int advance;
	};

	class Font
	{
	private:
		std::unordered_map<char, Character> charMap;
	public:
		Font(const std::string& path);
	};

	class Text : public Engine::Component
	{
	private:
		std::shared_ptr<ShaderProgram> m_shaderProgram = nullptr;
		std::shared_ptr<Font> m_font = nullptr;

	public:
		Text(Engine::Entity& owner, const std::string& path);
		~Text();

		void draw() const;

		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
	};
}