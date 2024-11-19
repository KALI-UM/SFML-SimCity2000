#include "pch.h"
#include "Tile.h"

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





