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
	class Deserializer
	{
		TiXmlDocument _doc;
		TiXmlElement* _currElement;
		std::filesystem::path _path;
		bool hasFile;
		const char* EmptyStringReplacement = "__EMPTY__";

		bool CheckAttributeValue(const std::string& name);

		bool GetAttributeValue(const std::string& name, std::string& result);

		bool SetMonoScriptPointer(const std::string& scriptName, MonoScript*& att_data);

		void CleanMonoVariables(std::vector<MonoVariable>& origin, std::vector<MonoVariable>& saved);

	public:
		Deserializer();

		bool StartElement(const std::string& ele_name, bool isMultiple = false, size_t counter = 0);

		void CloseElement();

		template <typename T>
		inline void ReadAttributeFromElement(const std::string& att_name, T& att_data)
		{
			std::string str;
			if (GetAttributeValue(att_name, str))
			{
				att_data = lexical_cast<T>(str);
			}
		}

		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, std::string& att_data)
		{
			std::string str;

			if (GetAttributeValue(att_name, str))
			{
				att_data = str;
			}
		}

		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, EntityType& att_data)
		{
			std::string str;

			if (GetAttributeValue(att_name, str))
			{
				att_data = lexical_cast_toEnum<EntityType>(str);
			}
		}
		
		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, bool& att_data)
		{
			std::string str;

			if (GetAttributeValue(att_name, str))
			{
				att_data = lexical_cast<bool>(str);
			}
		}
		
		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, CameraComponent::CameraType& att_data)
		{
			std::string str;

			if (GetAttributeValue(att_name, str))
			{
				att_data = lexical_cast_toEnum<CameraComponent::CameraType>(str);
			}
		}
		
		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, CameraComponent::ProjectionType& att_data)
		{
			std::string str;

			if (GetAttributeValue(att_name, str))
			{
				att_data = lexical_cast_toEnum<CameraComponent::ProjectionType>(str);
			}
		}
		
		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, MaterialModelType& att_data)
		{
			std::string str;

			if (GetAttributeValue(att_name, str))
			{
				att_data = lexical_cast_toEnum<MaterialModelType>(str);
			}
		}

		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, TextureType& att_data)
		{
			std::string str;

			if (GetAttributeValue(att_name, str))
			{
				att_data = lexical_cast_toEnum<TextureType>(str);
			}
		}

		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, MeshComponent::MapType& att_data)
		{
			std::string str;

			if (GetAttributeValue(att_name, str))
			{
				att_data = lexical_cast_toEnum<MeshComponent::MapType>(str);
			}
		}
		
		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, ModelType& att_data)
		{
			std::string str;

			if (GetAttributeValue(att_name, str))
			{
				att_data = lexical_cast_toEnum<ModelType>(str);
			}
		}
		
		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, PxShapeType& att_data)
		{
			std::string str;

			if (GetAttributeValue(att_name, str))
			{
				att_data = lexical_cast_toEnum<PxShapeType>(str);
			}
		}
		
		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, m_Type& att_data)
		{
			std::string str;

			if (GetAttributeValue(att_name, str))
			{
				att_data = lexical_cast_toEnum<m_Type>(str);
			}
		}
		
		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, Texture& att_data)
		{
			(void)att_name;
			ReadAttributeFromElement("Type", att_data.Type);
			ReadAttributeFromElement("Directory", att_data.Directory);
			ReadAttributeFromElement("Handle", att_data.handle);
		}

		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, EC_Shape& att_data)
		{
			(void)att_name;
			ReadAttributeFromElement("Shape", att_data.shape);
			ReadAttributeFromElement("Hx", att_data.hx);
			ReadAttributeFromElement("Hy", att_data.hy);
			ReadAttributeFromElement("Hz", att_data.hz);
			ReadAttributeFromElement("Radius", att_data.radius);
			ReadAttributeFromElement("Hheight", att_data.hheight);
		}

		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, MonoScript*& att_data)
		{
			(void)att_name;
			std::string scriptName;
			ReadAttributeFromElement("ScriptName", scriptName);
			if (SetMonoScriptPointer(scriptName, att_data))
			{
				std::vector<MonoVariable> saved;
				StartElement("MonoVariables");
				ReadAttributeFromElement("Variables", saved);
				CloseElement();
				CleanMonoVariables(att_data->vars, saved);
			}
		}

		template <>
		inline void ReadAttributeFromElement(const std::string& att_name, MonoVariable& att_data)
		{
			(void)att_name;
			ReadAttributeFromElement("Type", att_data.type);
			ReadAttributeFromElement("Name", att_data.varName);
			ReadAttributeFromElement("Value", att_data.varValue);
		}
		
		template <typename T>
		inline void ReadAttributeFromElement(const std::string& att_name, std::vector<T>& att_data)
		{
			std::string str;
			GetAttributeValue("size", str);

			size_t size = lexical_cast<size_t>(str);
			std::string name{ "Member" };
			for (size_t i = 0; i < size; ++i)
			{
				StartElement(name, true, i);
				T data;
				ReadAttributeFromElement(att_name.c_str(), data);
				att_data.push_back(data);
				CloseElement();
			}
		}

		template <typename T, size_t N>
		inline void ReadAttributeFromElement(const std::string& att_name, Vector<T, N>& att_data)
		{
			(void)att_name;
			std::string vecNames[4] = { {"x"}, {"y"}, {"z"}, {"w"} };
			void (Vector<T, N>::*v[4])(T) = { &Vector<T, N>::setX, &Vector<T, N>::setY, &Vector<T, N>::setZ, &Vector<T, N>::setW };
			for (size_t i = 0; i < N; ++i)
			{
				std::string str;
				GetAttributeValue(vecNames[i], str);
				(att_data.*v[i])(lexical_cast<T>(str));
			}
		}

		template <typename T, size_t N1, size_t N2, glm::qualifier GLM>
		inline void ReadAttributeFromElement(const std::string& att_name, glm::mat<N1, N2, T, GLM>& att_data)
		{
			(void)att_name;
			std::string name{ "Col" };
			for (auto i = 0; i < N1; ++i)
			{
				std::string dataName{ "_" + std::to_string(i) };
				StartElement(name, true, i);
				for (auto j = 0; j < N2; ++j)
				{
					std::string str;
					GetAttributeValue(dataName + std::to_string(j), str);
					att_data[i][j] = lexical_cast<T>(str);
				}
				CloseElement();
			}
		}

		template <typename T, size_t N, glm::qualifier GLM>
		inline void ReadAttributeFromElement(const std::string& att_name, glm::vec<N, T, GLM>& att_data)
		{
			(void)att_name;
			std::string vecNames[4] = { {"x"}, {"y"}, {"z"}, {"w"} };
			for (auto i = 0; i < N; ++i)
			{
				std::string str;
				GetAttributeValue(vecNames[i], str);
				att_data[i] = lexical_cast<T>(str);
			}
		}

		template <typename T, size_t N>
		inline void ReadAttributeFromElement(const std::string& att_name, std::array<T, N>& att_data)
		{
			std::string str;
			GetAttributeValue("size", str);
			att_data.fill(0);
			size_t size = lexical_cast<size_t>(str);
			std::string name{ "Member" };
			for (size_t i = 0; i < size; ++i)
			{
				StartElement(name, true, i);
				T data = 0;
				ReadAttributeFromElement(att_name.c_str(), data);
				att_data[i] = data;
				CloseElement();
			}
		}

		template <size_t N>
		inline void ReadAttributeFromElement(const std::string& att_name, std::array<char, N>& att_data)
		{
			att_data.fill(0);
			std::string data;
			ReadAttributeFromElement(att_name.c_str(), data);
			strcpy(att_data.data(), data.c_str());
		}

		void IterateChildrenOfElement();

		bool LoadXML(const std::string& savePath);

		bool LoadBackup(TiXmlDocument& target, const std::string& path);

		~Deserializer();
	};
}