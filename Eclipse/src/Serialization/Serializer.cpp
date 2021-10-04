#include "pch.h"
#include "Serializer.h"

namespace Eclipse
{
	Serializer::Serializer()
		: _currElement{ 0 }
	{
		Init();
	}

	void Serializer::Init()
	{
		TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
		_doc.LinkEndChild(decl);
	}

	void Serializer::GenerateDirectories(const std::string& path)
	{
		std::filesystem::path p{ path };
		std::filesystem::create_directories(p.remove_filename());
	}

	void Serializer::BaseSave(const std::string& savePath)
	{
		GenerateDirectories(savePath);
		_doc.SaveFile(savePath.c_str());
	}

	void Serializer::CleanUp()
	{
		_doc.Clear();
		_currElement = nullptr;
		Init();
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
			std::string finalName = ele_name + std::to_string(counter);
			newChild = new TiXmlElement(finalName.c_str());
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
		BaseSave(savePath);
		CleanUp();
	}

	void Serializer::SaveBackup(TiXmlDocument& backup, std::string& path)
	{
		BaseSave(path);
		backup = _doc;
		CleanUp();
	}

	Serializer::~Serializer()
	{

	}
}
