#include "pch.h"
#include "SerializationManager.h"

SerializationManager::SerializationManager() {}

void SerializationManager::Update()
{

}

void SerializationManager::SerializeAllEntity(const char* fullpath)
{
	std::filesystem::path p(fullpath);
	p.filename().replace_extension("");

	sz.StartElement(p.string());
	sz.StartElement("Entities");
	
	for (auto const& ent : mEntities)
	{

	}
	sz.AddAttributeToElement("Size", mEntities.size());
	sz.CloseElement();
	sz.CloseElement();
}

SerializationManager::~SerializationManager() 
{
	SerializeAllEntity();
}