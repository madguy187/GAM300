#pragma once
#include "Serializer.h"

class SerializationManager : public System
{
	Serializer sz;

public:
	SerializationManager();

	void Update() override;

	void SerializeAllEntity(const char* fullpath = "Data/Temp/temp.xml");

	~SerializationManager();
};