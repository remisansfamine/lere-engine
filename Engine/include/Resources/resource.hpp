#pragma once
#include <string>

namespace Resources
{
	class Resource
	{
	protected:
		std::string m_filePath;
		std::string m_name;

		Resource() = default;
		Resource(const std::string& filePath);
	public:
		std::string getPath();
	};
}