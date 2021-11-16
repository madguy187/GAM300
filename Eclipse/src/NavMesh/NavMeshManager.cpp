#include "pch.h"
#include "NavMeshManager.h"

namespace Eclipse
{
	/*void NavMeshManager::CreateRecastPolyMesh(const rcPolyMesh& mesh)
	{

	}*/
	void NavMeshManager::CalculateMinMax(Entity ent)
	{
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);
		auto& navmeshcomp = engine->world.GetComponent<NavMeshVolumeComponent>(ent);

		navmeshcomp.NavMeshMin = transform.position - (transform.scale / 2);
		navmeshcomp.NavMeshMax = transform.position + (transform.scale / 2);

	}

	void NavMeshManager::GetVertsandTrisOnScene(std::vector<ECVec3>& verts, std::vector<std::array<int, 3>>& tris)
	{
		int Tricount = 0;
		for (auto ent : engine->editorManager->GetEntityListByConstRef())
		{
			if (!engine->world.CheckComponent<CollisionComponent>(ent) || engine->world.CheckComponent<NavMeshVolumeComponent>(ent))
				continue;

			auto& col = engine->world.GetComponent<CollisionComponent>(ent);
			if (col.shape.shape != PxShapeType::Px_CUBE)
				continue;
			auto& transform = engine->world.GetComponent<TransformComponent>(ent);
			ECVec3 tempverts[8]{ transform.position };


			tempverts[0].setX(transform.position.getX() - col.shape.hx);
			tempverts[0].setY(transform.position.getY() - col.shape.hy);
			tempverts[0].setZ(transform.position.getZ() - col.shape.hz);

			tempverts[1].setX(transform.position.getX() + col.shape.hx);
			tempverts[1].setY(transform.position.getY() + col.shape.hy);
			tempverts[1].setZ(transform.position.getZ() - col.shape.hz);

			tempverts[2].setX(transform.position.getX() - col.shape.hx);
			tempverts[2].setY(transform.position.getY() + col.shape.hy);
			tempverts[2].setZ(transform.position.getZ() - col.shape.hz);

			tempverts[3].setX(transform.position.getX() + col.shape.hx);
			tempverts[3].setY(transform.position.getY() - col.shape.hy);
			tempverts[3].setZ(transform.position.getZ() - col.shape.hz);

			tempverts[4].setX(transform.position.getX() - col.shape.hx);
			tempverts[4].setY(transform.position.getY() + col.shape.hy);
			tempverts[4].setZ(transform.position.getZ() + col.shape.hz);

			tempverts[5].setX(transform.position.getX() - col.shape.hx);
			tempverts[5].setY(transform.position.getY() - col.shape.hy);
			tempverts[5].setZ(transform.position.getZ() + col.shape.hz);

			tempverts[6].setX(transform.position.getX() + col.shape.hx);
			tempverts[6].setY(transform.position.getY() + col.shape.hy);
			tempverts[6].setZ(transform.position.getZ() + col.shape.hz);

			tempverts[7].setX(transform.position.getX() + col.shape.hx);
			tempverts[7].setY(transform.position.getY() - col.shape.hy);
			tempverts[7].setZ(transform.position.getZ() + col.shape.hz);

			for (size_t i = 0; i < 8; i++)
			{
				verts.push_back(tempverts[i]);
			}
			if (Tricount > 0)
			{
				//front
				tris.push_back({ Tricount + 1,Tricount + 2,Tricount + 3 });
				tris.push_back({ Tricount + 2,Tricount + 1,Tricount + 4 });
				//back
				tris.push_back({ Tricount + 8,Tricount + 5,Tricount + 7 });
				tris.push_back({ Tricount + 5,Tricount + 8,Tricount + 6 });
				//left
				tris.push_back({ Tricount + 6 ,Tricount + 3,Tricount + 5 });
				tris.push_back({ Tricount + 3 ,Tricount + 6,Tricount + 1 });
				//right
				tris.push_back({ Tricount + 4,Tricount + 7,Tricount + 2 });
				tris.push_back({ Tricount + 7,Tricount + 4,Tricount + 8 });
				//up
				tris.push_back({ Tricount + 3,Tricount + 7,Tricount + 5 });
				tris.push_back({ Tricount + 7,Tricount + 3,Tricount + 2 });
				//down
				tris.push_back({ Tricount + 4,Tricount + 6,Tricount + 8 });
				tris.push_back({ Tricount + 6,Tricount + 4,Tricount + 1 });
			}
			else
			{
				//front
				tris.push_back({ Tricount ,Tricount + 1,Tricount + 2 });
				tris.push_back({ Tricount + 1,Tricount ,Tricount + 3 });
				//back
				tris.push_back({ Tricount + 7,Tricount + 4,Tricount + 6 });
				tris.push_back({ Tricount + 4,Tricount + 7,Tricount + 5 });
				//left
				tris.push_back({ Tricount + 5 ,Tricount + 2,Tricount + 4 });
				tris.push_back({ Tricount + 2 ,Tricount + 5,Tricount });
				//right
				tris.push_back({ Tricount + 3,Tricount + 6,Tricount + 1 });
				tris.push_back({ Tricount + 6,Tricount + 3,Tricount + 7 });
				//up
				tris.push_back({ Tricount + 2,Tricount + 6,Tricount + 4 });
				tris.push_back({ Tricount + 6,Tricount + 2,Tricount + 1 });
				//down
				tris.push_back({ Tricount + 3,Tricount + 5,Tricount + 7 });
				tris.push_back({ Tricount + 5,Tricount + 3,Tricount });
			}

			if (Tricount == 0)
				Tricount += 7;
			else
				Tricount += 8;
		}
	}

	bool NavMeshManager::BuildNavMesh(Entity ent)
	{
		RecastCleanup();

		auto& navmeshcomp = engine->world.GetComponent<NavMeshVolumeComponent>(ent);
		EDITOR_LOG_INFO("Building navmesh...");
		unsigned int nLoop = 0;
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

		CalculateMinMax(ent);

		rc_bmin[0] = navmeshcomp.NavMeshMin.getX();
		rc_bmin[1] = navmeshcomp.NavMeshMin.getY();
		rc_bmin[2] = navmeshcomp.NavMeshMin.getZ();

		rc_bmax[0] = navmeshcomp.NavMeshMax.getX();
		rc_bmax[1] = navmeshcomp.NavMeshMax.getY();
		rc_bmax[2] = navmeshcomp.NavMeshMax.getZ();

		std::vector<ECVec3> verts;
		std::vector<std::array<int, 3>> tris;

		GetVertsandTrisOnScene(verts, tris);

		rc_nverts = static_cast<unsigned int>(verts.size());
		rc_verts = new float[static_cast<size_t>(rc_nverts) * 3];
		for (nLoop = 0; nLoop < rc_nverts; nLoop++)
		{
			rc_verts[nLoop * 3 + 0] = verts[nLoop].getX();
			rc_verts[nLoop * 3 + 1] = verts[nLoop].getY();
			rc_verts[nLoop * 3 + 2] = verts[nLoop].getZ();
		}

		//for (nLoop = 0; nLoop < rc_nverts * 3; nLoop++)
		//{
		//	EDITOR_LOG_INFO(std::to_string(rc_verts[nLoop]).c_str());
		//}


		rc_ntris = static_cast<unsigned int>(tris.size());
		rc_tris = new int[static_cast<size_t>(rc_ntris) * 3];
		rc_trinorms = new float[static_cast<size_t>(rc_ntris) * 3];
		int nTriCount = 0;
		for (nLoop = 0; nLoop < rc_ntris; nLoop++)
		{
			rc_tris[nTriCount * 3 + 0] = tris[nLoop][0];
			rc_tris[nTriCount * 3 + 1] = tris[nLoop][2];
			rc_tris[nTriCount * 3 + 2] = tris[nLoop][1];

			nVert = tris[nLoop][0];
			VertA.setX(verts[nVert].getX());
			VertA.setY(verts[nVert].getY());
			VertA.setZ(verts[nVert].getZ());

			nVert = tris[nLoop][1];
			VertB.setX(verts[nVert].getX());
			VertB.setY(verts[nVert].getY());
			VertB.setZ(verts[nVert].getZ());

			nVert = tris[nLoop][2];
			VertC.setX(verts[nVert].getX());
			VertC.setY(verts[nVert].getY());
			VertC.setZ(verts[nVert].getZ());

			VertA -= VertC;
			VertB -= VertC;

			ECVec3 normal;
			normal.setX((VertA.getY() * VertB.getZ()) - (VertA.getZ() * VertB.getY()));
			normal.setY((VertA.getZ() * VertB.getX()) - (VertA.getX() * VertB.getZ()));
			normal.setZ((VertA.getX() * VertB.getY()) - (VertA.getY() * VertB.getX()));

			VectorNormalize<float, 3>(VertA, normal);

			rc_trinorms[nTriCount * 3 + 0] = VertA.getX();
			rc_trinorms[nTriCount * 3 + 1] = VertA.getY();
			rc_trinorms[nTriCount * 3 + 2] = VertA.getZ();

			nTriCount++;
		}


		for (nLoop = 0; nLoop < rc_ntris * 3; nLoop++)
		{
			//EDITOR_LOG_INFO(std::to_string(rc_tris[nLoop]).c_str());
			EDITOR_LOG_INFO(std::to_string(rc_trinorms[nLoop]).c_str());
		}




		m_ctx = new rcContext(true);
		m_cellSize = 0.3f;
		m_cellHeight = 0.2f;
		m_agentMaxSlope = navmeshcomp.MaxSlope;
		m_agentHeight = navmeshcomp.AgentHeight;
		m_agentMaxClimb = navmeshcomp.JumpDistance;
		m_agentRadius = navmeshcomp.AgentRadius;
		m_edgeMaxLen = 512;
		m_edgeMaxError = 1.3f;
		m_regionMinSize = 50;
		m_regionMergeSize = 20;
		m_vertsPerPoly = 6;
		m_detailSampleDist = 6;
		m_detailSampleMaxError = 1;
		m_keepInterResults = false;

		//
	// Step 1. Initialize build config.
	//

	// Init build configuration from GUI
		memset(&m_cfg, 0, sizeof(m_cfg));
		m_cfg.cs = m_cellSize;
		m_cfg.ch = m_cellHeight;
		m_cfg.walkableSlopeAngle = m_agentMaxSlope;
		m_cfg.walkableHeight = (int)ceilf(m_agentHeight / m_cfg.ch);
		m_cfg.walkableClimb = (int)floorf(m_agentMaxClimb / m_cfg.ch);
		m_cfg.walkableRadius = (int)ceilf(m_agentRadius / m_cfg.cs);
		m_cfg.maxEdgeLen = (int)(m_edgeMaxLen / m_cellSize);
		m_cfg.maxSimplificationError = m_edgeMaxError;
		m_cfg.minRegionArea = (int)rcSqr(m_regionMinSize);		// Note: area = size*size
		m_cfg.mergeRegionArea = (int)rcSqr(m_regionMergeSize);	// Note: area = size*size
		m_cfg.maxVertsPerPoly = (int)m_vertsPerPoly;
		m_cfg.detailSampleDist = m_detailSampleDist < 0.9f ? 0 : m_cellSize * m_detailSampleDist;
		m_cfg.detailSampleMaxError = m_cellHeight * m_detailSampleMaxError;

		// Set the area where the navigation will be build.
	// Here the bounds of the input mesh are used, but the
	// area could be specified by an user defined box, etc.
		rcVcopy(m_cfg.bmin, rc_bmin);
		rcVcopy(m_cfg.bmax, rc_bmax);
		rcCalcGridSize(m_cfg.bmin, m_cfg.bmax, m_cfg.cs, &m_cfg.width, &m_cfg.height);

		// Reset build times gathering.
		m_ctx->resetTimers();

		// Start the build process.	
		m_ctx->startTimer(RC_TIMER_TOTAL);

		m_ctx->log(RC_LOG_PROGRESS, "Building navigation:");
		m_ctx->log(RC_LOG_PROGRESS, " - %d x %d cells", m_cfg.width, m_cfg.height);
		m_ctx->log(RC_LOG_PROGRESS, " - %.1fK verts, %.1fK tris", rc_nverts / 1000.0f, rc_ntris / 1000.0f);

		EDITOR_LOG_INFO("Building navigation:");
		EDITOR_LOG_INFO(" - %d x %d cells", m_cfg.width, m_cfg.height);
		EDITOR_LOG_INFO(" - %.1fK verts, %.1fK tris", rc_nverts / 1000.0f, rc_ntris / 1000.0f);

		//
	// Step 2. Rasterize input polygon soup.
	//

	// Allocate voxel heightfield where we rasterize our input data to.
		m_solid = rcAllocHeightfield();
		if (!m_solid)
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
			EDITOR_LOG_WARN("buildNavigation: Out of memory 'solid'.");
			return false;
		}
		if (!rcCreateHeightfield(m_ctx, *m_solid, m_cfg.width, m_cfg.height, m_cfg.bmin, m_cfg.bmax, m_cfg.cs, m_cfg.ch))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
			EDITOR_LOG_WARN("buildNavigation: Could not create solid heightfield.");
			return false;
		}

		// Allocate array that can hold triangle area types.
		// If you have multiple meshes you need to process, allocate
		// and array which can hold the max number of triangles you need to process.
		m_triareas = new unsigned char[rc_ntris];
		if (!m_triareas)
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'm_triareas' (%d).", rc_ntris);
			EDITOR_LOG_WARN("buildNavigation: Out of memory 'm_triareas' (%d).", rc_ntris);
			return false;
		}

		// Find triangles which are walkable based on their slope and rasterize them.
		// If your input data is multiple meshes, you can transform them here, calculate
		// the are type for each of the meshes and rasterize them.
		memset(m_triareas, 0, rc_ntris * sizeof(unsigned char));
		rcMarkWalkableTriangles(m_ctx, m_cfg.walkableSlopeAngle, rc_verts, rc_nverts, rc_tris, rc_ntris, m_triareas);
		if (!rcRasterizeTriangles(m_ctx, rc_verts, rc_nverts, rc_tris, m_triareas, rc_ntris, *m_solid, m_cfg.walkableClimb))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not rasterize triangles.");
			EDITOR_LOG_WARN("buildNavigation: Could not rasterize triangles.");
			return false;
		}

		if (!m_keepInterResults)
		{
			delete[] m_triareas;
			m_triareas = 0;
		}

		//
	// Step 3. Filter walkables surfaces.
	//

	// Once all geoemtry is rasterized, we do initial pass of filtering to
	// remove unwanted overhangs caused by the conservative rasterization
	// as well as filter spans where the character cannot possibly stand.
		rcFilterLowHangingWalkableObstacles(m_ctx, m_cfg.walkableClimb, *m_solid);
		rcFilterLedgeSpans(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid);
		rcFilterWalkableLowHeightSpans(m_ctx, m_cfg.walkableHeight, *m_solid);

		//
		// Step 4. Partition walkable surface to simple regions.
		//

		// Compact the heightfield so that it is faster to handle from now on.
		// This will result more cache coherent data as well as the neighbours
		// between walkable cells will be calculated.
		m_chf = rcAllocCompactHeightfield();
		if (!m_chf)
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
			EDITOR_LOG_WARN("buildNavigation: Out of memory 'chf'.");
			return false;
		}
		if (!rcBuildCompactHeightfield(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid, *m_chf))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
			EDITOR_LOG_WARN("buildNavigation: Could not build compact data.");
			return false;
		}

		if (!m_keepInterResults)
		{
			rcFreeHeightField(m_solid);
			m_solid = 0;
		}

		// Erode the walkable area by agent radius.
		if (!rcErodeWalkableArea(m_ctx, m_cfg.walkableRadius, *m_chf))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
			EDITOR_LOG_WARN("buildNavigation: Could not erode.");
			return false;
		}

		//// (Optional) Mark areas.
		//const ConvexVolume* vols = m_geom->getConvexVolumes();
		//for (int i = 0; i < m_geom->getConvexVolumeCount(); ++i)
		//	rcMarkConvexPolyArea(m_ctx, vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, *m_chf);

		// Prepare for region partitioning, by calculating distance field along the walkable surface.
		if (!rcBuildDistanceField(m_ctx, *m_chf))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build distance field.");
			EDITOR_LOG_WARN("buildNavigation: Could not build distance field.");
			return false;
		}

		// Partition the walkable surface into simple regions without holes.
		if (!rcBuildRegions(m_ctx, *m_chf, m_cfg.borderSize, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build regions.");
			EDITOR_LOG_WARN("buildNavigation: Could not build regions.");
			return false;
		}

		//
		// Step 5. Trace and simplify region contours.
		//

		// Create contours.
		m_cset = rcAllocContourSet();
		if (!m_cset)
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'cset'.");
			EDITOR_LOG_WARN("buildNavigation: Out of memory 'cset'.");
			return false;
		}
		if (!rcBuildContours(m_ctx, *m_chf, m_cfg.maxSimplificationError, m_cfg.maxEdgeLen, *m_cset))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create contours.");
			EDITOR_LOG_WARN("buildNavigation: Could not create contours.");
			return false;
		}

		//
		// Step 6. Build polygons mesh from contours.
		//

		// Build polygon navmesh from the contours.
		m_pmesh = rcAllocPolyMesh();
		if (!m_pmesh)
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmesh'.");
			EDITOR_LOG_WARN("buildNavigation: Out of memory 'pmesh'.");
			return false;
		}
		if (!rcBuildPolyMesh(m_ctx, *m_cset, m_cfg.maxVertsPerPoly, *m_pmesh))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not triangulate contours.");
			EDITOR_LOG_WARN("buildNavigation: Could not triangulate contours.");
			return false;
		}

		//
		// Step 7. Create detail mesh which allows to access approximate height on each polygon.
		//

		m_dmesh = rcAllocPolyMeshDetail();
		if (!m_dmesh)
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmdtl'.");
			EDITOR_LOG_WARN("buildNavigation: Out of memory 'pmdtl'.");
			return false;
		}

		if (!rcBuildPolyMeshDetail(m_ctx, *m_pmesh, *m_chf, m_cfg.detailSampleDist, m_cfg.detailSampleMaxError, *m_dmesh))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build detail mesh.");
			EDITOR_LOG_WARN("buildNavigation: Could not build detail mesh.");
			return false;
		}

		if (!m_keepInterResults)
		{
			rcFreeCompactHeightfield(m_chf);
			m_chf = 0;
			rcFreeContourSet(m_cset);
			m_cset = 0;
		}

		// At this point the navigation mesh data is ready, you can access it from m_pmesh.
	// See duDebugDrawPolyMesh or dtCreateNavMeshData as examples how to access the data.

	//
	// (Optional) Step 8. Create Detour data from Recast poly mesh.
	//

	// The GUI may allow more max points per polygon than Detour can handle.
	// Only build the detour navmesh if we do not exceed the limit.


		if (m_cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
		{
			EDITOR_LOG_INFO("Detour 1000");

			unsigned char* navData = 0;
			int navDataSize = 0;


			// Update poly flags from areas.
			for (int i = 0; i < m_pmesh->npolys; ++i)
			{
				if (m_pmesh->areas[i] == RC_WALKABLE_AREA)
				{
					m_pmesh->areas[i] = SAMPLE_POLYAREA_GROUND;
					m_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
				}
			}


			dtNavMeshCreateParams params;
			memset(&params, 0, sizeof(params));
			params.verts = m_pmesh->verts;
			params.vertCount = m_pmesh->nverts;
			params.polys = m_pmesh->polys;
			params.polyAreas = m_pmesh->areas;
			params.polyFlags = m_pmesh->flags;
			params.polyCount = m_pmesh->npolys;
			params.nvp = m_pmesh->nvp;
			params.detailMeshes = m_dmesh->meshes;
			params.detailVerts = m_dmesh->verts;
			params.detailVertsCount = m_dmesh->nverts;
			params.detailTris = m_dmesh->tris;
			params.detailTriCount = m_dmesh->ntris;

			// no off mesh connections yet
			m_offMeshConCount = 0;
			params.offMeshConVerts = m_offMeshConVerts;
			params.offMeshConRad = m_offMeshConRads;
			params.offMeshConDir = m_offMeshConDirs;
			params.offMeshConAreas = m_offMeshConAreas;
			params.offMeshConFlags = m_offMeshConFlags;
			params.offMeshConUserID = m_offMeshConId;
			params.offMeshConCount = m_offMeshConCount;

			params.walkableHeight = m_agentHeight;
			params.walkableRadius = m_agentRadius;
			params.walkableClimb = m_agentMaxClimb;
			rcVcopy(params.bmin, m_pmesh->bmin);
			rcVcopy(params.bmax, m_pmesh->bmax);
			params.cs = m_cfg.cs;
			params.ch = m_cfg.ch;

			EDITOR_LOG_INFO("Detour 2000");

			if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
			{
				m_ctx->log(RC_LOG_ERROR, "Could not build Detour navmesh.");
				EDITOR_LOG_WARN("Could not build Detour navmesh.");
				return false;
			}

			EDITOR_LOG_INFO("Detour 3000");

			m_navMesh = dtAllocNavMesh();
			if (!m_navMesh)
			{
				dtFree(navData);
				m_ctx->log(RC_LOG_ERROR, "Could not create Detour navmesh");
				EDITOR_LOG_WARN("Could not create Detour navmesh");
				return false;
			}

			EDITOR_LOG_INFO("Detour 4000");

			dtStatus status;

			status = m_navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
			if (dtStatusFailed(status))
			{
				dtFree(navData);
				m_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh");
				EDITOR_LOG_WARN("Could not init Detour navmesh");
				return false;
			}

			EDITOR_LOG_INFO("Detour 5000");

			m_navQuery = dtAllocNavMeshQuery();
			status = m_navQuery->init(m_navMesh, 2048);

			EDITOR_LOG_INFO("Detour 5500");

			if (dtStatusFailed(status))
			{
				m_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh query");
				EDITOR_LOG_WARN("Could not init Detour navmesh query");
				return false;
			}

			EDITOR_LOG_INFO("Detour 6000");
		}

		m_ctx->stopTimer(RC_TIMER_TOTAL);


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// cleanup stuff we don't need
		delete[] rc_verts;
		delete[] rc_tris;
		delete[] rc_trinorms;

		//CreateRecastPolyMesh(*m_pmesh);

		EDITOR_LOG_INFO("NavMeshBuild End");
		return true;
	}
	void NavMeshManager::RecastCleanup()
	{
		if (m_triareas)
			delete[] m_triareas;

		m_triareas = 0;

		rcFreeHeightField(m_solid);
		m_solid = 0;
		rcFreeCompactHeightfield(m_chf);
		m_chf = 0;
		rcFreeContourSet(m_cset);
		m_cset = 0;
		rcFreePolyMesh(m_pmesh);
		m_pmesh = 0;
		rcFreePolyMeshDetail(m_dmesh);
		m_dmesh = 0;
		dtFreeNavMesh(m_navMesh);
		m_navMesh = 0;

		dtFreeNavMeshQuery(m_navQuery);
		m_navQuery = 0;

		if (m_ctx) delete m_ctx;
	}

	void NavMeshManager::RenderMesh()
	{
		//	if (!m_geom || !m_geom->getMesh())
		//		return;

		//	glEnable(GL_FOG);
		//	glDepthMask(GL_TRUE);

		//	const float texScale = 1.0f / (m_cellSize * 10.0f);

		//	if (m_drawMode != DRAWMODE_NAVMESH_TRANS)
		//	{
		//		// Draw mesh
		//		duDebugDrawTriMeshSlope(&m_dd, m_geom->getMesh()->getVerts(), m_geom->getMesh()->getVertCount(),
		//			m_geom->getMesh()->getTris(), m_geom->getMesh()->getNormals(), m_geom->getMesh()->getTriCount(),
		//			m_agentMaxSlope, texScale);
		//		m_geom->drawOffMeshConnections(&m_dd);
		//	}

		//	glDisable(GL_FOG);
		//	glDepthMask(GL_FALSE);

		//	// Draw bounds
		//	const float* bmin = m_geom->getNavMeshBoundsMin();
		//	const float* bmax = m_geom->getNavMeshBoundsMax();
		//	duDebugDrawBoxWire(&m_dd, bmin[0], bmin[1], bmin[2], bmax[0], bmax[1], bmax[2], duRGBA(255, 255, 255, 128), 1.0f);
		//	m_dd.begin(DU_DRAW_POINTS, 5.0f);
		//	m_dd.vertex(bmin[0], bmin[1], bmin[2], duRGBA(255, 255, 255, 128));
		//	m_dd.end();

		//	if (m_navMesh && m_navQuery &&
		//		(m_drawMode == DRAWMODE_NAVMESH ||
		//			m_drawMode == DRAWMODE_NAVMESH_TRANS ||
		//			m_drawMode == DRAWMODE_NAVMESH_BVTREE ||
		//			m_drawMode == DRAWMODE_NAVMESH_NODES ||
		//			m_drawMode == DRAWMODE_NAVMESH_INVIS))
		//	{
		//		if (m_drawMode != DRAWMODE_NAVMESH_INVIS)
		//			duDebugDrawNavMeshWithClosedList(&m_dd, *m_navMesh, *m_navQuery, m_navMeshDrawFlags);
		//		if (m_drawMode == DRAWMODE_NAVMESH_BVTREE)
		//			duDebugDrawNavMeshBVTree(&m_dd, *m_navMesh);
		//		if (m_drawMode == DRAWMODE_NAVMESH_NODES)
		//			duDebugDrawNavMeshNodes(&m_dd, *m_navQuery);
		//		duDebugDrawNavMeshPolysWithFlags(&m_dd, *m_navMesh, SAMPLE_POLYFLAGS_DISABLED, duRGBA(0, 0, 0, 128));
		//	}

		//	glDepthMask(GL_TRUE);

		//	if (m_chf && m_drawMode == DRAWMODE_COMPACT)
		//		duDebugDrawCompactHeightfieldSolid(&m_dd, *m_chf);

		//	if (m_chf && m_drawMode == DRAWMODE_COMPACT_DISTANCE)
		//		duDebugDrawCompactHeightfieldDistance(&m_dd, *m_chf);
		//	if (m_chf && m_drawMode == DRAWMODE_COMPACT_REGIONS)
		//		duDebugDrawCompactHeightfieldRegions(&m_dd, *m_chf);
		//	if (m_solid && m_drawMode == DRAWMODE_VOXELS)
		//	{
		//		glEnable(GL_FOG);
		//		duDebugDrawHeightfieldSolid(&m_dd, *m_solid);
		//		glDisable(GL_FOG);
		//	}
		//	if (m_solid && m_drawMode == DRAWMODE_VOXELS_WALKABLE)
		//	{
		//		glEnable(GL_FOG);
		//		duDebugDrawHeightfieldWalkable(&m_dd, *m_solid);
		//		glDisable(GL_FOG);
		//	}
		//	if (m_cset && m_drawMode == DRAWMODE_RAW_CONTOURS)
		//	{
		//		glDepthMask(GL_FALSE);
		//		duDebugDrawRawContours(&m_dd, *m_cset);
		//		glDepthMask(GL_TRUE);
		//	}
		//	if (m_cset && m_drawMode == DRAWMODE_BOTH_CONTOURS)
		//	{
		//		glDepthMask(GL_FALSE);
		//		duDebugDrawRawContours(&m_dd, *m_cset, 0.5f);
		//		duDebugDrawContours(&m_dd, *m_cset);
		//		glDepthMask(GL_TRUE);
		//	}
		//	if (m_cset && m_drawMode == DRAWMODE_CONTOURS)
		//	{
		//		glDepthMask(GL_FALSE);
		//		duDebugDrawContours(&m_dd, *m_cset);
		//		glDepthMask(GL_TRUE);
		//	}
		//	if (m_chf && m_cset && m_drawMode == DRAWMODE_REGION_CONNECTIONS)
		//	{
		//		duDebugDrawCompactHeightfieldRegions(&m_dd, *m_chf);

		//		glDepthMask(GL_FALSE);
		//		duDebugDrawRegionConnections(&m_dd, *m_cset);
		//		glDepthMask(GL_TRUE);
		//	}
		//	if (m_pmesh && m_drawMode == DRAWMODE_POLYMESH)
		//	{
		//		glDepthMask(GL_FALSE);
		//		duDebugDrawPolyMesh(&m_dd, *m_pmesh);
		//		glDepthMask(GL_TRUE);
		//	}
		//	if (m_dmesh && m_drawMode == DRAWMODE_POLYMESH_DETAIL)
		//	{
		//		glDepthMask(GL_FALSE);
		//		duDebugDrawPolyMeshDetail(&m_dd, *m_dmesh);
		//		glDepthMask(GL_TRUE);
		//	}

		//	m_geom->drawConvexVolumes(&m_dd);

		//	if (m_tool)
		//		m_tool->handleRender();
		//	renderToolStates();

		//	glDepthMask(GL_TRUE);
		//}
	}
}