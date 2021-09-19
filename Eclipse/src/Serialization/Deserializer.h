#pragma once
#include "Global.h"
#include "TinyXML/tinyxml.h"
#include "Library/Strings/Lexical.h"
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

		bool CheckAttributeValue(const std::string& name);

		const char* GetAttributeValue(const std::string& name);

	public:
		Deserializer();

		bool StartElement(const std::string& ele_name, bool isMultiple = false, size_t counter = 0);

		void CloseElement();

		template <typename T>
		inline void ReadAttributeFromElement(const std::string& att_name, T& att_data)
		{
			att_data = lexical_cast<T>(GetAttributeValue(att_name));
		}

		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, std::string& att_data)
		{
			att_data = GetAttributeValue(att_name);
		}

		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, EntityType& att_data)
		{
			std::string str = GetAttributeValue(att_name);
			att_data = lexical_cast_toEnum<EntityType>(str);
		}
		
		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, bool& att_data)
		{
			std::string str = GetAttributeValue(att_name);
			att_data = lexical_cast<bool>(str);
		}
		
		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, CameraComponent::CameraType& att_data)
		{
			std::string str = GetAttributeValue(att_name);
			att_data = lexical_cast_toEnum<CameraComponent::CameraType>(str);
		}
		
		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, CameraComponent::ProjectionType& att_data)
		{
			std::string str = GetAttributeValue(att_name);
			att_data = lexical_cast_toEnum<CameraComponent::ProjectionType>(str);
		}
		
		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, MaterialComponent::ModelType& att_data)
		{
			std::string str = GetAttributeValue(att_name);
			att_data = lexical_cast_toEnum<MaterialComponent::ModelType>(str);
		}

		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, TextureType& att_data)
		{
			std::string str = GetAttributeValue(att_name);
			att_data = lexical_cast_toEnum<TextureType>(str);
		}
		
		template <typename T, size_t N>
		inline void ReadAttributeFromElement(const std::string& att_name, Vector<T, N>& att_data)
		{
			std::string vecNames[4] = { {"x"}, {"y"}, {"z"}, {"w"} };
			void (Vector<T, N>::*v[4])(T) = { &Vector<T, N>::setX, &Vector<T, N>::setY, &Vector<T, N>::setZ, &Vector<T, N>::setW };
			for (size_t i = 0; i < N; ++i)
			{
				std::string str = GetAttributeValue(vecNames[i]);
				(att_data.*v[i])(lexical_cast<T>(str));
			}
		}

		template <typename T, size_t N1, size_t N2, glm::qualifier GLM>
		inline void ReadAttributeFromElement(const std::string& att_name, glm::mat<N1, N2, T, GLM>& att_data)
		{
			std::string name{ "Col" };
			for (size_t i = 0; i < N1; ++i)
			{
				std::string dataName{ "_" + std::to_string(i) };
				StartElement(name, true, i);
				for (size_t j = 0; j < N2; ++j)
				{
					std::string str = GetAttributeValue(dataName + std::to_string(j));
					att_data[i][j] = lexical_cast<T>(str);
				}
				CloseElement();
			}

		}

		template <typename T, size_t N, glm::qualifier GLM>
		inline void ReadAttributeFromElement(const std::string& att_name, glm::vec<N, T, GLM>& att_data)
		{
			std::string vecNames[4] = { {"x"}, {"y"}, {"z"}, {"w"} };
			for (size_t i = 0; i < N; ++i)
			{
				std::string str = GetAttributeValue(vecNames[i]);
				att_data[i] = lexical_cast<T>(str);
			}
		}

		void IterateChildrenOfElement();

		bool LoadXML(const std::string& savePath);

		~Deserializer();
	};
}