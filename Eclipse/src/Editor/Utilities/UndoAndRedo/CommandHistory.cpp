#include "CommandHistory.h"

namespace Eclipse
{
	ICommand* CommandHistory::m_Commands[MAX_SIZE] = {};
	int CommandHistory::m_CommandSize = 0;
	int CommandHistory::m_CommandPtrIndex = 0;

	void CommandHistory::RegisterCommand(ICommand* cmd)
	{
		cmd->execute();

		// Delete all the existing commands infront when a new command is done
		if (m_CommandPtrIndex < m_CommandSize - 1)
		{
			for (int i = m_CommandSize - 1; i > m_CommandPtrIndex; i--)
				delete m_Commands[i];

			m_CommandSize = m_CommandPtrIndex + 1;
		}

		// Insert command into array
		m_Commands[m_CommandSize] = cmd;
		m_CommandSize++;

		// Check if the command before and the current one are able to merge
		if (m_CommandSize > 1 && m_Commands[m_CommandSize - 1]->CanMerge() && 
			m_Commands[m_CommandSize - 2]->CanMerge())
		{
			// Check if merge is successful
			if (m_Commands[m_CommandSize - 1]->MergeCmds(m_Commands[m_CommandSize - 2]))
			{
				// Delete the current one if successful after merging
				delete m_Commands[m_CommandSize - 1];
				m_CommandSize--;
			}
		}

		// Decrement Pointer Index
		m_CommandPtrIndex = m_CommandSize - 1;
	}

	void CommandHistory::DisableMergeForMostRecendCommand()
	{
		// Manually Stop merging all commands
		if ((m_CommandSize - 1) >= 0)
			m_Commands[m_CommandSize - 1]->DisableMerge();
	}

	void CommandHistory::Undo()
	{
		if (m_CommandSize >= 0)
		{
			m_Commands[m_CommandPtrIndex]->undo();
			m_CommandPtrIndex--;
		}
	}

	void CommandHistory::Redo()
	{
		int redoIndex = m_CommandPtrIndex + 1;

		if (redoIndex < m_CommandSize && redoIndex >= 0)
		{
			m_Commands[redoIndex]->execute();
			m_CommandPtrIndex = redoIndex;
		}
	}

	void CommandHistory::Clear()
	{
		for (int i = 0; i < m_CommandSize; ++i)
		{
			if (m_Commands[i])
				delete m_Commands[i];
		}

		m_CommandSize = 0;
		m_CommandPtrIndex = 0;
	}
}
