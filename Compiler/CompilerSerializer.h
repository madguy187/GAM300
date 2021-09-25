#pragma once
#include "TinyXML/tinyxml.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <type_traits>

namespace EclipseCompiler
{
    class Serializer
    {
        TiXmlDocument _doc;
        TiXmlElement* _currElement;

    public:

        Serializer() :
            _currElement{ 0 }
        {
            TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
            _doc.LinkEndChild(decl);
        }

        void StartElement(const std::string& ele_name, bool isMultiple = false, size_t counter = 0)
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

        void CloseElement()
        {
            if (_currElement && _currElement->Parent())
            {
                _currElement = _currElement->Parent()->ToElement();
            }
        }



        void SaveXML(const std::string& savePath)
        {
            std::filesystem::path p{ savePath };
            std::filesystem::create_directories(p.remove_filename());
            _doc.SaveFile(savePath.c_str());
        }

        ~Serializer()
        {
            SaveXML("Data\\Temp\\temp.xml");
        }

        //Add attribute to element
        template <typename T>
        void AddAttributeToElement(const std::string& att_name, const T& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), std::to_string(att_data).c_str());
        }

        template <>
        void AddAttributeToElement<int>(const std::string& att_name, const int& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), att_data);
        }

        template <>
        void AddAttributeToElement<double>(const std::string& att_name, const double& att_data)
        {
            _currElement->SetDoubleAttribute(att_name.c_str(), att_data);
        }

        template <>
        void AddAttributeToElement<float>(const std::string& att_name, const float& att_data)
        {
            _currElement->SetDoubleAttribute(att_name.c_str(), att_data);
        }

        template <>
        void AddAttributeToElement<std::string>(const std::string& att_name, const std::string& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), att_data.c_str());
        }

        template <>
        void AddAttributeToElement<const char*>(const std::string& att_name, const char* const& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), att_data);
        }

        template <>
        void AddAttributeToElement<char>(const std::string& att_name, const char& att_data)
        {
            std::string c{ att_data };
            _currElement->SetAttribute(att_name.c_str(), c.c_str());
        }

        template <>
        void AddAttributeToElement<bool>(const std::string& att_name, const bool& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), att_data ? "true" : "false");
        }
    };
}