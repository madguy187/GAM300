#pragma once
#include "Recast.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourDebugDraw.h"
#include "RecastDebugDraw.h"
#include "ECS/ComponentManager/Components/NavMeshVolumeComponent.h"
#include "Source/Sample.h"
#include "Source/InputGeom.h"
#include "Source/SampleInterfaces.h"
#define MAX_PATHSLOT		128 // how many paths we can store
#define MAX_PATHPOLY		256 // max number of polygons in a path
#define MAX_PATHVERT		512 // most verts in a path 

// structure for storing output straight line paths
typedef struct
{
	float PosX[MAX_PATHVERT]{0.0f};
	float PosY[MAX_PATHVERT]{0.0f};
	float PosZ[MAX_PATHVERT]{0.0f};
	int MaxVertex{0};
	int Target{0};
}
PATHDATA;

// These are just sample areas to use consistent values across the samples.
// The use should specify these base on his needs.

// bzn most aren't used yet, just SAMPLE_POLYAREA_GROUND and SAMPLE_POLYFLAGS_WALK

namespace Eclipse
{
	class NavMeshManager
	{
	private:
		unsigned char* m_triareas{nullptr};
		rcHeightfield* m_solid{ nullptr };
		rcCompactHeightfield* m_chf{ nullptr };
		rcContourSet* m_cset{ nullptr };
		rcPolyMesh* m_pmesh{ nullptr };
		rcConfig m_cfg;
		rcPolyMeshDetail* m_dmesh{ nullptr };

		class dtNavMesh* m_navMesh;
		class dtNavMeshQuery* m_navQuery;
		unsigned char m_navMeshDrawFlags{0};

		rcContext* m_ctx{ nullptr };
		SampleDebugDraw m_dd;
		float m_cellSize{0.0f};
		float m_cellHeight{ 0.0f };
		float m_agentHeight{ 0.0f };
		float m_agentRadius{ 0.0f };
		float m_agentMaxClimb{ 0.0f };
		float m_agentMaxSlope{ 0.0f };
		float m_regionMinSize{ 0.0f };
		float m_regionMergeSize{ 0.0f };
		float m_edgeMaxLen{ 0.0f };
		float m_edgeMaxError{ 0.0f };
		float m_vertsPerPoly{ 0.0f };
		float m_detailSampleDist{ 0.0f };
		float m_detailSampleMaxError{ 0.0f };
		bool m_keepInterResults{ false };

		// Off-Mesh connections.  Not used yet.
		static const int MAX_OFFMESH_CONNECTIONS = 256;
		float m_offMeshConVerts[MAX_OFFMESH_CONNECTIONS * 3 * 2];
		float m_offMeshConRads[MAX_OFFMESH_CONNECTIONS];
		unsigned char m_offMeshConDirs[MAX_OFFMESH_CONNECTIONS];
		unsigned char m_offMeshConAreas[MAX_OFFMESH_CONNECTIONS];
		unsigned short m_offMeshConFlags[MAX_OFFMESH_CONNECTIONS];
		unsigned int m_offMeshConId[MAX_OFFMESH_CONNECTIONS];
		int m_offMeshConCount{0};

		PATHDATA m_PathStore[MAX_PATHSLOT];
	public:
		//void CreateRecastPolyMesh(const rcPolyMesh& mesh);
		void CalculateMinMax(Entity ent);
		void GetVertsandTrisOnScene(std::vector<ECVec3>& verts, std::vector<std::array<int,3>>& tris);
		bool BuildNavMesh(Entity ent);
		void RenderMesh();
		void RecastCleanup();
		int FindPath(float* StartPos, float* EndPos, int pathslot, int target);
	};
}