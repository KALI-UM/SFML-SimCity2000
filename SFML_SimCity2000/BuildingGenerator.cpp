#include "pch.h"
#include "BuildingGenerator.h"
#include "TileResTable.h"

std::vector<std::list<CellIndex>> BuildingGenerator::m_lotSet;

BuildingGenerator::BuildingGenerator(ZoneType type)
	:m_Zone(type)
{
	SetLotSet();
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
		const auto& res = DATATABLE_TILERES->GetTileRes(m_Zone, sf::Vector2u(lotSize, lotSize), m_RandomValue);
		if (res.name == "")
			return;
		
		//GetBuildPossiblePos
		sf::Vector2i offset = m_GetCanBuildPosFunc(m_lotSet[lotSize], res);
		if (offset == sf::Vector2i(-1, -1))
			return;

		std::list<CellIndex> tiles;
		for (auto& t : m_lotSet[lotSize])
		{
			tiles.push_back(t + offset);
		}

		//BuildNoneBuilding
		m_GenerateBuildingFunc(tiles, res);
		m_RandomValue = 0;
	}
}

void BuildingGenerator::SetLotSet()
{
	if (m_lotSet.empty())
	{
		m_lotSet.resize(5);

		for (int lot = 1; lot <= 4; lot++)
		{
			for (int j = 0; j < lot; j++)
			{
				for (int i = 0; i < lot; i++)
				{
					m_lotSet[lot].push_back({ i,j });
				}
			}
		}
	}
}
