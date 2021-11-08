#pragma once
#include "TinyXML/tinyxml.h"
#include "Library/Strings/Lexical.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <type_traits>
#include "ECS/ComponentManager/ComponentList.h"


namespace Eclipse
{
    class Serializer
    {
        TiXmlDocument _doc;
        TiXmlElement* _currElement;
        const char* EmptyStringReplacement = "__EMPTY__";

        void Init();

        void GenerateDirectories(const std::string& path);

        void BaseSave(const std::string& savePath);

        void CleanUp();
    public:
        Serializer();

        void StartElement(const std::string& ele_name, bool isMultiple = false, size_t counter = 0);

        void CloseElement();

        void SaveXML(const std::string& savePath);

        void SaveBackup(TiXmlDocument& backup, const std::string& path);

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

        template <>
        inline void AddAttributeToElement<std::string>(const std::string& att_name, const std::string& att_data)
        {/*
            if(!att_data.empty())
                _currElement->SetAttribute(att_name.c_str(), EmptyStringReplacement);
            else*/
            _currElement->SetAttribute(att_name.c_str(), att_data.c_str());
        }

        template <>
        inline void AddAttributeToElement<const char*>(const std::string& att_name, const char* const& att_data)
        {/*
            if(!att_data)
                _currElement->SetAttribute(att_name.c_str(), EmptyStringReplacement);
            else*/
            _currElement->SetAttribute(att_name.c_str(), att_data);
        }

        template <>
        inline void AddAttributeToElement<char>(const std::string& att_name, const char& att_data)
        {
            std::string c{ att_data };

            //if(!c.empty())
            //    _currElement->SetAttribute(att_name.c_str(), EmptyStringReplacement);
            //else
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
        
        template <>
        inline void AddAttributeToElement(const std::string& att_name, const MeshComponent::MapType& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), lexical_cast_toStr<MeshComponent::MapType>(const_cast<MeshComponent::MapType&>(att_data)).c_str());
        }
        
        template <>
        inline void AddAttributeToElement(const std::string& att_name, const ModelType& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), lexical_cast_toStr<ModelType>(const_cast<ModelType&>(att_data)).c_str());
        }
        
        template <>
        inline void AddAttributeToElement(const std::string& att_name, const PxShapeType& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), lexical_cast_toStr<PxShapeType>(const_cast<PxShapeType&>(att_data)).c_str());
        }
        
        template <>
        inline void AddAttributeToElement(const std::string& att_name, const m_Type& att_data)
        {
            _currElement->SetAttribute(att_name.c_str(), lexical_cast_toStr<m_Type>(const_cast<m_Type&>(att_data)).c_str());
        }
        
        template <>
        inline void AddAttributeToElement(const std::string& att_name, const Texture& att_data)
        {
            (void)att_name;
            AddAttributeToElement("Type", att_data.Type);
            AddAttributeToElement("Directory", att_data.Directory);
            AddAttributeToElement("Handle", att_data.handle);
        }

        template <>
        inline void AddAttributeToElement(const std::string& att_name, const EC_Shape& att_data)
        {
            (void)att_name;
            AddAttributeToElement("Shape", att_data.shape);
            AddAttributeToElement("Hx", att_data.hx);
            AddAttributeToElement("Hy", att_data.hy);
            AddAttributeToElement("Hz", att_data.hz);
            AddAttributeToElement("Radius", att_data.radius);
            AddAttributeToElement("Hheight", att_data.hheight);
        }
        
        template <>
        inline void AddAttributeToElement(const std::string& att_name, const MonoScript& att_data)
        {
            (void)att_name;
            AddAttributeToElement("ScriptName", att_data.scriptName);
            StartElement("MonoVariables");
            AddAttributeToElement("Variables", att_data.vars);
            CloseElement();
        }
        
        template <>
        inline void AddAttributeToElement(const std::string& att_name, const MonoVariable& att_data)
        {
            (void)att_name;
            AddAttributeToElement("Type", att_data.type);
            AddAttributeToElement("Name", att_data.varName);
            AddAttributeToElement("Value", att_data.varValue);
        }

		template <typename T>
		inline void AddAttributeToElement(const std::string& att_name, const std::vector<T>& att_data)
		{
            (void)att_name;
			AddAttributeToElement("size", att_data.size());
			size_t counter = 0;
			std::string name{"Member"};
			for (const T& data : att_data)
			{
				StartElement(name, true, counter++);
				AddAttributeToElement<T>("value", data);
				CloseElement();
			}
		}
        
        template <typename T, size_t N>
        inline void AddAttributeToElement(const std::string& att_name, const Vector<T, N>& att_data)
        {
            (void)att_name;
            std::string vecNames[4] = { {"x"}, {"y"}, {"z"}, {"w"} };
            for (size_t i = 0; i < N; ++i)
            {
                AddAttributeToElement<T>(vecNames[i], att_data[i]);
            }
        }

        template <typename T, size_t N1, size_t N2, glm::qualifier GLM>
        inline void AddAttributeToElement(const std::string& att_name, const glm::mat<N1, N2, T, GLM>& att_data)
        {
            (void)att_name;
            AddAttributeToElement<size_t>("Col", N1);
            AddAttributeToElement<size_t>("Row", N2);
            std::string name{ "Col" };
            for (auto i = 0; i < N1; ++i)
            {
                std::string dataName{ "_" + std::to_string(i) };
                StartElement(name, true, i);
                for (auto j = 0; j < N2; ++j)
                {
                    AddAttributeToElement<T>(dataName + std::to_string(j), att_data[i][j]);
                }
                CloseElement();
            }
        }

        template <typename T, size_t N, glm::qualifier GLM>
        inline void AddAttributeToElement(const std::string& att_name, const glm::vec<N, T, GLM>& att_data)
        {
            (void)att_name;
            std::string vecNames[4] = { {"x"}, {"y"}, {"z"}, {"w"} };
            for (auto i = 0; i < N; ++i)
            {
                AddAttributeToElement<T>(vecNames[i], att_data[i]);
            }
        }

        template <typename T, size_t N>
        inline void AddAttributeToElement(const std::string& att_name, const std::array<T, N>& att_data)
        {
            AddAttributeToElement("size", N);
            size_t counter = 0;
            std::string name{ "Member" };
            for (const T& data : att_data)
            {
                StartElement(name, true, counter++);
                AddAttributeToElement<T>("value", data);
                CloseElement();
            }
        }

        template <size_t N>
        inline void AddAttributeToElement(const std::string& att_name, const std::array<char, N>& att_data)
        {
            (void)att_name;
            std::string data = att_data.data();
            AddAttributeToElement("value", data);
        }
    };
}