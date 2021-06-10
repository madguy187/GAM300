/**********************************************************************************
* \file EventSystems.h
* \brief Declaration of event system
* \author Wong, Swee Jong Nico, 100% Code Contribution
*
*
* \copyright Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "pch.h"
#include "Global.h"

namespace Eclipse
{
	template <typename Event>
	class EventSystem
	{
	public:
		using EventID = uint32_t;
		using EventHandlers = std::unordered_map< EventID, std::function<void(const Event&)> >;

		static EventID registerListener(std::function<bool(const Event&)> handle)
		{
			EventID newID = getNextID();
			getHandlers()[newID] = handle;
			return newID;
		}

		static void removeListener(EventID id)
		{
			getHandlers().erase(id);
		}

		template <typename Event>
		static void dispatchEvent(const Event& t)
		{
			EventSystem<Event>::triggerEvent(t);
		}

		template <typename T>
		static void dispatchEvent(T* t)
		{
			dispatchEvent<T>(*t);
		}

	private:
		inline static EventID _id = 0;

		inline static EventID getNextID()
		{
			return _id++;
		}

		inline static EventHandlers& getHandlers()
		{
			static EventHandlers handle;
			return handle;
		}

		static void triggerEvent(const Event& events)
		{
			for (const auto& fnPtr : getHandlers())
			{
				fnPtr.second(events);
			}
		}
	};
}