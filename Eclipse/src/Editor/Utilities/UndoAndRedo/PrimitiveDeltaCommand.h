#pragma once

#include "ICommand.h"

namespace Eclipse
{
	/*************************************************************************/
	/*                  USE ONLY FOR PRIMITIVE DATA TYPES                    */
	/*************************************************************************/
	template <typename T>
	class PrimitiveDeltaCommand : public ICommand
	{
	public:
		PrimitiveDeltaCommand(T& ogVar, T& newVar) :
			m_Variable{ ogVar }, m_NewVariable{ newVar }, m_OldVariable{ T{} }
		{}

		virtual void Execute() override
		{
			m_OldVariable = m_Variable;
			m_Variable = m_NewVariable;
		}

		virtual void Undo() override
		{
			m_Variable = m_OldVariable;
		}

		virtual bool MergeCmds(ICommand* otherCmd) override
		{
			PrimitiveDeltaCommand* primDeltaCmd = dynamic_cast<PrimitiveDeltaCommand*>(otherCmd);

			if (primDeltaCmd)
			{
				// Check if both command is referring to the same variable
				if (&primDeltaCmd->m_Variabe == &this->m_Variabe)
				{
					primDeltaCmd->m_NewVariable = this->m_NewVariable;
					return true;
				}
			}

			return false;
		}

	private:
		T& m_Variabe;
		T m_NewVariable;
		T m_OldVariable;
	};
}