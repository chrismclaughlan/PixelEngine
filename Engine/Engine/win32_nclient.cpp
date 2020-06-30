#include "win32_nclient.h"
#include "exception.h"

NClient::NClient()
{
	localAddr = {};
	incomingAddr = {};

	int32 res;
	WSADATA wsaData;
	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != NO_ERROR)
	{
		THROW_EXCEPTION("WSAStartup() failed");  // TODO implement WSAGetLastError()
	}

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET)
	{
		THROW_EXCEPTION("socket() invalid");  // TODO implement WSAGetLastError()
	}

	uint64 Blocking = 0;
	uint64 nonBlocking = 1;
	res = ioctlsocket(sock, FIONBIO, &nonBlocking);
	if (res == SOCKET_ERROR)
	{
		THROW_EXCEPTION("ioctlsocket() error");  // TODO implement WSAGetLastError()
	}
}

NClient::~NClient()
{
	WSACleanup();
}

void NClient::connect(const char* sIP, const uint16 sPort)
{
	memset((char*)&this->incomingAddr, 0, sizeof(this->incomingAddr));
	incomingAddr.sin_family = AF_INET;
	incomingAddr.sin_port = htons(sPort);
	int32 result = inet_pton(AF_INET, sIP, &incomingAddr.sin_addr.s_addr);
	if (result != 1)
	{
		THROW_EXCEPTION("Not a valid IPv4 or IPv6 address");
	}
	else if (result != 1)
	{
		THROW_EXCEPTION("Error converting server IP address");  // TODO implement WSAGetLastError()
	}

	// Check connection
	ping();
}

bool NClient::isOnline()
{
	return true;
}