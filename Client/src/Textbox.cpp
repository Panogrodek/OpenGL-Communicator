#include "pch.h"
#include "ui/Textbox.hpp"
#include "Utilities/InputGuard.hpp"

#include "SPDLOG/spdlog.h"
#include "Client.hpp"

Textbox::Textbox(pl::AABB2D aabb)
{
	m_hitbox = aabb;
	glm::vec2 size = aabb.upperBound - aabb.lowerBound;
	m_body = new pl::RectangleShape(aabb.lowerBound + size / 2.f, size);
	m_text = new pl::Text("arial");
	m_text->Update();
	m_text->SetString("kuba na tescia");
	m_text->SetTextBounds(aabb);
}

Textbox::~Textbox()
{
	delete m_text;
	delete m_body;
}

void Textbox::Update(){

	//Detect if shift is pressed
	bool shiftPressed = false;
	auto& lshift = pl::inputGuard[GLFW_KEY_LEFT_SHIFT];
	auto& rshift = pl::inputGuard[GLFW_KEY_RIGHT_SHIFT];
	if (lshift == pl::KeyCode::PRESS || lshift == pl::KeyCode::REPEAT 
		|| rshift == pl::KeyCode::PRESS || rshift == pl::KeyCode::REPEAT)
		shiftPressed = true;

	//Char check for all non special chars (non writable)
	for (char c = 32; c < 94; c++) {
		if (pl::inputGuard[c] == pl::KeyCode::PRESS || pl::inputGuard[c] == pl::KeyCode::REPEAT) {
			pl::inputGuard.Lock(c);
			if (!shiftPressed) {
				//we shift the char value by 32 for all letters (to make them small)
				m_text->SetString(m_text->GetString() + char(c + 32 * (c > 64 && c < 91)));
			}
			else
				HandleShift(c);
		}
	}

	//Char checks for special chars
	auto& backspace = pl::inputGuard[GLFW_KEY_BACKSPACE];
	if (backspace == pl::KeyCode::PRESS || backspace == pl::KeyCode::REPEAT) {
		pl::inputGuard.Lock(GLFW_KEY_BACKSPACE);
		std::string textString = m_text->GetString();
		m_text->SetString(textString.substr(0, textString.size() - 1));
	}

	auto& enter = pl::inputGuard[GLFW_KEY_ENTER];
	if (enter == pl::KeyCode::PRESS || enter == pl::KeyCode::REPEAT) {
		pl::inputGuard.Lock(GLFW_KEY_ENTER);
		if (shiftPressed) {
			m_text->SetString(m_text->GetString() + '\n');
		}
		else if(m_text->GetString() != "") { //we dont send empty messages
			spdlog::info("Sending message {}", m_text->GetString());
			pl::Packet* packet = new pl::Packet(pl::PacketType::ChatMessage);
			*packet << m_text->GetString();
			client.SendPacket(packet);
			m_text->SetString("");
		}
	}
}

void Textbox::Render()
{
	m_body->Draw();
	m_text->Draw();
}

void Textbox::SetBody(pl::RectangleShape& body)
{
	m_body = &body;
}

void Textbox::SetText(pl::Text& text)
{
	m_text = &text;
}

pl::RectangleShape& Textbox::GetBody()
{
	return *m_body;
}

pl::Text& Textbox::GetText()
{
	return *m_text;
}

void Textbox::HandleShift(char c)
{
	//kinda makes you think what did they have in mind when creating ascii tables
	switch (c)
	{
	case '1':										  //WHY?
		m_text->SetString(m_text->GetString() + '!'); //49 -> 33
		break;
	case '2':
		m_text->SetString(m_text->GetString() + '@'); //50 -> 64
		break;
	case '3':
		m_text->SetString(m_text->GetString() + '#'); //51 -> 35
		break;
	case '4':
		m_text->SetString(m_text->GetString() + '$');
		break;
	case '5':
		m_text->SetString(m_text->GetString() + '%');
		break;
	case '6':
		m_text->SetString(m_text->GetString() + '^');
		break;
	case '7':
		m_text->SetString(m_text->GetString() + '&');
		break;
	case '8':
		m_text->SetString(m_text->GetString() + '*');
		break;
	case '9':
		m_text->SetString(m_text->GetString() + '(');
		break;
	case '0':
		m_text->SetString(m_text->GetString() + ')');
		break;
	case '-':
		m_text->SetString(m_text->GetString() + '_');
		break;
	case '=':
		m_text->SetString(m_text->GetString() + '+');
		break;
	case '[':
		m_text->SetString(m_text->GetString() + '{');
		break;
	case ']':
		m_text->SetString(m_text->GetString() + '}');
		break;
	case '\\':
		m_text->SetString(m_text->GetString() + '|');
		break;
	case ';':
		m_text->SetString(m_text->GetString() + ':');
		break;
	case '\'':
		m_text->SetString(m_text->GetString() + '"');
		break;
	case ',':
		m_text->SetString(m_text->GetString() + '<');
		break;
	case '.':
		m_text->SetString(m_text->GetString() + '>');
		break;
	case '/':
		m_text->SetString(m_text->GetString() + '?');
		break;
	default:
		m_text->SetString(m_text->GetString() + c); //normal characters
		break;
	}
}
