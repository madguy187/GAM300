#pragma once

namespace Eclipse
{
	/*************************************************************************/
	/*                         FINITE STATE MACHINE                          */
	/*************************************************************************/
	template <typename T>
	class FSM
	{
	public:
		FSM();

		// Entering Next State
		// State -> Going to Next State
		virtual void BeginState(T) {}
		// Updating Current State
		// State -> Mainitaning at Current State
		virtual void UpdateState(T) {}
		// Leaving Previous State
		// State -> Leaving Previous State
		virtual void EndState(T) {}

		void SetState(T state);
		void UpdateFSM(float dt = 0.0f);

		float GetTimeInCurrentState() const;
		T GetCurrentState() const;
		T GetPreviousState() const;
		T GetNextState() const;

		void SetNextState(T state);
	private:
		float m_CurrentStateTimer;
		int m_CurrentState;
		int m_NextState;
		int m_PreviousState;
	};

	template<typename T>
	inline FSM<T>::FSM() :
		m_CurrentStateTimer{ 0.0f }, 
		m_CurrentState{ -1 },
		m_NextState{ -1 },
		m_PreviousState{ -1 }
	{}

	template<typename T>
	inline void FSM<T>::SetState(T state)
	{
		EndState((T)m_CurrentState);
		m_PreviousState = m_CurrentState;
		m_NextState = m_CurrentState = (int)state;
		m_CurrentStateTimer = 0.0f;
		BeginState((T)m_CurrentState);
	}

	template<typename T>
	inline void FSM<T>::UpdateFSM(float dt)
	{
		if (m_CurrentState != -1)
		{
			if (m_NextState == m_CurrentState)
			{
				m_CurrentStateTimer += dt;
				UpdateState((T)m_CurrentState);
				// T printState = (T)m_CurrentState;
			    // std::cout << lexical_cast_toStr<T>(printState) << std::endl;
			}
			else
			{
				SetState((T)m_NextState);
			}
		}
	}

	template<typename T>
	inline float FSM<T>::GetTimeInCurrentState() const
	{
		return m_CurrentStateTimer;
	}

	template<typename T>
	inline T FSM<T>::GetCurrentState() const
	{
		return (T)m_CurrentState;
	}

	template<typename T>
	inline T FSM<T>::GetPreviousState() const
	{
		return (T)m_PreviousState;
	}

	template<typename T>
	inline T FSM<T>::GetNextState() const
	{
		return (T)m_NextState;
	}

	template<typename T>
	inline void FSM<T>::SetNextState(T state)
	{
		m_NextState = (int)state;
	}
}
