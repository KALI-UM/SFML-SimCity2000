#include "pch.h"
#include "SimCityGameSystem.h"
#include "Scene_SimCityInGame.h"
#include "TileResTable.h"
#include "TileModel.h"
#include "BuildingGenerator.h"
#include "PowerPlantBuilding.h"
#include "NormalBuilding.h"

SimCityGameSystem::SimCityGameSystem(TileModel* model)
	:mcv_Model(model)
{
}

SimCityGameSystem::~SimCityGameSystem()
{
}

bool SimCityGameSystem::Initialize()
{
	SetTileSet();

	for (int zone = 0; zone < (int)ZoneType::None; zone++)
	{
		m_BuildingGenerator.push_back(BuildingGenerator((ZoneType)zone));
		m_BuildingGenerator[zone].SetGetBuildPosFunc(std::bind(&SimCityGameSystem::GetBuildPossiblePos, this, (ZoneType)zone, std::placeholders::_1, std::placeholders::_2));
		m_BuildingGenerator[zone].SetBuildFunc(std::bind(&SimCityGameSystem::BuildNoneBuilding, this, (ZoneType)zone, std::placeholders::_1, std::placeholders::_2));
		m_BuildingGenerator[zone].Initialize();
	}

	m_ElecSupply = std::vector<std::vector<int>>(mcv_Model->m_CellCount.y, std::vector<int>(mcv_Model->m_CellCount.x, 0));
	m_BuildingMap = std::vector<std::vector<Building*>>(mcv_Model->m_CellCount.y, std::vector<Building*>(mcv_Model->m_CellCount.x, nullptr));
	m_ZoneInfos = std::vector<std::vector<ZoneType>>(mcv_Model->m_CellCount.y, std::vector<ZoneType>(mcv_Model->m_CellCount.x, ZoneType::None));

	m_ElecGroup.resize(1);
	return false;
}

void SimCityGameSystem::Reset()
{
}

void SimCityGameSystem::Update(float dt)
{
	if (m_CurrStatus == GameStatus::Pause)
		return;

	for (int zone = 0; zone < (int)ZoneType::None; zone++)
	{
		m_BuildingGenerator[zone].Update();
	}
}

void SimCityGameSystem::SetCurrTileSet(ButtonSet set)
{
	m_CurrTileSet = set;
}

void SimCityGameSystem::BuildSomething(std::list<CellIndex>& tiles)
{
	TileType type = GetCurrTileSet().type;

	if (type == TileType::Zone)
	{
		BuildZone(tiles);
		return;
	}
	else
	{
		if (type == TileType::Building)
		{
			BuildBuilding(tiles);
		}
		else if (type == TileType::Powerline)
		{
			BuildPowerlink(tiles);
		}

		for (auto& currIndex : tiles)
		{
			m_ZoneInfos[currIndex.y][currIndex.x] = ZoneType::None;
		}
	}

	mcv_Model->SetTiles(tiles, GetCurrTileSet().type, GetCurrTileSet().subtype, GetCurrTileSet().name, GetCurrTileSet().lotSize != sf::Vector2u(1, 1));
}

void SimCityGameSystem::BuildZone(std::list<CellIndex>& tiles)
{
	std::list<CellIndex> possibleTiles;
	for (auto& currIndex : tiles)
	{
		auto& currTileInfo = mcv_Model->GetTileInfo(TileDepth::OnGround, currIndex);
		//if (currTileInfo.zone != ZoneType::None || currTileInfo.type != TileType::None || currTileInfo.subtype == "powerline")
		if (currTileInfo.zone != ZoneType::None || currTileInfo.type != TileType::None)
			continue;

		m_ZoneInfos[currIndex.y][currIndex.x] = ZoneType::Residential;
		possibleTiles.push_back(currIndex);
	}
	mcv_Model->SetTiles(possibleTiles, TileType::Zone, "", Tile::GetZoneToName(ZoneType::Residential));
}

void SimCityGameSystem::BuildBuilding(std::list<CellIndex>& tiles)
{
	Building* building = nullptr;

	switch (m_CurrTileSet)
	{
	case ButtonSet::Powerplant:
		building = BuildPowerPlant(tiles);
		break;
	default:
		break;
	}
	if (!building)return;

	building->INITIALIZE();
	building->RESET();
	SCENE_MGR->GetCurrentScene()->AddGameObject(3, building);
	building->BuildBuilding(tiles);
	for (auto& currIndex : tiles)
	{
		m_BuildingMap[currIndex.y][currIndex.x] = building;
	}
	BuildPowerlink(tiles, building->GetBuildingInfo().buildingType == BuildingType::PowerPlant ? building->GetBuildingInfo().buildingId : -1);
}

void SimCityGameSystem::BuildNoneBuilding(ZoneType zone, std::list<CellIndex>& tiles, const TileResData& info)
{
	Building* building = new NormalBuilding();
	building->INITIALIZE();
	building->RESET();
	SCENE_MGR->GetCurrentScene()->AddGameObject(3, building);
	building->BuildBuilding(tiles);
	for (auto& currIndex : tiles)
	{
		m_BuildingMap[currIndex.y][currIndex.x] = building;
	}
	BuildPowerlink(tiles, -1);
	mcv_Model->SetTiles(tiles, Tile::GetTypeToEnum(info.type), info.subtype, info.name, info.lotSize != sf::Vector2u(1, 1));
}

PowerPlantBuilding* SimCityGameSystem::BuildPowerPlant(std::list<CellIndex>& tiles)
{
	PowerPlantBuilding* building = new PowerPlantBuilding();
	m_PowerPlantBuildings.push_back(building);
	return building;
}

void SimCityGameSystem::BuildPowerlink(std::list<CellIndex>& tiles, int powerplantId)
{
	for (auto& line : tiles)
	{
		if (m_ElecSupply[line.y][line.x] == 0)
			m_ElecSupply[line.y][line.x] = powerplantId;
	}

	std::vector<std::vector<bool>> visited = std::vector<std::vector<bool>>
		(mcv_Model->m_CellCount.y, std::vector<bool>(mcv_Model->m_CellCount.x, false));

	std::queue<CellIndex> q;
	for (auto& currIndex : tiles)
	{
		if (!visited[currIndex.y][currIndex.x])
			q.push(currIndex);

		while (!q.empty())
		{
			CellIndex currIndex_currIndex = q.front();
			q.pop();

			if (visited[currIndex_currIndex.y][currIndex_currIndex.x])
				continue;
			visited[currIndex_currIndex.y][currIndex_currIndex.x] = true;
			for (int i = 0; i < 4; i++)
			{
				CellIndex temp = currIndex_currIndex + Tile::d[i];
				if (!visited[temp.y][temp.x] && m_ElecSupply[temp.y][temp.x] == -1)
				{
					q.push(temp);
				}
				else if (!visited[temp.y][temp.x] && m_ElecSupply[temp.y][temp.x] > 0)
				{
					UpdatePowerlink(tiles, powerplantId);
					return;
				}
			}
		}
	}
	if (powerplantId == -1)
		mcv_Model->SetTiles(tiles, TileType::Other, "", "power_outage_lightning");
}

CellIndex SimCityGameSystem::GetBuildPossiblePos(ZoneType zone, std::list<CellIndex>& tiles, const TileResData& info) const
{
	std::vector<CellIndex> possiblePos;
	for (int j = 0; j < m_ZoneInfos.size(); ++j)
	{
		for (int i = 0; i < m_ZoneInfos[j].size(); ++i)
		{
			bool canBuild = true;
			for (auto& currIndex : tiles)
			{
				auto currIndex_curIndex = currIndex + sf::Vector2i(i, j);
				if (m_ZoneInfos[currIndex_curIndex.y][currIndex_curIndex.x] != zone ||
					!mcv_Model->IsPossibleToBuild(currIndex_curIndex, Tile::GetTypeToEnum(info.type), info.subtype))
				{
					canBuild = false;
					break;
				}
			}
			if (canBuild)
				possiblePos.push_back({ i,j });
		}
	}

	if (possiblePos.empty())
		return { -1,-1 };
	else
		return possiblePos[Utils::RandomRange(0, possiblePos.size() - 1)];
}

void SimCityGameSystem::DestroySomething(const CellIndex& tileIndex)
{
	auto& originTile = mcv_Model->GetTileInfo(TileDepth::OnGround, tileIndex);
	if (originTile.type == TileType::Building && originTile.subtype != "rubble")
	{
		DestroyBuilding(tileIndex);
	}
	else
	{
		std::list<CellIndex> destroy;
		destroy.push_back(tileIndex);
		mcv_Model->SetTiles(destroy, GetCurrTileSet().type, GetCurrTileSet().subtype, GetCurrTileSet().name, GetCurrTileSet().lotSize != sf::Vector2u(1, 1));
	}

	if (originTile.type == TileType::Powerline)
		DestroyPowerlink(tileIndex);
}

void SimCityGameSystem::DestroyBuilding(const CellIndex& tileIndex)
{
	Building* building = m_BuildingMap[tileIndex.y][tileIndex.x];
	if (!building) return;
	if (building->GetBuildingInfo().buildingType == BuildingType::PowerPlant)
	{
		m_PowerPlantBuildings.remove(dynamic_cast<PowerPlantBuilding*>(building));
	}

	std::list<CellIndex> destroy;
	for (auto& currIndex : building->GetBuildingInfo().position)
	{
		destroy.push_back(currIndex);
		m_BuildingMap[currIndex.y][currIndex.x] = nullptr;
		DestroyPowerlink(currIndex);
	}
	mcv_Model->SetTiles(destroy, GetCurrTileSet().type, GetCurrTileSet().subtype, GetCurrTileSet().name, GetCurrTileSet().lotSize != sf::Vector2u(1, 1));
	SCENE_MGR->GetCurrentScene()->RemoveGameObject(3, building);
}

void SimCityGameSystem::DestroyPowerlink(const CellIndex& tileIndex)
{
	m_ElecSupply[tileIndex.y][tileIndex.x] = 0;
	std::list<CellIndex> destroy;
	destroy.push_back(tileIndex);
	mcv_Model->SetTiles(destroy, TileType::Other, "", "");

	ResetPowerlink();
	for (auto& powerplant : m_PowerPlantBuildings)
	{
		std::queue<CellIndex> powerplantQue;
		for (auto& currIndex : powerplant->GetBuildingInfo().position)
		{
			m_ElecSupply[currIndex.y][currIndex.y] = powerplant->GetBuildingInfo().buildingId;
			powerplantQue.push(currIndex);
		}

		std::queue<CellIndex> powerlinkQue;
		while (!powerplantQue.empty())
		{
			CellIndex currIndex = powerplantQue.front();
			powerplantQue.pop();
			for (int i = 0; i < 4; i++)
			{
				CellIndex temp = currIndex + Tile::d[i];
				if (m_ElecSupply[temp.y][temp.x] == -1)
					powerlinkQue.push(temp);
			}
		}

		while (!powerlinkQue.empty())
		{
			CellIndex currIndex = powerlinkQue.front();
			powerlinkQue.pop();

			if (m_ElecSupply[currIndex.y][currIndex.x] != -1)
				continue;
			m_ElecSupply[currIndex.y][currIndex.x] = powerplant->GetBuildingInfo().buildingId;
			for (int i = 0; i < 4; i++)
			{
				CellIndex temp = currIndex + Tile::d[i];
				if (m_ElecSupply[temp.y][temp.x] == -1)
					powerlinkQue.push(temp);
			}
		}
	}
	UpdatePowerlink();
}

void SimCityGameSystem::UpdatePowerlink(std::list<CellIndex>& tiles, int powerplantId)
{
	std::list<CellIndex> elecUpdate;
	std::vector<std::vector<bool>> visited = std::vector<std::vector<bool>>
		(mcv_Model->m_CellCount.y, std::vector<bool>(mcv_Model->m_CellCount.x, false));

	std::queue<CellIndex> q;
	for (auto& currIndex : tiles)
	{
		if (!visited[currIndex.y][currIndex.x])
			q.push(currIndex);

		while (!q.empty())
		{
			CellIndex currIndex_currIndex = q.front();
			q.pop();

			if (visited[currIndex_currIndex.y][currIndex_currIndex.x])
				continue;
			visited[currIndex_currIndex.y][currIndex_currIndex.x] = true;
			if (m_ElecSupply[currIndex_currIndex.y][currIndex_currIndex.x] == -1)
				elecUpdate.push_back(currIndex_currIndex);
			m_ElecSupply[currIndex_currIndex.y][currIndex_currIndex.x] = powerplantId;
			for (int i = 0; i < 4; i++)
			{
				CellIndex temp = currIndex_currIndex + Tile::d[i];
				if (!visited[temp.y][temp.x] && m_ElecSupply[temp.y][temp.x] == -1)
				{
					q.push(temp);
				}
			}
		}
	}
	mcv_Model->SetTiles(elecUpdate, TileType::Other, "", "");
}

void SimCityGameSystem::UpdatePowerlink()
{
	std::list<CellIndex> elecUpdate;
	for (int j = 0; j < m_ElecSupply.size(); j++)
	{
		for (int i = 0; i < m_ElecSupply[j].size(); i++)
		{
			if (m_ElecSupply[j][i] == -1)
				elecUpdate.push_back({ i,j });
		}
	}
	mcv_Model->SetTiles(elecUpdate, TileType::Other, "", "power_outage_lightning");
}

void SimCityGameSystem::ResetPowerlink()
{
	for (auto& tiles : m_ElecSupply)
	{
		for (auto& tile : tiles)
		{
			if (tile > 0)
				tile = -1;
		}
	}
}

int SimCityGameSystem::Find(int groupId)
{
	if (m_ElecGroup[groupId] == groupId)return groupId;
	else m_ElecGroup[groupId] = Find(m_ElecGroup[groupId]);
}

void SimCityGameSystem::Union(int groupId1, int groupId2)
{
	groupId1 = Find(groupId1);
	groupId2 = Find(groupId2);

	if (groupId1 > groupId2)
		std::swap(groupId1, groupId2);

	m_ElecGroup[groupId2] = groupId1;
}

void SimCityGameSystem::SetTileSet()
{
	TileInfo road;
	road.type = TileType::Road;
	road.subtype = "road";
	road.name = "road_1";
	road.lotSize = DATATABLE_TILERES->GetTileRes(road.type, road.subtype, road.name).lotSize;
	m_TileSet.insert({ ButtonSet::Road, road });

	TileInfo rail;
	rail.type = TileType::Rail;
	rail.subtype = "rail";
	rail.name = "rail_1";
	rail.lotSize = DATATABLE_TILERES->GetTileRes(rail.type, rail.subtype, rail.name).lotSize;
	m_TileSet.insert({ ButtonSet::Rail, rail });

	TileInfo powerline;
	powerline.type = TileType::Powerline;
	powerline.subtype = "powerline";
	powerline.name = "powerline_1";
	powerline.lotSize = DATATABLE_TILERES->GetTileRes(powerline.type, powerline.subtype, powerline.name).lotSize;
	m_TileSet.insert({ ButtonSet::Powerlink, powerline });

	TileInfo powerplant;
	powerplant.type = TileType::Building;
	powerplant.subtype = "power_plant";
	powerplant.name = "coal";
	powerplant.lotSize = DATATABLE_TILERES->GetTileRes(powerplant.type, powerplant.subtype, powerplant.name).lotSize;
	m_TileSet.insert({ ButtonSet::Powerplant, powerplant });

	TileInfo zone;
	zone.type = TileType::Zone;
	zone.subtype = "";
	zone.name = "zone_1";
	zone.lotSize = DATATABLE_TILERES->GetTileRes(zone.type, zone.subtype, zone.name).lotSize;
	m_TileSet.insert({ ButtonSet::Zone, zone });

	TileInfo destroy;
	destroy.type = TileType::Building;
	destroy.subtype = "rubble";
	destroy.name = "rubble_1";
	destroy.lotSize = DATATABLE_TILERES->GetTileRes(destroy.type, destroy.subtype, destroy.name).lotSize;
	m_TileSet.insert({ ButtonSet::Destroy, destroy });

}
