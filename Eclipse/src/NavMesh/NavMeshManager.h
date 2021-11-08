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
	float PosX[MAX_PATHVERT];
	float PosY[MAX_PATHVERT];
	float PosZ[MAX_PATHVERT];
	int MaxVertex;
	int Target;
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
		unsigned char* m_triareas;
		rcHeightfield* m_solid;
		rcCompactHeightfield* m_chf;
		rcContourSet* m_cset;
		rcPolyMesh* m_pmesh;
		rcConfig m_cfg;
		rcPolyMeshDetail* m_dmesh;

		class InputGeom* m_geom;
		class dtNavMesh* m_navMesh;
		class dtNavMeshQuery* m_navQuery;
		unsigned char m_navMeshDrawFlags;

		rcContext* m_ctx;

		float m_cellSize;
		float m_cellHeight;
		float m_agentHeight;
		float m_agentRadius;
		float m_agentMaxClimb;
		float m_agentMaxSlope;
		float m_regionMinSize;
		float m_regionMergeSize;
		float m_edgeMaxLen;
		float m_edgeMaxError;
		float m_vertsPerPoly;
		float m_detailSampleDist;
		float m_detailSampleMaxError;
		bool m_keepInterResults;

		// Off-Mesh connections.  Not used yet.
		static const int MAX_OFFMESH_CONNECTIONS = 256;
		float m_offMeshConVerts[MAX_OFFMESH_CONNECTIONS * 3 * 2];
		float m_offMeshConRads[MAX_OFFMESH_CONNECTIONS];
		unsigned char m_offMeshConDirs[MAX_OFFMESH_CONNECTIONS];
		unsigned char m_offMeshConAreas[MAX_OFFMESH_CONNECTIONS];
		unsigned short m_offMeshConFlags[MAX_OFFMESH_CONNECTIONS];
		unsigned int m_offMeshConId[MAX_OFFMESH_CONNECTIONS];
		int m_offMeshConCount;


	public:
		void CreateRecastPolyMesh(const rcPolyMesh& mesh);
		void CalculateMinMax(Entity ent);
		void GetVertsandTrisOnScene(std::vector<ECVec3>& verts, std::vector<std::array<int,3>>& tris);
		bool BuildNavMesh(Entity ent);
		void RenderMesh();
		void RecastCleanup();
	};
}