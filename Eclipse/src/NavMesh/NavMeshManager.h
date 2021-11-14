#pragma once
#include "Recast.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourDebugDraw.h"
#include "RecastDebugDraw.h"
#include "ECS/ComponentManager/Components/NavMeshVolumeComponent.h"

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
enum SamplePolyAreas
{
	SAMPLE_POLYAREA_GROUND,
	SAMPLE_POLYAREA_WATER,
	SAMPLE_POLYAREA_ROAD,
	SAMPLE_POLYAREA_DOOR,
	SAMPLE_POLYAREA_GRASS,
	SAMPLE_POLYAREA_JUMP,
};
enum SamplePolyFlags
{
	SAMPLE_POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
	SAMPLE_POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
	SAMPLE_POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
	SAMPLE_POLYFLAGS_JUMP = 0x08,		// Ability to jump.
	SAMPLE_POLYFLAGS_ALL = 0xffff		// All abilities.
};

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

		class InputGeom* m_geom;
		class dtNavMesh* m_navMesh;
		class dtNavMeshQuery* m_navQuery;
		unsigned char m_navMeshDrawFlags{0};

		rcContext* m_ctx{ nullptr };

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


	public:
		//void CreateRecastPolyMesh(const rcPolyMesh& mesh);
		void CalculateMinMax(Entity ent);
		void GetVertsandTrisOnScene(std::vector<ECVec3>& verts, std::vector<std::array<int,3>>& tris);
		bool BuildNavMesh(Entity ent);
		void RenderMesh();
		void RecastCleanup();
	};
}