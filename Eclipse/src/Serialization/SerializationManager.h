#pragma once
#include "Serializer.h"

namespace Eclipse
{
	class SerializationManager
	{
		Serializer sz;

	public:
		SerializationManager();

		void SerializeAllEntity(const char* fullpath = "Data/Temp/temp.xml");

		~SerializationManager();
	};
}