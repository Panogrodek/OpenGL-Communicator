#pragma once
#include "StateMachine.hpp"
#include "Graphics/Graphics.hpp"
#include "ui/Textbox.hpp"
#include "ui/Logbox.hpp"

/**
 * @file Menu.hpp
 * @brief Contains the declaration of the Menu class. The Menu class is a state updated by application
 */

class Menu : public State {
public:
	/**
	* @brief Constructs a new Menu state.
	*
	* Initializes the Menu state by loading fonts, creating a camera, and initializing UI elements.
	*/
	Menu();
	/**
	 * @brief Destroys the Menu state.
	 *
	 * Cleans up resources associated with the Menu state.
	 */
	virtual ~Menu();

	/**
	* @brief Updates the Menu state.
	*
	* This function updates the Menu state.
	*/
	virtual void Update() override;
	/**
	* @brief Renders the Menu state.
	*
	* This function renders the Menu state.
	*/
	virtual void Render() override;
private:
	pl::Camera* m_camera;
	Textbox* m_textbox;
};