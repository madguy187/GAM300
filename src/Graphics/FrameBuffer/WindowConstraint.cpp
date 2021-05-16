#include "pch.h"
#include "WindowConstraint.h"

#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))

namespace CustomConstraint
{
  void ResizeCallBack(ImGuiSizeCallbackData* data)
  {
	float ratio = *(float*)data->UserData;
	data->DesiredSize.x = IM_MAX(data->DesiredSize.x, data->DesiredSize.y);
	data->DesiredSize.y = data->DesiredSize.x / ratio;
  }
}