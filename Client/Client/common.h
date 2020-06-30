#pragma once

namespace NETWORK
{
	namespace DEFAULTS
	{
		const int8 addr[] = "127.0.0.1";
		const uint16 port = 9999;
		const uint32 buffer_size = 512;
		const uint32 in_buffer_size = 8;  // wasd etc.
	}  // namespace Defaults
}

namespace GAME
{
	namespace DEFAULTS
	{

	}

	namespace STATES
	{
		const int8 running = 0x80;	// 1|XXXXXXX
	}
}