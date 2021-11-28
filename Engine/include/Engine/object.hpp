#pragma once

#include <memory>

namespace Engine
{
	class Object
	{
	private:
		bool isEnable = true;

	protected:

	public:
		bool virtual isActive() const;
		void virtual setActive(bool value);
		
		void virtual onDestroy() {}
		void virtual destroy() {}
	};
}