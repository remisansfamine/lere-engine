#pragma once

#include "singleton.hpp"

namespace Core::Engine
{
	class EngineMaster final : public Singleton<EngineMaster>
	{
		friend class Singleton<EngineMaster>;

	private:
		EngineMaster();
		~EngineMaster();
		
		bool editMode = false;

		void toggleEditMode();

	public:
		static void update();
	};
}