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

		template <typename T>
		void AddAttributeToElement(const std::string& att_name, const std::vector<T>& att_data)
		{
			StartElement(att_name);
			size_t counter = 0;
			std::string name{att_name + " member"};
			for (const T& data : att_data)
			{
				StartElement(name, true, counter++);
				AddAttributeToElement<T>("member", data);
				CloseElement();
			}
			CloseElement();
		}


		template <typename T, size_t N>
		void AddAttributeToElement(const std::string& att_name, const Vector<T, N>& att_data)
		{
			StartElement(att_name);
			std::string name{ att_name + "Member" };
			for (size_t i = 0; i < N; ++i)
			{
				StartElement(name, true, i);
				AddAttributeToElement<T>("Data", att_data[i]);
				CloseElement();
			}
			CloseElement();
		}

	};
}