#include "win32_network.h"
#include "exception.h"
#include "types.h"
#include "defines.h"

#include <iostream>

// inet_pton
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

void Network::deliver(NETWORK::Packet& packet)
{
	if (packet.numBytes > NETWORK::maxBufferSize)
	{
		THROW_EXCEPTION("packet larger than max buffer size");
	}

	int32 res, flags = 0;
	res = sendto(sock, packet.buffer, packet.numBytes, flags,
		(struct sockaddr*) & incomingAddr, (int32)sizeof(incomingAddr));
	if (res == SOCKET_ERROR)
	{
		THROW_EXCEPTION("sendto() failed");
	}
}

NETWORK::Packet& Network::receive()
{
	int8 buffer[NETWORK::maxBufferSize];
	int32 server_addr_size = sizeof(incomingAddr);
	int32 numBytes = recvfrom(sock, buffer, NETWORK::maxBufferSize, 0,
		(struct sockaddr*) & incomingAddr, &server_addr_size);

	NETWORK::Packet packet(numBytes, buffer);

	if (packet.numBytes == SOCKET_ERROR)
	{
		int32 WSALastError = WSAGetLastError();
		if (WSALastError == WSAEWOULDBLOCK)
		{
			return packet;
		}
		else
		{
			THROW_EXCEPTION("recvfrom returned SOCKET_ERROR");
		}
	}

#if DISPLAY_DEBUG_CONSOLE
	char buff[16];
	inet_ntop(incomingAddr.sin_family, &incomingAddr.sin_addr, buff, sizeof(buff));
	printf("Received packet from %s:%d\n", buff, ntohs(incomingAddr.sin_port));
	printf("Data: %s\n", packet.buffer);
#endif

	//if (packet == NETWORK::pingString)
	if (packet.contains(NETWORK::pingString))
	{
		// return ping
		deliver(packet);
	}

	return packet;
}

// Returns time in ms for response: -1 for failure
double Network::ping()
{
	std::cout << "Ping sent\n";

	NETWORK::Packet ping(NETWORK::pingString);
	//strcpy_s(ping.buffer, NETWORK::maxBufferSize, NETWORK::pingString);
	//ping.numBytes = std::strlen(ping.buffer);
	deliver(ping);

	// wait for response
	NETWORK::Packet response;
	response = receive();
	if (response == ping)
	{
		std::cout << "Ping received\n";
	}

	return 0.1;  // TODO
}