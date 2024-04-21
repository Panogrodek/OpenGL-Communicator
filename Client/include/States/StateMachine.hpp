#pragma once
#include <queue>

namespace priv {
	class StateMachine;
}

class State {
public:
	virtual ~State() {};

	virtual void Update() = 0;
	virtual void Render() = 0;
protected:
	friend class priv::StateMachine;
	bool m_running = false;
};

namespace priv {
	class StateMachine {
	public:
		void Update();
		void Render();

		void Push(State* state);
		void Pop();
		void Clear();

		::State* GetTop();
	private:
		std::queue<::State*> m_states;
	};
}

inline priv::StateMachine stateMachine;