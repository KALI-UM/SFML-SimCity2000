#include "pch.h"
#include "BuildingGenerator.h"
#include "TileResTable.h"


BuildingGenerator::BuildingGenerator(ZoneType type)
	:m_Zone(type)
{
}

BuildingGenerator::~BuildingGenerator()
{
}

bool BuildingGenerator::Initialize()
{
	return false;
}

void BuildingGenerator::Reset()
{
	m_RandomValue = 0;
}

void BuildingGenerator::Update()
{
	m_RandomValue += Utils::RandomRange(0, 5);
	if (m_RandomValue > 100)
	{
		int lotSize = Utils::RandomRange(1, 4);

		//GetBuildPossiblePos
		sf::Vector2i offset = m_GetCanBuildPosFunc(Tile::lotSet[lotSize]);
		if (offset == sf::Vector2i(-1, -1))
			return;

		const auto& res = DATATABLE_TILERES->GetTileRes(m_Zone, sf::Vector2u(lotSize, lotSize), m_RandomValue);
		if (res.name == "")
			return;

		std::list<CellIndex> tiles;
		for (auto& t : Tile::lotSet[lotSize])
		{
			tiles.push_back(t + offset);
		}

		//BuildNoneBuilding
		m_GenerateBuildingFunc(tiles, res);
		m_RandomValue = 0;
	}
}
