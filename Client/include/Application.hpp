#pragma once

/**
 * @file Application.hpp
 * @brief Definition of the Application class
 */

class Application {
public:
	/**
	* @brief Constructor of the Application class.
	*
	* Initializes an object of the Application class, initializes the rendering engine, and adds a Menu state to the state machine.
	*/
	Application();
	/**
	* @brief Destructor of the Application class.
	*
	* Destroys the client and the rendering engine.
	*/
	~Application();

	/**
	* @brief Method running the main application loop.
	*
	* The main application loop, which runs until the renderer window is closed.
	* Updates the delta time, calls the Update and Render methods.
	*/
	void Run();
private:
	/**
	 * @brief Method for updating the application state.
	 *
	 * Runs the client and updates the state machine.
	 */
	void Update();

	/**
	 * @brief Method for rendering the application.
	 *
	 * Begins drawing with the renderer, renders the state machine, and ends drawing.
	 */
	void Render();

	float m_timeAccumulator = 0.f;
	float m_lastFrame = 0.f;
};