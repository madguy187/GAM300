#include "pch.h"
#include "Deserializer.h"

Deserializer::Deserializer() :
	_currElement{ 0 },
	hasFile{ false }
{

}

bool Deserializer::StartElement(const std::string& ele_name, bool isMultiple, size_t counter)
{
	std::string finalName = ele_name;
	bool result = false;
	if (isMultiple)
	{
		finalName += std::to_string(counter);
	}

	if (_currElement)
	{
		if (_currElement->FirstChild(finalName.c_str()))
		{
			_currElement = _currElement->FirstChild(finalName.c_str())->ToElement();
			
			result = true;
		}
	}
	else
	{
		if (_doc.FirstChild(finalName.c_str()))
		{
			_currElement = _doc.FirstChild(finalName.c_str())->ToElement();
			result = true;
		}
	}
	return result;
}

void Deserializer::CloseElement()
{
	if (_currElement && _currElement->Parent())
	{
		_currElement = _currElement->Parent()->ToElement();
	}
}

void Deserializer::IterateChildElement()
{
	if (_currElement->NoChildren())
	{
		std::ostringstream os;
		os << "No childeren in \"" << _currElement->Value() << "\" element!";
		ENGINE_CORE_WARN(os.str())
		return;
	}

	_currElement = _currElement->FirstChildElement();
	TiXmlElement* endElement = _currElement->Parent()->LastChild()->ToElement();
	while (_currElement != endElement)
	{
		std::cout << _currElement->Value() << std::endl;
		_currElement = _currElement->NextSiblingElement();
	}
	std::cout << _currElement->Value() << std::endl;
	CloseElement();
}

bool Deserializer::LoadXML(const std::string& loadPath)
{
	_path = loadPath;
	bool result = _doc.LoadFile(loadPath.c_str());
	hasFile = result;
	if (!result)
	{
		std::ostringstream os;
		os << "Fail to load the file \"" << _path.filename() << "\"." << 
			_doc.ErrorDesc() << " at Row " << _doc.ErrorRow() << " Column " 
			<< _doc.ErrorCol();
		ENGINE_CORE_WARN(os.str().c_str())
	}
	else
	{
		std::ostringstream os;
		os << "File \"" << _path.filename() << "\"" << " is loaded successfuly.";
		ENGINE_CORE_INFO(os.str().c_str())
	}

	return result;
}

Deserializer::~Deserializer()
{

}