#pragma once

#include "ICommand.h"

namespace Eclipse
{
	class ECVec2DeltaCommand : public ICommand
	{
	public:
		ECVec2DeltaCommand(ECVec2& ogVec, ECVec2& newVec) :
			m_Vector{ ogVec }, m_NewVector{ newVec }, m_OldVector{ ECVec2{} }
		{}

		virtual void Execute() override
		{
			m_OldVector = m_Vector;
			m_Vector.setX(m_NewVector.getX());
			m_Vector.setY(m_NewVector.getY());
		}

		virtual void Undo() override
		{
			m_Vector.setX(m_OldVector.x);
			m_Vector.setY(m_OldVector.y);
		}

		virtual bool MergeCmds(ICommand* otherCmd) override
		{
			ECVec2DeltaCommand* vecDeltaCmd = dynamic_cast<ECVec2DeltaCommand*>(otherCmd);

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
		ECVec2& m_Vector;
		ECVec2 m_NewVector;
		ECVec2 m_OldVector;
	};
}