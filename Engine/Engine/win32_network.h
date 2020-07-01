#pragma once
#include "types.h"
#include <winsock2.h>
#include <assert.h>

#include <cstring>

// inet_pton
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

namespace NETWORK
{
	static constexpr const int32 maxBufferSize = 100;
	//static constexpr const char* pingString = "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP";
	static constexpr const char* pingString = "PING_PING1PING_PING2PING_PING3PING_PING4PING_PING5PING_PING6PING_PING7PING_PING8PING_PING9PING_PING";

	struct Packet
	{
		int32 numBytes = 0;
		int8 buffer[maxBufferSize];

		Packet() : numBytes(0)
		{
			memset(buffer, 0, maxBufferSize);
		}
		
		Packet(const int8* _buffer)
		{
			strncpy_s(buffer, _buffer, _TRUNCATE);
			numBytes = std::strlen(buffer);
		}

		Packet(int32 _numBytes, int8* _buffer)
			: numBytes(_numBytes)
		{
			//strncpy_s(buffer, _numBytes, _buffer, _TRUNCATE);
			strncpy_s(buffer, _buffer, _TRUNCATE);
		}

		Packet& operator=(const Packet& a)
		{
			numBytes = a.numBytes;
			//strncpy_s(buffer, a.numBytes, a.buffer, _TRUNCATE);
			strncpy_s(buffer, a.buffer, _TRUNCATE);
			return *this;
		}

		bool operator==(const Packet& a) const
		{
			return (strcmp(buffer, a.buffer) == 0) ? true : false;
		}

		bool contains(const char* a)
		{
			return (strcmp(buffer, a) == 0) ? true : false;
		}
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
	virtual void receive(NETWORK::Packet& packet);
	virtual double ping();

	virtual bool isOnline() = 0;
};