#pragma once

#include "ICommand.h"

namespace Eclipse
{
	class ECVec4DeltaCommand : public ICommand
	{
	public:
		ECVec4DeltaCommand(ECVec4& ogVec, ECVec4& newVec) :
			m_Vector{ ogVec }, m_NewVector{ newVec }, m_OldVector{ ECVec4{} }
		{}

		virtual void Execute() override
		{
			m_OldVector = m_Vector;
			m_Vector.setX(m_NewVector.getX());
			m_Vector.setY(m_NewVector.getY());
			m_Vector.setZ(m_NewVector.getZ());
			m_Vector.setW(m_NewVector.getW());
		}

		virtual void Undo() override
		{
			m_Vector.setX(m_OldVector.x);
			m_Vector.setY(m_OldVector.y);
			m_Vector.setZ(m_OldVector.z);
			m_Vector.setW(m_OldVector.w);
		}

		virtual bool MergeCmds(ICommand* otherCmd) override
		{
			ECVec4DeltaCommand* vecDeltaCmd = dynamic_cast<ECVec4DeltaCommand*>(otherCmd);

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
		ECVec4& m_Vector;
		ECVec4 m_NewVector;
		ECVec4 m_OldVector;
	};
}