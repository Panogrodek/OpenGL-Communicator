#include "plpch.h"
#include "Networking/TCPConnection.hpp"

using namespace pl;

TCPConnection::TCPConnection(Socket socket, IPEndpoint endpoint) :
	socket(socket), m_endpoint(endpoint)
{
	m_stringRepresentation = "[" + endpoint.GetIpString();
	m_stringRepresentation += ":" + std::to_string(endpoint.GetPort()) + "]";
}

void TCPConnection::Close()
{
	socket.Close();
	pmIncoming.Clear();
	pmOutgoing.Clear();
}

std::string TCPConnection::ToString()
{
	return m_stringRepresentation;
}
