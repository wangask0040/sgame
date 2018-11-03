#pragma once

#include <string>
#include "common/tinyxml2.h"
#include <iostream>
#include "common/singleton.h"

class ILoadXml
{
public:
	ILoadXml() {}
	virtual ~ILoadXml() {}
	virtual void Clear() = 0;

	bool Load(const std::string& filefullpath)
	{
		tinyxml2::XMLDocument doc;
		auto ret = doc.LoadFile(filefullpath.c_str());
		if (ret == tinyxml2::XML_SUCCESS)
		{
			auto root = doc.RootElement();
			if (root != nullptr)
			{
				Clear();
				return Read(root);
			}
			else
			{
				std::cout << "load xml " << filefullpath << " root is null" << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << "load xml " << filefullpath << ", err:" << ret << std::endl;
			return false;
		}
	}

	virtual bool Read(tinyxml2::XMLElement* root) = 0;
};

