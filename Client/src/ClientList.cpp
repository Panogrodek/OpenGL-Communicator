#include "pch.h"
#include "ui/ClientList.hpp"

using namespace priv;

void ClientList::Init(pl::AABB2D aabb)
{
	glm::vec2 size = aabb.upperBound - aabb.lowerBound;
	m_body = new pl::RectangleShape(aabb.lowerBound + size / 2.f, size);

	m_text = new pl::Text("arial");
	m_text->SetSize({ 0.5f,0.5f });
	m_text->SetTextBounds(aabb);
}

void ClientList::Update()
{
}

void ClientList::Render()
{
	m_body->Draw();
	m_text->Draw();
}

pl::RectangleShape& ClientList::GetBody()
{
	return *m_body;
}

pl::Text& ClientList::GetText()
{
	return *m_text;
}

void ClientList::SetMe(std::string me)
{
	for (int i = 0; i < m_clients.size(); i++) {
		if (m_clients.at(i).nick != me)
			continue;

		m_me = &m_clients.at(i);
		break;
	}

	m_text->SetString("");
	for (auto& client : m_clients) {
		m_text->AddString(client.nick + "\n");
	}
}

void ClientList::ChangeClientsNick(std::string old, std::string _new)
{
	for (int i = 0; i < m_clients.size(); i++) {
		auto& client = m_clients[i];
		if (client.nick == old) {
			client.nick = _new;
			break;
		}
	}

	m_text->SetString("");
	for (auto& client : m_clients) {
		if (&client == m_me) {
			m_text->SetDrawingColor(glm::vec4(117, 148, 132, 255) / 255.f);
			m_text->AddString(client.nick + "\n");
			m_text->SetDrawingColor(glm::vec4(1.0f));
		}
		else {
			m_text->AddString(client.nick + "\n");
		}
	}
}

void ClientList::AddClient(ClientData nick)
{
	m_clients.emplace_back(nick);
	m_text->SetString("");
	for (auto& client : m_clients) {
		if (&client == m_me) { //for some reason this doesnt work, I'm propably dumb
			m_text->SetDrawingColor(glm::vec4(117, 148, 132, 255) / 255.f);
			m_text->AddString(client.nick + "\n");
			m_text->SetDrawingColor(glm::vec4(1.0f));
		}
		else {
			m_text->AddString(client.nick + "\n");
		}
	}
}

void ClientList::RemoveClient(ClientData nick)
{
	for (int i = 0; i < m_clients.size(); i++) {
		auto& client = m_clients[i];
		if (client.nick == nick.nick) {
			m_clients.erase(m_clients.begin() + i);
			break;
		}
	}

	m_text->SetString("");
	for (auto& client : m_clients) {
		if (&client == m_me) {
			m_text->SetDrawingColor(glm::vec4(117, 148, 132, 255) / 255.f);
			m_text->AddString(client.nick + "\n");
			m_text->SetDrawingColor(glm::vec4(1.0f));
		}
		else {
			m_text->AddString(client.nick + "\n");
		}
	}
}

bool ClientList::DoesExist(std::string nick)
{
	for (auto& client : m_clients) {
		if (client.nick == nick)
			return true;
	}
	return false;
}
