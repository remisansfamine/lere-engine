#pragma once

#include <memory>

namespace Engine
{
	class Object
	{
	private:
		bool isEnable = true;

	public:
		bool virtual isActive();
		void virtual setActive(bool value);
		
		void virtual destroy() {}
	};
}