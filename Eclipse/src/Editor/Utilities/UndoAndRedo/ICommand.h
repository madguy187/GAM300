#pragma once

namespace Eclipse
{
	class ICommand
	{
	public:
		virtual void Execute() = 0;
		virtual void Undo() = 0;
		virtual bool MergeCmds(ICommand* otherCmd) = 0;
		virtual ~ICommand() = default;

		void DisableMerge() { CanMerge_ = false; }
		bool CanMerge() { return CanMerge_; }
	protected:
		bool CanMerge_{ true };
	private:
		static int ID_;
	};
}

#include "CommandHistory.h"
#include "ECVec3DeltaCommand.h"
#include "ECVec2DeltaCommand.h"