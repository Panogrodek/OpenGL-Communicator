#pragma once
#include "Graphics/Graphics.hpp"

/**
 * @file Textbox.hpp
 * @brief Contains the declaration of the Textbox class.
 */

class Textbox {
public:
	Textbox() {};
	/**
	* @brief Constructs a new Textbox object.
	*
	* @param aabb The axis-aligned bounding box (AABB) defining the size and position of the textbox.
	*/
	Textbox(pl::AABB2D aabb);

	/**
	* @brief Destroys the Textbox object.
	*
	* Deletes the body and text objects.
	*/
	~Textbox();

	/**
	* @brief Updates the textbox.
	*
	* This function handles text input, including detecting special characters such as backspace and enter.
	*/
	void Update();
	/**
	* @brief Renders the Textbox.
	*/
	void Render();

	/**
	* @brief Sets the body of the Textbox.
	* @param body The RectangleShape representing the body of the textbox.
	*/
	void SetBody(pl::RectangleShape& body);
	/**
	* @brief Sets the text of the Textbox.
	* @param text The Text representing the text displayed in the textbox.
	*/
	void SetText(pl::Text& text);

	/**
	* @brief Retrieves the body of the Textbox.
	* @return Reference to the RectangleShape representing the body of the textbox.
	*/
	pl::RectangleShape& GetBody();
	/**
	* @brief Retrieves the text of the Textbox.
	* @return Reference to the Text representing the text displayed in the textbox.
	*/
	pl::Text& GetText();
private:
	/**
	* @brief Handles shift key functionality for special characters.
	* @param c The character for which shift functionality is applied.
	*/
	void HandleShift(char c);
	/**
	* @brief Checks for special commands in the text input.
	* @return True if a command is found and processed successfully, false otherwise.
	*/
	bool LookForCommands();
	pl::AABB2D m_hitbox;
	pl::RectangleShape* m_body;
	pl::Text* m_text;
};