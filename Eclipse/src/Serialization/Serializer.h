/*
Attribute available to serialize for each data:
int series, size_t
char
bool
string
const char*
float, double
***/

#pragma once
#include "Global.h"
#include "TinyXML/tinyxml.h"
#include "Library/Strings/Lexical.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <type_traits>
//#include "../Reflection/registration.h"

namespace Eclipse
{
    class Serializer
    {
        TiXmlDocument _doc;
        TiXmlElement* _currElement;

    public:

        Serializer();

        void StartElement(const std::string& ele_name, bool isMultiple = false, size_t counter = 0);

        void CloseElement();

        void SaveXML(const std::string& savePath);

        ~Serializer();

        //Add attribute to element
        template <typename T>
        inline void AddAttributeToElement(const std::string& att_name, const T& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), lexical_cast<std::string>(att_data).c_str());
        }

        template <>
        inline void AddAttributeToElement<int>(const std::string& att_name, const int& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), att_data);
        }

        template <>
        inline void AddAttributeToElement<double>(const std::string& att_name, const double& att_data)
        {
            _currElement->SetDoubleAttribute(att_name.c_str(), att_data);
        }

        template <>
        inline void AddAttributeToElement<float>(const std::string& att_name, const float& att_data)
        {
            _currElement->SetDoubleAttribute(att_name.c_str(), att_data);
        }

        /*template <>
        inline void AddAttributeToElement<std::string>(const std::string& att_name, const std::string& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), att_data.c_str());
        }*/

        template <>
        inline void AddAttributeToElement<const char*>(const std::string& att_name, const char* const& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), att_data);
        }

        template <>
        inline void AddAttributeToElement<char>(const std::string& att_name, const char& att_data)
        {
            std::string c{ att_data };
            _currElement->SetAttribute(att_name.c_str(), c.c_str());
        }

        template <>
        inline void AddAttributeToElement(const std::string& att_name, const EntityType& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), lexical_cast_toStr<EntityType>(const_cast<EntityType&>(att_data)).c_str());
        }

        template <>
        inline void AddAttributeToElement(const std::string& att_name, const CameraComponent::CameraType& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), lexical_cast_toStr<CameraComponent::CameraType>(const_cast<CameraComponent::CameraType&>(att_data)).c_str());
        }

        template <>
        inline void AddAttributeToElement(const std::string& att_name, const CameraComponent::ProjectionType& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), lexical_cast_toStr<CameraComponent::ProjectionType>(const_cast<CameraComponent::ProjectionType&>(att_data)).c_str());
        }

        template <>
        inline void AddAttributeToElement(const std::string& att_name, const MaterialModelType& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), lexical_cast_toStr<MaterialModelType>(const_cast<MaterialModelType&>(att_data)).c_str());
        }

        template <>
        inline void AddAttributeToElement(const std::string& att_name, const TextureType& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), lexical_cast_toStr<TextureType>(const_cast<TextureType&>(att_data)).c_str());
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

        template <typename T, size_t N>
        inline void AddAttributeToElement(const std::string& att_name, const Vector<T, N>& att_data)
        {
            std::string vecNames[4] = { {"x"}, {"y"}, {"z"}, {"w"} };
            for (size_t i = 0; i < N; ++i)
            {
                AddAttributeToElement<T>(vecNames[i], att_data[i]);
            }
        }

        template <typename T, size_t N1, size_t N2, glm::qualifier GLM>
        inline void AddAttributeToElement(const std::string& att_name, const glm::mat<N1, N2, T, GLM>& att_data)
        {
            //Col x Row
            /*2x3
                        Col1 Col2
            Row1
            Row2
            Row3
            */
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

        template <typename T, size_t N, glm::qualifier GLM>
        inline void AddAttributeToElement(const std::string& att_name, const glm::vec<N, T, GLM>& att_data)
        {
            std::string vecNames[4] = { {"x"}, {"y"}, {"z"}, {"w"} };
            for (size_t i = 0; i < N; ++i)
            {
                AddAttributeToElement<T>(vecNames[i], att_data[i]);
            }
        }
    };
}