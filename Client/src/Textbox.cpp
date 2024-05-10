#include "pch.h"
#include "ui/Textbox.hpp"
#include "ui/Logbox.hpp"
#include "ui/ClientList.hpp"
#include "Utilities/InputGuard.hpp"

#include "SPDLOG/spdlog.h"
#include "Client.hpp"
#include <Utilities/Utilities.hpp>

Textbox::Textbox(pl::AABB2D aabb)
{
	m_hitbox = aabb;
	glm::vec2 size = aabb.upperBound - aabb.lowerBound;
	m_body = new pl::RectangleShape(aabb.lowerBound + size / 2.f, size);
	m_text = new pl::Text("arial");
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
				m_text->AddChar(char(c + 32 * (c > 64 && c < 91)));
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
		m_text->RemoveLast();
	}

	auto& enter = pl::inputGuard[GLFW_KEY_ENTER];
	if (enter == pl::KeyCode::PRESS || enter == pl::KeyCode::REPEAT) {
		pl::inputGuard.Lock(GLFW_KEY_ENTER);
		if (shiftPressed) {
			m_text->AddChar('\n');
		}
		else if(m_text->GetString() != "") { //we dont send empty messages
			spdlog::info("Sending message {}", m_text->GetString());
			if (!LookForCommands()) {
				pl::Packet* packet = new pl::Packet(pl::PacketType::ChatMessage, pl::ChatType::HasSendMessage);
				*packet << m_text->GetString();
				client.SendPacket(packet);
			}
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
		m_text->AddChar('!'); //49 -> 33
		break;
	case '2':
		m_text->AddChar('@'); //50 -> 64
		break;
	case '3':
		m_text->AddChar('#'); //51 -> 35
		break;
	case '4':
		m_text->AddChar('$');
		break;
	case '5':
		m_text->AddChar('%');
		break;
	case '6':
		m_text->AddChar('^');
		break;
	case '7':
		m_text->AddChar('&');
		break;
	case '8':
		m_text->AddChar('*');
		break;
	case '9':
		m_text->AddChar('(');
		break;
	case '0':
		m_text->AddChar(')');
		break;
	case '-':
		m_text->AddChar('_');
		break;
	case '=':
		m_text->AddChar('+');
		break;
	case '[':
		m_text->AddChar('{');
		break;
	case ']':
		m_text->AddChar('}');
		break;
	case '\\':
		m_text->AddChar('|');
		break;
	case ';':
		m_text->AddChar(':');
		break;
	case '\'':
		m_text->AddChar('"');
		break;
	case ',':
		m_text->AddChar('<');
		break;
	case '.':
		m_text->AddChar('>');
		break;
	case '/':
		m_text->AddChar('?');
		break;
	default:
		m_text->AddChar(c); //normal characters
		break;
	}
}

bool Textbox::LookForCommands() //only returns false, when the packet could not be send
{
	std::string textString = m_text->GetString();
	if (textString.length() < 2)
		return false;

	if (textString[0] != '/')
		return false;

	pl::Packet* packet = nullptr;
	std::string whisperTo{};
	switch (textString[1])
	{
	case 'h': //show help info
		logBox.GetText().SetDrawingColor(glm::vec4(26, 92, 56, 255) / 255.f);
		logBox.AddMessage("/h -> shows this message\n");
		logBox.AddMessage("/n nick -> sets your nick. Nick must be less than\n16 characters\n");
		logBox.AddMessage("/w receiver message -> creates a secret message that only receiver sees\n");
		logBox.GetText().SetDrawingColor(glm::vec4(1.0f));
		return true;
	case 'n': //change nick
		packet = new pl::Packet(pl::PacketType::ChatMessage, pl::ChatType::SetNick);
		textString.erase(0, 3);
		if (textString.length() > 16) { //nick is too long
			logBox.GetText().SetDrawingColor(glm::vec4(112, 3, 41,255)/255.f);
			logBox.AddMessage("Your nick must be less than 16 characters!\n");
			logBox.GetText().SetDrawingColor(glm::vec4(1.0f));
			return true;
		}
		for (auto& c : textString) {
			if (c >= 48 && c <= 57) //numbers 0-9
				continue;
			if (c >= 65 && c <= 90) //capital letters A-Z
				continue;
			if (c >= 97 && c <= 122) //lower letters a-z
				continue;
			logBox.GetText().SetDrawingColor(glm::vec4(112, 3, 41, 255) / 255.f);
			logBox.AddMessage("Your nick can only include numbers and letters!\n");
			logBox.GetText().SetDrawingColor(glm::vec4(1.0f));
			return true;
		}
		*packet << textString;
		break;
	case 'w': //whisper
		textString.erase(0, 3);
		whisperTo = ErasePart(' ', textString);
		if (whisperTo.empty()) {
			logBox.GetText().SetDrawingColor(glm::vec4(112, 3, 41, 255) / 255.f);
			logBox.AddMessage("Try: /w nick message\n");
			logBox.GetText().SetDrawingColor(glm::vec4(1.0f));
			return true;
		}
		whisperTo.pop_back();
		if (!clientList.DoesExist(whisperTo)) {
			logBox.GetText().SetDrawingColor(glm::vec4(112, 3, 41, 255) / 255.f);
			logBox.AddMessage(whisperTo + " is not connected!\n");
			logBox.GetText().SetDrawingColor(glm::vec4(1.0f));
			return true;
		}

		if (textString.empty()) {
			logBox.GetText().SetDrawingColor(glm::vec4(112, 3, 41, 255) / 255.f);
			logBox.AddMessage("You tried to send empty message to " + whisperTo + "!\n");
			logBox.GetText().SetDrawingColor(glm::vec4(1.0f));
			return true;
		}

		packet = new pl::Packet(pl::PacketType::ChatMessage, pl::ChatType::WhispersTo);
		*packet << whisperTo + ":" + textString;
		logBox.GetText().SetDrawingColor(glm::vec4(116, 114, 130, 255) / 255.f);
		logBox.AddMessage("[You/" + whisperTo + "] " + textString + "\n");
		logBox.GetText().SetDrawingColor(glm::vec4(1.0f));
		break;
	default:
		return false;
	}
	if (packet == nullptr)
		return false;
	client.SendPacket(packet);
	return true;
}
