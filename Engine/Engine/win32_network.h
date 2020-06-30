#pragma once
#include "types.h"
#include <winsock2.h>

// inet_pton
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

namespace NETWORK
{
	static constexpr const int32 maxBufferSize = 512;

	struct Packet
	{
		int32 numBytes;
		int8 buffer[maxBufferSize];
	};
}

class Network
{
protected:
	SOCKET sock;
	sockaddr_in localAddr;
	sockaddr_in incomingAddr;
public:
	virtual void deliver(NETWORK::Packet& packet);
	virtual NETWORK::Packet& receive();
	virtual double ping();

	virtual bool isOnline() = 0;
};