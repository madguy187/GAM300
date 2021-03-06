#pragma once

#include "ICommand.h"

#define MAX_SIZE 1000

namespace Eclipse
{
	class CommandHistory
	{
	public:
		static void RegisterCommand(ICommand* cmd);
		static void DisableMergeForMostRecentCommand();
		static void Undo();
		static void Redo();
		static void Clear();
	private:
		static ICommand* m_Commands[MAX_SIZE];
		static int m_CommandSize;
		static int m_CommandPtrIndex;
	};
}
