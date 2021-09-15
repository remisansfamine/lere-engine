#include "recipe.hpp"

#include "debug.hpp"
#include <errno.h>
#include <string.h>
#include <locale.h>
#include <stdio.h>

#include "thread_pool.hpp"

namespace Resources
{
	Recipe::Recipe(const std::string& filePath)
		: Resource(filePath)
	{
		//ThreadPool::addTask(std::bind(&Recipe::load, this, filePath));
		load(filePath);
	}

	void Recipe::load(const std::string& filePath)
	{
		std::ifstream ifs(filePath);

		// Check if the file can be read
		if (ifs.fail())
		{
			std::string error = std::system_error(errno, std::system_category()).code().message();
			Core::Debug::Log::error("Cannot load the recipe " + filePath + ": " + error);
		}

		recipe.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
	}
}