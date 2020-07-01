#include "win32_network.h"
#include "exception.h"
#include "types.h"
#include "defines.h"
#include "win32_stopwatch.h"

#include <iostream>

// inet_pton
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

void Network::deliver(NETWORK::Packet& packet)
{
	if (packet.numBytes >= NETWORK::maxBufferSize)
	{
		packet.numBytes = NETWORK::maxBufferSize - 1;
	}

	int32 res, flags = 0;
	res = sendto(sock, packet.buffer, packet.numBytes, flags,
		(struct sockaddr*) & incomingAddr, (int32)sizeof(incomingAddr));
	if (res == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError();
		THROW_EXCEPTION("sendto() failed");
	}
}

bool Network::receive(NETWORK::Packet& packet)
{
	int32 server_addr_size = sizeof(incomingAddr);
	packet.numBytes = recvfrom(sock, packet.buffer, sizeof(packet.buffer) - 1, 0,
		(struct sockaddr*) & incomingAddr, &server_addr_size);

	if (packet.numBytes == SOCKET_ERROR)
	{
		int32 WSALastError = WSAGetLastError();
		if (WSALastError == WSAEWOULDBLOCK)
		{
			return false;
		}
		else
		{
			std::cout << WSAGetLastError();
			THROW_EXCEPTION("recvfrom returned SOCKET_ERROR");
		}
	}

	//packet.buffer[NETWORK::maxBufferSize - 1] = 0;

//#if DISPLAY_DEBUG_CONSOLE
	//char buff[16];
	//inet_ntop(incomingAddr.sin_family, &incomingAddr.sin_addr, buff, sizeof(buff));
	//printf("Received packet from %s:%d\n", buff, ntohs(incomingAddr.sin_port));
	//printf("Data: %s\n", packet.buffer);
//#endif

	return true;
}

// Returns time in ms for response: -1 for failure
// TODO multithreading?
double Network::ping()
{
	double time = -1;
	Stopwatch stopwatch;

	NETWORK::Packet ping(NETWORK::pingString);
	deliver(ping);
	stopwatch.start();
	std::cout << "Ping sent\n";

	// wait for response
	NETWORK::Packet response;
	receive(response);
	if (response == ping)
	{
		time = stopwatch.stop(TIME::MILISECONDS_MULTIPLYER);
		fprintf(stdout, "Ping received after: %fms\n", time);
	}

	return time;
}