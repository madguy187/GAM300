#pragma once
#include "Global.h"
#include "TinyXML/tinyxml.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <type_traits>


namespace Eclipse
{
	class Deserializer
	{
		TiXmlDocument _doc;
		TiXmlElement* _currElement;
		std::filesystem::path _path;
		bool hasFile;

	public:
		Deserializer();

		bool StartElement(const std::string& ele_name, bool isMultiple = false, size_t counter = 0);

		void CloseElement();

		void IterateChildrenOfElement();

		bool LoadXML(const std::string& savePath);

		~Deserializer();
	};
}