#pragma once
#include "Recast.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "ECS/ComponentManager/Components/NavMeshVolumeComponent.h"
namespace Eclipse
{
	class NavMeshManager
	{
	private:
		void CreateRecastPolyMesh(const rcPolyMesh& mesh);
		void CalculateMinMax(Entity ent);
		bool BuildNavMesh(Entity ent);
	};
}