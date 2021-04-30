#include "resource.hpp"

namespace Resources
{
	Resource::Resource(const std::string& filePath)
		: m_filePath(filePath)
	{
		// TODO
		//m_name = Utils::getNameFromPath(filePath);
	}

	std::string Resource::getPath()
	{
		return m_filePath;
	}
}