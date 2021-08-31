#pragma once

#include "ICommand.h"

#define MAX_SIZE 500

namespace Eclipse
{
	class CommandHistory
	{
	public:
		static void RegisterCommand(ICommand* cmd);
		static void DisableMergeForMostRecendCommand();
		static void Undo();
		static void Redo();
		static void Clear();
	private:
		static ICommand* m_Commands[MAX_SIZE];
		static int m_CommandSize;
		static int m_CommandPtrIndex;
	};
}
