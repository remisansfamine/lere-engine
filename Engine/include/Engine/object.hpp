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
		virtual bool isActive();
		virtual void setActive(bool value);

		virtual void onDestroy() {}
		virtual void destroy() {}
	};
}