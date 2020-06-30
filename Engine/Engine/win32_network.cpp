#include "win32_network.h"
#include "exception.h"
#include "types.h"

#include <iostream>

// inet_pton
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

void Network::deliver(NETWORK::Packet& packet)
{
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
	NETWORK::Packet packet = { 0 };

	int32 server_addr_size = sizeof(incomingAddr);
	packet.numBytes = recvfrom(sock, packet.buffer, NETWORK::maxBufferSize, 0,
		(struct sockaddr*) & incomingAddr, &server_addr_size);
	if (packet.numBytes == SOCKET_ERROR)
	{
		THROW_EXCEPTION("recvfrom returned SOCKET_ERROR");
	}

	char buff[16];
	inet_ntop(incomingAddr.sin_family, &incomingAddr.sin_addr, buff, sizeof(buff));

	printf("Received packet from %s:%d\n", buff, ntohs(incomingAddr.sin_port));
	printf("Data: %s\n", packet.buffer);

	return packet;
}

// Returns time in ms for response: -1 for failure
double Network::ping()
{
	const int32 symbol = (int32)"P";

	NETWORK::Packet ping;
	ping.numBytes = NETWORK::maxBufferSize;
	memset(ping.buffer, symbol, NETWORK::maxBufferSize);
	deliver(ping);

	// wait for response
	NETWORK::Packet response;
	response = receive();

	// test
	for (int i = 0; i < NETWORK::maxBufferSize; i++)
	{
		if (response.buffer[i] != symbol)
		{
			std::cout << "\ni " << i;
			THROW_EXCEPTION("Buffer out != Buffer in");  // test
			return -1;
		}
	}

	return 0.1;  // TODO
}