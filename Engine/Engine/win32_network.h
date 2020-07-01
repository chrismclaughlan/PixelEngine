#pragma once
#include "types.h"
#include <winsock2.h>
#include <assert.h>

#include <string>
#include <iostream>

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
			assert(numBytes < maxBufferSize);
		}
		
		Packet(const std::string _buffer)
		{
			if (_buffer.length() >= maxBufferSize)
				numBytes = maxBufferSize - 1;
			else
				numBytes = _buffer.length();
			_buffer.copy(buffer, numBytes);
			assert(numBytes < maxBufferSize);
		}

		Packet(const int8* _buffer)
		{
			strncpy_s(buffer, _buffer, _TRUNCATE);
			numBytes = std::strlen(buffer);
			assert(numBytes < maxBufferSize);
		}

		Packet(const int8* _buffer, const int32 _numBytes)
			: numBytes(_numBytes)
		{
			//strncpy_s(buffer, _numBytes, _buffer, _TRUNCATE);
			strncpy_s(buffer, _buffer, _TRUNCATE);
			assert(numBytes < maxBufferSize);
		}

		Packet& operator=(const Packet& a)
		{
			numBytes = a.numBytes;
			//strncpy_s(buffer, a.numBytes, a.buffer, _TRUNCATE);
			strncpy_s(buffer, a.buffer, _TRUNCATE);
			assert(numBytes < maxBufferSize);
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
	virtual bool receive(NETWORK::Packet& packet);
	virtual double ping();

	virtual bool isOnline() = 0;

	void coutIncomingAddr() const noexcept
	{
		char buff[16];
		inet_ntop(incomingAddr.sin_family, &incomingAddr.sin_addr, buff, sizeof(buff));
		std::cout << "Received packet from " << buff << ":" << ntohs(incomingAddr.sin_port);
	}
};