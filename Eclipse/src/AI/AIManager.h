#pragma once
#include "ECS/ComponentManager/Components/AIComponent.h"
using ASNodeID = uint16_t;
namespace Eclipse
{
	struct grid
	{
		float _squareDist{ 0 };
	};
	struct NeighbourInfo
	{
		AstarNode* neighbour;
		int distfromNeighbour;
	};

	struct AstarNode
	{
		ASNodeID _ID;
		ECVec3 Position;
		int hcost;
		int gcost;
		int fcost;
		std::vector<NeighbourInfo> neighbours;
		AstarNode* prev;
	};


	struct AstarPathComponent
	{


	};


	class AIManager
	{
	private:
		AstarNode current;
		std::list<AstarNode*> openlist;
		std::list<AstarNode*> closedlist;
		std::vector<AstarNode> _Nodes;
	public:
		AstarNode* GetNextCurrent();
		void CalculatePath(AstarNode& start, AstarNode& end);
	};
}