#include "pch.h"
#include "Serializer.h"

namespace Eclipse
{
	Serializer::Serializer()
		: _currElement{ 0 }
	{
		TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
		_doc.LinkEndChild(decl);
	}

	void Serializer::StartElement(const std::string& ele_name, bool isMultiple, size_t counter)
	{

		TiXmlElement* newChild;
		if (!isMultiple)
		{
			newChild = new TiXmlElement(ele_name.c_str());
		}
		else
		{
			std::string final_name = ele_name + std::to_string(counter);
			newChild = new TiXmlElement(final_name.c_str());
		}


		if (!_currElement)
		{
			_doc.LinkEndChild(newChild);
		}
		else
		{
			_currElement->LinkEndChild(newChild);
		}

		_currElement = newChild;
	}

	void Serializer::CloseElement()
	{
		if (_currElement && _currElement->Parent())
		{
			_currElement = _currElement->Parent()->ToElement();
		}
	}

	void Serializer::SaveXML(const std::string& savePath)
	{
		std::filesystem::path p{ savePath };
		std::filesystem::create_directories(p.remove_filename());
		_doc.SaveFile(savePath.c_str());
	}

	Serializer::~Serializer()
	{
		SaveXML("Data\\Temp\\Temp.xml");
	}
}