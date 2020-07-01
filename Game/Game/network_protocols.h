#pragma once
#include "Engine/types.h"
#include <vector>

namespace NETWORK
{
	// TODO more efficient method?

	struct PacketData
	{
		struct Header
		{
			enum Type : uint8
			{
				Invalid = 0,
				Join = 1 << 1,
				Leave = 1 << 2,
				Ping = 1 << 3,
				Data = 1 << 4,
			};
			Type type;
		};
		Header header;

		struct GameData
		{
			uint8 gridSizeX = 0;
			uint8 gridSizeY = 0;
			std::vector<uint8> gridData;
		};
		GameData gameData;

		struct PingData
		{
			// TODO safe?
			double startTime;
		};
		PingData pingData;

		//PacketData& PACKAGE_JOIN()
		//{
		//	header.type = Header::Type::Join;
		//	return *this;
		//}

		//PacketData& PACKAGE_LEAVE()
		//{
		//	header.type = Header::Type::Leave;
		//	return *this;
		//}
		//
		//PacketData& PACKAGE_PING()
		//{
		//	header.type = Header::Type::Ping;
		//	return *this;
		//}
		//		
		//PacketData& PACKAGE_DATA(int32 x, int32 y, std::vector<uint8> grid)
		//{
		//	header.type = Header::Type::Data;
		//	gameData.gridSizeX = x;
		//	gameData.gridSizeY = y;
		//	gameData.gridData = grid;
		//	return *this;
		//}
	};
}