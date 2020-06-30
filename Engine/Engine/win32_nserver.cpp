#include "win32_nserver.h"
#include "exception.h"

NServer::NServer(uint16 port)
{
	this->localAddr.sin_family = AF_INET;
	this->localAddr.sin_addr.s_addr = INADDR_ANY;
	this->localAddr.sin_port = htons(port);

	incomingAddr = {};

	int32 res;
	WSADATA wsaData;
	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != NO_ERROR)
	{
		THROW_EXCEPTION("WSAStartup() failed");  // TODO implement WSAGetLastError()
	}

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		THROW_EXCEPTION("socket() invalid");  // TODO implement WSAGetLastError()
	}

	res = bind(sock, (struct sockaddr*) & localAddr, sizeof(localAddr));
	if (res == SOCKET_ERROR)
	{
		THROW_EXCEPTION("bind() socket error");  // TODO implement WSAGetLastError()
	}

	uint64 Blocking = 0;
	uint64 nonBlocking = 1;
	res = ioctlsocket(sock, FIONBIO, &nonBlocking);
	if (res == SOCKET_ERROR)
	{
		THROW_EXCEPTION("ioctlsocket() error");  // TODO implement WSAGetLastError()
	}
}

NServer::~NServer()
{
	closesocket(sock);
	WSACleanup();
}

void NServer::listen()
{

}

bool NServer::isOnline()
{
	return true;
}