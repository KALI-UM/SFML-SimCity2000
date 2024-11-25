#include "pch.h"
#include "Tile.h"

const CellIndex Tile::d[8] = { {-1,0}, {1,0}, {0,-1},{0,1} ,{ -1,-1 },{1,-1},{-1,1},{1,1} };
std::vector<std::list<CellIndex>> Tile::lotSet;

void Tile::Initialize()
{
	SetLotSet();
}

std::string Tile::GetTypeToString(TileType type)
{
	switch (type)
	{
	case TileType::None:
		return "";
		break;
	case TileType::Terrain:
		return "terrain";
		break;
	case TileType::Building:
		return "building";
		break;
	case TileType::Zone:
		return "zone";
		break;
	case TileType::Road:
		return "road";
		break;
	case TileType::Rail:
		return "rail";
		break;
	case TileType::Powerline:
		return "powerline";
		break;
	case TileType::Highway:
		return "highway";
		break;
	case TileType::Underground:
		return "underground";
		break;
	case TileType::Other:
		return "other";
		break;
	}
	return "";
}

TileType Tile::GetTypeToEnum(const std::string& type)
{
	if (type == "")
		return TileType::None;
	if (type == "terrain")
		return TileType::Terrain;
	if (type == "building")
		return TileType::Building;
	if (type == "zone")
		return TileType::Zone;
	if (type == "road")
		return TileType::Road;
	if (type == "rail")
		return TileType::Rail;
	if (type == "highway")
		return TileType::Highway;
	if (type == "powerline")
		return TileType::Powerline;
	if (type == "underground")
		return TileType::Underground;
	if (type == "other")
		return TileType::Other;

	return TileType::None;
}

std::string Tile::GetZoneToString(ZoneType type)
{
	switch (type)
	{
	case ZoneType::Residential:
		return "residential";
		break;
	case ZoneType::Commercial:
		return "commercial";
		break;
	case ZoneType::Industrial:
		return "industrial";
		break;
	case ZoneType::Military:
		return "military";
		break;
	case ZoneType::Airport:
		return "airport";
		break;
	case ZoneType::Seaport:
		return "seaport";
		break;
	case ZoneType::None:
		return "";
		break;
	}
	return "";
}

ZoneType Tile::GetZoneToEnum(const std::string& type)
{
	if (type == "")
		return ZoneType::None;
	if (type == "residential")
		return ZoneType::Residential;
	if (type == "commercial")
		return ZoneType::Commercial;
	if (type == "industrial")
		return ZoneType::Industrial;
	if (type == "military")
		return ZoneType::Military;
	if (type == "airport")
		return ZoneType::Airport;
	if (type == "seaport")
		return ZoneType::Seaport;

	return ZoneType::None;
}

std::string Tile::GetZoneToName(ZoneType type)
{
	switch (type)
	{
	case ZoneType::Residential:
		return "zone_1";
		break;
	case ZoneType::Commercial:
		return "zone_3";
		break;
	case ZoneType::Industrial:
		return "zone_5";
		break;
	case ZoneType::Military:
		return "zone_7";
		break;
	case ZoneType::Airport:
		return "zone_8";
		break;
	case ZoneType::Seaport:
		return "zone_9";
		break;
	case ZoneType::None:
		return "";
		break;
	}
	return "";
}

ZoneType Tile::GetNameToZone(const NAME& name)
{
	if (name == "")
		return ZoneType::None;
	if (name == "zone_1")
		return ZoneType::Residential;
	if (name == "zone_3")
		return ZoneType::Commercial;
	if (name == "zone_5")
		return ZoneType::Industrial;
	if (name == "zone_7")
		return ZoneType::Military;
	if (name == "zone_8")
		return ZoneType::Airport;
	if (name == "zone_9")
		return ZoneType::Seaport;

	return ZoneType::None;
}

void Tile::SetLotSet()
{
	if (lotSet.empty())
	{
		lotSet.resize(5);

		for (int lot = 1; lot <= 4; lot++)
		{
			for (int j = 0; j > -lot; j--)
			{
				for (int i = 0; i < lot; i++)
				{
					lotSet[lot].push_back({ i,j });
				}
			}
		}
	}
}





