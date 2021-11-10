#include "pch.h"
#include "Deserializer.h"

namespace Eclipse
{
	void Deserializer::SetMonoScriptPointer(const std::string& scriptName, MonoScript*& att_data)
	{
		att_data = engine->mono.GetScriptPointerByName(scriptName);
	}

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

	void Deserializer::IterateChildrenOfElement()
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

			if (!_currElement->NoChildren())
			{
				auto _currChildElement = _currElement->FirstChildElement();
				TiXmlElement* endChildElement = _currElement->LastChild()->ToElement();

				while (_currChildElement != endChildElement)
				{
					std::cout << _currChildElement->Value() << std::endl;

					_currChildElement = _currChildElement->NextSiblingElement();
				}
			}

			_currElement = _currElement->NextSiblingElement();
		}
		std::cout << _currElement->Value() << std::endl;
		CloseElement();
	}

	bool Deserializer::LoadXML(const std::string& loadPath)
	{
		_path = loadPath;
		hasFile = _doc.LoadFile(loadPath.c_str());

		if (!hasFile)
		{
			std::ostringstream os;
			os << "Fail to load the file \"" << _path.filename() << "\"." <<
				_doc.ErrorDesc() << " at Row " << _doc.ErrorRow() << " Column "
				<< _doc.ErrorCol();
			EDITOR_LOG_WARN(os.str().c_str())
		}
		else
		{
			std::ostringstream os;
			os << "File \"" << _path.filename() << "\"" << " is loaded successfuly.";
			EDITOR_LOG_INFO(os.str().c_str())
		}

		return hasFile;
	}

	bool Deserializer::LoadBackup(TiXmlDocument& backup, const std::string& path)
	{
		if (std::filesystem::exists(path))
		{
			_doc = backup;
			hasFile = true;
			//hasFile = backup.LoadFile(path.c_str());
		}
		else
		{
			hasFile = false;
			EDITOR_LOG_INFO("Temp.scn does not exist.")
		}

		if (!hasFile)
		{
			EDITOR_LOG_INFO("Failed to load backup Temp.scn.")
		}
		else
		{

		}

		return hasFile;
	}

	Deserializer::~Deserializer()
	{

	}

	bool Deserializer::CheckAttributeValue(const std::string& name)
	{
		if (_currElement->Attribute(name.c_str()))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Deserializer::GetAttributeValue(const std::string& name, std::string& result)
	{
		int temp = 0;

		int queryResult = _currElement->QueryIntAttribute(name.c_str(), &temp);

		if (queryResult == TIXML_NO_ATTRIBUTE)
		{
			std::string msg = "Failed to obtain attribute value of \"";
			msg += name + " during deserialization!";
			EDITOR_LOG_WARN(msg.c_str());
			return false;
		}

		result = _currElement->Attribute(name.c_str());

		return true;
	}
}