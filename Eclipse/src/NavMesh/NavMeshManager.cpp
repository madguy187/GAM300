#include "pch.h"
#include "NavMeshManager.h"

namespace Eclipse
{
	void NavMeshManager::CreateRecastPolyMesh(const rcPolyMesh& mesh)
	{
		
	}
	void NavMeshManager::CalculateMinMax(Entity ent)
	{
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);
		auto& navmeshcomp = engine->world.GetComponent<NavMeshVolumeComponent>(ent);

		//navmeshcomp.

	}
	bool NavMeshManager::BuildNavMesh(Entity ent)
	{
		EDITOR_LOG_INFO("Building navmesh...");
		int nLoop = 0;
		float* rc_verts;
		unsigned int     rc_nverts;
		int* rc_tris;
		float* rc_trinorms;
		unsigned int     rc_ntris;
		float	           rc_bmin[3];
		float	           rc_bmax[3];

		ECVec3 VertA;
		ECVec3 VertB;
		ECVec3 VertC;
		ECVec3 TriNorm;
		int nVert = 0;

		rc_bmin[0] = INFINITY;
		rc_bmin[1] = INFINITY;
		rc_bmin[2] = INFINITY;

		rc_bmin[0] = INFINITY;
		rc_bmin[1] = INFINITY;
		rc_bmin[2] = INFINITY;
		return false;
	}
}