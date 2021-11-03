#pragma once
namespace Eclipse
{
	struct NMVertex
	{
		ECVec3 Position;
	};

	struct NMPolygon
	{
		std::vector<std::pair<NMVertex, NMVertex>> _edges;
	};

	struct NavMeshComponent
	{
		NMPolygon _Poly;
		bool _init;
	};
}