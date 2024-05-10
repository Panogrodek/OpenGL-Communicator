#pragma once
#include <queue>

/**
 * @file StateMachine.hpp
 * @brief Contains the declaration of the StateMachine singleton class as well as State class.
 */

namespace priv {
	class StateMachine;
}

class State {
public:
	/**
	* @brief Default state deconstructor.
	*
	* This is the default state deconstructor.
	*/
	virtual ~State() = default;

	/**
	* @brief Update of the state.
	*
	* This function updates the state.
	*/
	virtual void Update() = 0;
	/**
	* @brief Render of the state.
	*
	* This function renders the state.
	*/
	virtual void Render() = 0;
protected:
	friend class priv::StateMachine;
	bool m_running = false;
};

namespace priv {
	class StateMachine {
	public:
		/**
		* @brief Updates the state machine.
		*
		* This function updates the state machine by executing the update function of the top state.
		*/
		void Update();
		/**
		* @brief Renders the state machine.
		*
		* This function renders the state machine by executing the render function of the top state.
		*/
		void Render();

		/**
		* @brief Pushes a state onto the state machine.
		*
		* @param state A pointer to the state to be pushed.
		*/
		void Push(State* state);
		/**
		* @brief Pops the top state from the state machine.
		*
		* This function removes the top state from the state machine and deletes it.
		*/
		void Pop();
		/**
		* @brief Clears all states from the state machine.
		*
		* This function removes and deletes all states from the state machine.
		*/
		void Clear();

		/**
		* @brief Gets the top state from the state machine.
		*
		* @return A pointer to the top state.
		*/
		::State* GetTop();
	private:
		std::queue<::State*> m_states;
	};
}

inline priv::StateMachine stateMachine;