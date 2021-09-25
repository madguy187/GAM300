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
    public:
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

        template <typename T, size_t N1, size_t N2, glm::qualifier GLM>
        inline void AddAttributeToElement(const std::string& att_name, const glm::mat<N1, N2, T, GLM>& att_data)
        {
            AddAttributeToElement<size_t>("Col", N1);
            AddAttributeToElement<size_t>("Row", N2);
            std::string name{ "Col" };
            for (size_t i = 0; i < N1; ++i)
            {
                std::string dataName{ "_" + std::to_string(i) };
                StartElement(name, true, i);
                for (size_t j = 0; j < N2; ++j)
                {
                    AddAttributeToElement<T>(dataName + std::to_string(j), att_data[i][j]);
                }
                CloseElement();
            }
        }

        template <typename T>
        inline void AddAttributeToElement(const std::string& att_name, const std::vector<T>& att_data)
        {
            size_t counter = 0;
            std::string name{ att_name + " member" };
            for (const T& data : att_data)
            {
                StartElement(name, true, counter++);
                AddAttributeToElement<T>("member", data);
                CloseElement();
            }
        }

        template <typename T, size_t N, glm::qualifier GLM>
        inline void AddAttributeToElement(const std::string& att_name, const glm::vec<N, T, GLM>& att_data)
        {
            std::string vecNames[4] = { {"x"}, {"y"}, {"z"}, {"w"} };
            for (size_t i = 0; i < N; ++i)
            {
                AddAttributeToElement<T>(vecNames[i], att_data[i]);
            }
        }

        //inline void SerializeData() {}

        //template <typename T, typename... Args>
        //inline void SerializeData(const char* name, T element, Args... elements)
        //{
        //    StartElement(name);
        //    AddAttributeToElement("value", element);
        //    CloseElement();
        //    SerializeData(elements...);
        //}

        //template <typename CompType>
        //inline void SerializeComponentData(const CompType& data)
        //{
        //    AddAttributeToElement("Unavailable", true);
        //}

        //template <>
        //inline void SerializeComponentData<Mesh>(const Mesh& data)
        //{
        //    SerializeData(
        //        "MeshName", data.MeshName,
        //        "NoTextures", data.NoTex,
        //        "Diffuse", data.Diffuse
        //    );
        //}

        //template <typename CompType>
        //inline void SerializeComponent(Mesh& ent)
        //{
        //    StartElement("MeshTest");
        //    SerializeComponentData<Mesh>(ent);
        //    CloseElement();
        //}
    };
}