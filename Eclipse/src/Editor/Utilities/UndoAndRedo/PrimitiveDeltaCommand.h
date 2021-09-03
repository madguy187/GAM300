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
			m_Variable{ ogVar }, m_VariableToKeep{ newVar }, m_NewVariable2{ newVar }, m_OldVariable{ T{} }
		{}

		virtual void Execute() override
		{
			m_OldVariable = m_Variable;
			m_VariableToKeep = m_NewVariable2;
		}

		virtual void Undo() override
		{
			m_VariableToKeep = m_OldVariable;
		}

		virtual bool MergeCmds(ICommand* otherCmd) override
		{
			PrimitiveDeltaCommand* primDeltaCmd = dynamic_cast<PrimitiveDeltaCommand*>(otherCmd);

			if (primDeltaCmd)
			{
				// Check if both command is referring to the same variable
				if (&primDeltaCmd->m_VariableToKeep == &this->m_VariableToKeep)
				{
					primDeltaCmd->m_NewVariable2 = this->m_NewVariable2;
					return true;
				}
			}

			return false;
		}

	private:
		T m_Variable;
		T& m_VariableToKeep;
		T m_NewVariable2;
		T m_OldVariable;
	};
}