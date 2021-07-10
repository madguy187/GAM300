#include "pch.h"
#include "SerializationManager.h"

namespace Eclipse
{
	SerializationManager::SerializationManager() {}

	void SerializationManager::SerializeAllEntity(const char* fullpath)
	{
		std::vector<Entity>& entities = engine->editorManager->EntityHierarchyList_;
		std::filesystem::path p(fullpath);
		p.filename().replace_extension("");

		sz.StartElement(p.string());
		sz.StartElement("Entities");
		sz.AddAttributeToElement("Size", entities.size());
		for (auto const& ent : entities)
		{

		}
		sz.CloseElement();
		sz.CloseElement();
	}

	SerializationManager::~SerializationManager()
	{
		SerializeAllEntity();
	}
}