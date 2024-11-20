#include "pch.h"
#include "SimCityGameSystem.h"
#include "Scene_SimCityInGame.h"
#include "BuildingGenerator.h"
#include "TileModel.h"
#include "PowerPlantBuilding.h"

SimCityGameSystem::SimCityGameSystem(TileModel* model)
	:mcv_Model(model)
{
}

SimCityGameSystem::~SimCityGameSystem()
{
}

bool SimCityGameSystem::Initialize()
{
	for (int zone = 0; zone < (int)ZoneType::None; zone++)
	{
		m_BuildingGenerator.push_back(BuildingGenerator((ZoneType)zone));
	}

	m_ElecSupply = std::vector<std::vector<int>>(mcv_Model->m_CellCount.y, std::vector<int>(mcv_Model->m_CellCount.x, 0));
	m_ElecGroupId = std::vector<std::vector<int>>(mcv_Model->m_CellCount.y, std::vector<int>(mcv_Model->m_CellCount.x, 0));
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


}

void SimCityGameSystem::PushToEmptyZone(const CellIndex& tileIndex, ZoneType zone)
{
	m_BuildingGenerator[(int)zone].PushToEmptyZone(tileIndex);
}

void SimCityGameSystem::RemoveToEmptyZone(const CellIndex& tileIndex, ZoneType zone)
{
	m_BuildingGenerator[(int)zone].RemoveToEmptyZone(tileIndex);
}

void SimCityGameSystem::BuildBuilding(std::list<CellIndex>& tiles, Building* building)
{
	PowerPlantBuilding* isPowerPlant = dynamic_cast<PowerPlantBuilding*>(building);

	building->BuildBuilding(tiles);
	if (isPowerPlant != nullptr)
	{

	}
	else
	{
		BuildPowerlink(tiles);
	}
}

void SimCityGameSystem::BuildPowerPlant(std::list<CellIndex>& tiles)
{
	PowerPlantBuilding* building = new PowerPlantBuilding();
	building->INITIALIZE();
	building->RESET();
	SCENE_MGR->GetCurrentScene()->AddGameObject(3, building);
	m_PowerPlantBuilding.push_back(building);

	building->BuildBuilding(tiles);
	for (auto& currIndex : tiles)
	{
		m_ElecSupply[currIndex.y][currIndex.x] = building->GetBuildingInfo().buildingId;
	}
	BuildPowerlink(tiles, building->GetBuildingInfo().buildingId);
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

void SimCityGameSystem::DestroyPowerlink(std::list<CellIndex>& tiles)
{
	std::list<CellIndex> elecUpdate;
	
	for (auto& line : tiles)
	{
		m_ElecSupply[line.y][line.x] = 0;
	}

	ResetPowerlink();
	for (auto& powerplant : m_PowerPlantBuilding)
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
