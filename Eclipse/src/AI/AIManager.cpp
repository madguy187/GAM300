#include "pch.h"
#include "AIManager.h"
namespace Eclipse
{

	/*AstarNode* AIManager::GetNextCurrent()
	{
		AstarNode* temp = openlist.front();
		for (auto node : openlist)
		{
			if (node->fcost < temp->fcost)
				temp = node;
		}

		return temp;
	}*/

	/*void AIManager::CalculatePath(AstarNode& start, AstarNode& end)
	{
		AstarNode* current = &start;
		openlist.push_back(current);
		current->gcost = 0;
		current->fcost = current->gcost;
		for (auto _neighbour : current->neighbours)
		{
			openlist.push_back(_neighbour.neighbour);
			_neighbour.neighbour->gcost = _neighbour.distfromNeighbour;
			_neighbour.neighbour->fcost = _neighbour.neighbour->gcost + _neighbour.neighbour->hcost;
			_neighbour.neighbour->prev = current;
		}
		openlist.pop_front();
		closedlist.push_back(current);

		while (std::find(openlist.begin(), openlist.end(), end) == openlist.end())
		{
			current = GetNextCurrent();
			for (auto _neighbour : current->neighbours)
			{
				openlist.push_back(_neighbour.neighbour);
				if (current->gcost + _neighbour.distfromNeighbour < _neighbour.neighbour->gcost)
				{
					_neighbour.neighbour->gcost = current->gcost + _neighbour.distfromNeighbour;
					_neighbour.neighbour->fcost = _neighbour.neighbour->gcost + _neighbour.neighbour->hcost;
					_neighbour.neighbour->prev = current;
				}
			}
			
		}
	}*/
}