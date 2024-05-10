#pragma once
#include "Graphics/Graphics.hpp"

/**
 * @file Logbox.hpp
 * @brief Contains the declaration of the LogBox singleton class.
 */

namespace priv {

	class LogBox {
	public:
		/**
		* @brief Initializes the LogBox object.
		*
		* Initializes the LogBox object with the given AABB (Axis-Aligned Bounding Box).
		*
		* @param aabb The AABB to initialize the LogBox with.
		*/
		void Init(pl::AABB2D aabb);
		/**
		* @brief Destructor for the LogBox object.
		*
		* Deletes the memory allocated for the body and text of the LogBox.
		*/
		~LogBox();

		/**
		* @brief Updates the LogBox object.
		*
		* This function updates the LogBox object based on the mouse offset.
		*/
		void Update();
		/**
		* @brief Renders the LogBox object.
		*
		* This function renders the LogBox object.
		*/
		void Render();

		/**
		 * @brief Gets the body of the LogBox object.
		 *
		 * @return A reference to the RectangleShape representing the body of the LogBox.
		 */
		pl::RectangleShape& GetBody();

		/**
		 * @brief Gets the text of the LogBox object.
		 *
		 * @return A reference to the Text object representing the text of the LogBox.
		 */
		pl::Text& GetText();

		/**
		 * @brief Adds a message to the LogBox.
		 *
		 * Adds a message to the LogBox object and updates its position accordingly.
		 *
		 * @param message The message to be added to the LogBox.
		 */
		void AddMessage(std::string message);
	private:
		std::vector<std::string> s_incomingMessages;
		pl::RectangleShape* m_body;
		pl::Text* m_text;

		float m_ceiling;
		float m_bottom;
	};
}

inline priv::LogBox logBox;