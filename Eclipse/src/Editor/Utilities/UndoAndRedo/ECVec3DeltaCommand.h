#pragma once

#include "ICommand.h"

namespace Eclipse
{
	class ECVec3DeltaCommand : public ICommand
	{
	public:
		ECVec3DeltaCommand(ECVec3& ogVec, ECVec3& newVec) :
			m_Vector{ ogVec }, m_NewVector{ newVec }, m_OldVector{ ECVec3{} }
		{}

		virtual void Execute() override
		{
			m_OldVector = m_Vector;
			m_Vector.setX(m_NewVector.getX());
			m_Vector.setY(m_NewVector.getY());
			m_Vector.setZ(m_NewVector.getZ());
		}

		virtual void Undo() override
		{
			m_Vector.setX(m_OldVector.getX());
			m_Vector.setY(m_OldVector.getY());
			m_Vector.setZ(m_OldVector.getZ());
		}

		virtual bool MergeCmds(ICommand* otherCmd) override
		{
			ECVec3DeltaCommand* vecDeltaCmd = dynamic_cast<ECVec3DeltaCommand*>(otherCmd);

			if (vecDeltaCmd)
			{
				// Check if both command is referring to the same variable
				if (&vecDeltaCmd->m_Vector == &this->m_Vector)
				{
					vecDeltaCmd->m_NewVector = this->m_NewVector;
					return true;
				}
			}

			return false;
		}

	private:
		ECVec3& m_Vector;
		ECVec3 m_NewVector;
		ECVec3 m_OldVector;
	};
}