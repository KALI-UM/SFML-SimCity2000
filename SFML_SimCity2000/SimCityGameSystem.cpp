#include "pch.h"
#include "SimCityGameSystem.h"
#include "Scene_SimCityInGame.h"
#include "TileResTable.h"
#include "TileModel.h"
#include "BuildingGenerator.h"
#include "PowerPlantBuilding.h"
#include "EducationBuilding.h"
#include "PublicServiceBuilding.h"
#include "RecreationBuilding.h"
#include "NormalBuilding.h"

SimCityGameSystem::SimCityGameSystem(TileModel* model)
	:mcv_Model(model)
{
	Tile::Initialize();
}

SimCityGameSystem::~SimCityGameSystem()
{
}

bool SimCityGameSystem::Initialize()
{
	SetTileSet();
	SetMenuSet();

	for (int zone = 0; zone < (int)ZoneType::None; zone++)
	{
		m_BuildingGenerator.push_back(BuildingGenerator((ZoneType)zone));
		m_BuildingGenerator[zone].SetGetBuildPosFunc(std::bind(&SimCityGameSystem::GetBuildPossiblePos, this, (ZoneType)zone, std::placeholders::_1));
		m_BuildingGenerator[zone].SetBuildFunc(std::bind(&SimCityGameSystem::BuildAutoBuilding, this, std::placeholders::_1, std::placeholders::_2));
		m_BuildingGenerator[zone].Initialize();
	}

	m_ElecSupply = std::vector<std::vector<int>>(mcv_Model->m_CellCount.y, std::vector<int>(mcv_Model->m_CellCount.x, 0));
	m_RoadSupply = std::vector<std::vector<int>>(mcv_Model->m_CellCount.y, std::vector<int>(mcv_Model->m_CellCount.x, 0));
	m_BuildingMap = std::vector<std::vector<Building*>>(mcv_Model->m_CellCount.y, std::vector<Building*>(mcv_Model->m_CellCount.x, nullptr));
	m_ZoneInfos = std::vector<std::vector<ZoneType>>(mcv_Model->m_CellCount.y, std::vector<ZoneType>(mcv_Model->m_CellCount.x, ZoneType::None));

	LoadTileDepthFile();
	return false;
}

void SimCityGameSystem::Reset()
{

}

void SimCityGameSystem::Update(float dt)
{
	while (!m_WantsToAddBuildings.empty())
	{
		auto building = m_WantsToAddBuildings.front();
		m_WantsToAddBuildings.pop();
		m_Buildings.push_back(building);
		SCENE_MGR->GetCurrentScene()->AddGameObject(2, building);
	}

	if (m_CurrStatus == GameStatus::Pause)
		return;

	auto nowTime = FRAMEWORK->GetRealTime();
	if (m_LastUpdate + (m_UpdateCycle / m_PlaySpeed) > nowTime)
	{
		for (int zone = 0; zone < (int)ZoneType::None; zone++)
		{
			m_BuildingGenerator[zone].Update();
		}

		m_LastUpdate = nowTime;
	}
}

void SimCityGameSystem::LoadTileDepthFile()
{
	LoadTerrainDepth();
	LoadOnGroundDepth();
}

void SimCityGameSystem::LoadTerrainDepth()
{
	rapidcsv::Document doc("datatables/tileInfo_depth1.csv", rapidcsv::LabelParams(-1, -1));
	int row = doc.GetRowCount();
	int col = doc.GetColumnCount();

	std::list<CellIndex> zones[3];
	for (int j = 0; j < row; j++)
	{
		for (int i = 0; i < col; i++)
		{
			std::string celldata = doc.GetCell<std::string>(i, j);
			std::vector<std::string> strings;
			SetStringToVectorElements(celldata, strings);
			if (!strings.empty())
			{
				if (TileType::Zone == Tile::GetTypeToEnum(strings[0]))
				{
					ZoneType zone = Tile::GetNameToZone(strings[2]);
					if (zone == ZoneType::Residential)
						zones[0].push_back({ i,j });
					else if (zone == ZoneType::Commercial)
						zones[1].push_back({ i,j });
					else if (zone == ZoneType::Industrial)
						zones[2].push_back({ i,j });

				}
				else
				{
					if (strings[0] == "")continue;
					BuildRawThing({ i,j }, DATATABLE_TILERES->GetTileRes(strings[0], strings[1], strings[2]));
				}
			}
		}
	}

	SetZone(zones[0], ZoneType::Residential);
	SetZone(zones[1], ZoneType::Commercial);
	SetZone(zones[2], ZoneType::Industrial);
}

void SimCityGameSystem::LoadOnGroundDepth()
{
	rapidcsv::Document doc("datatables/tileInfo_depth2.csv", rapidcsv::LabelParams(-1, -1));
	int row = doc.GetRowCount();
	int col = doc.GetColumnCount();

	std::list<CellIndex> roads;
	std::list<CellIndex> powerlines;
	std::list<CellIndex> rails;
	std::list<CellIndex> highways;

	for (int j = 0; j < row; j++)
	{
		for (int i = 0; i < col; i++)
		{
			std::string celldata = doc.GetCell<std::string>(i, j);
			std::vector<std::string> strings;
			SetStringToVectorElements(celldata, strings);
			if (!strings.empty() && strings[0] != "")
			{
				auto currType = Tile::GetTypeToEnum(strings[0]);
				if (TileType::Road == currType)
				{
					roads.push_back({ i,j });
					if (strings[1] == "rail")
						rails.push_back({ i,j });
					else if (strings[1] == "powerline")
						powerlines.push_back({ i,j });
				}
				else if (TileType::Rail == currType)
				{
					rails.push_back({ i,j });
					if (strings[1] == "powerline")
						powerlines.push_back({ i,j });
				}
				else if (TileType::Powerline == currType)
				{
					powerlines.push_back({ i,j });
				}
				else if (TileType::Highway == currType)
				{
					highways.push_back({ i,j });
				}
				else if (TileType::Building == currType)
				{
					std::list<CellIndex> temp;
					const TileResData& data = DATATABLE_TILERES->GetTileRes(strings[0], strings[1], strings[2]);
					for (auto& currIndex : Tile::lotSet[data.lotSize.x])
					{
						temp.push_back(currIndex + sf::Vector2i(i, j));
					}

					if (strings[1] == "")
					{
						BuildAutoBuilding(temp, DATATABLE_TILERES->GetTileRes(strings[0], strings[1], strings[2]));
					}
					else
					{
						for (int i = (int)Action::PowerPlant; i <= (int)Action::Stadium; i++)
						{
							auto& tileset = m_TileSet.find((Action)i)->second.info;
							if (strings[1] == tileset.subtype && strings[2] == tileset.name)
							{
								BuildBuilding(temp, (Action)i);
							}
						}
					}
				}
				else
				{
					BuildRawThing({ i,j }, DATATABLE_TILERES->GetTileRes(strings[0], strings[1], strings[2]));
				}
			}
		}
	}

	BuildRoad(roads);
	BuildPowerline(powerlines, -1);
}

void SimCityGameSystem::SaveTileDepthFile()
{
	SaveTerrainDepth();
	SaveOnGroundDepth();
}

void SimCityGameSystem::SaveTerrainDepth()
{
	rapidcsv::Document doc("", rapidcsv::LabelParams(-1, -1));
	for (int j = 0; j < mcv_Model->m_CellCount.x; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			const TileInfo& currInfo = mcv_Model->GetTileInfo(TileDepth::Terrain, { i,j });
			std::string strings;
			if (currInfo.ower != currInfo.index)
				strings = ",,," + std::to_string(currInfo.ower.x) + "," + std::to_string(currInfo.ower.y);
			else
				strings = Tile::GetTypeToString(currInfo.type) + "," + currInfo.subtype + "," + currInfo.name + ",,";

			doc.SetCell(i, j, strings);
		}
	}
	doc.Save("datatables/tileInfo_depth1.csv");
}

void SimCityGameSystem::SaveOnGroundDepth()
{
	rapidcsv::Document doc("", rapidcsv::LabelParams(-1, -1));
	for (int j = 0; j < mcv_Model->m_CellCount.x; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			const TileInfo& currInfo = mcv_Model->GetTileInfo(TileDepth::OnGround, { i,j });
			std::string strings;
			if (currInfo.ower != currInfo.index)
				strings = ",,," + std::to_string(currInfo.ower.x) + "," + std::to_string(currInfo.ower.y);
			else
				strings = Tile::GetTypeToString(currInfo.type) + "," + currInfo.subtype + "," + currInfo.name + ",,";
			doc.SetCell(i, j, strings);
		}
	}
	doc.Save("datatables/tileInfo_depth2.csv");
}

void SimCityGameSystem::SetCurrTileSet(Action set)
{
	m_CurrAction = set;
}

void SimCityGameSystem::BuildRawThing(const CellIndex& tileIndex, const TileResData& data)
{
	std::list<CellIndex> temp;
	temp.push_back(tileIndex);
	mcv_Model->SetTiles(temp, Tile::GetTypeToEnum(data.type), data.subtype, data.name);
}

void SimCityGameSystem::BuildSomething(std::list<CellIndex>& tiles, const Action& set)
{
	auto& tileset = m_TileSet.find(set)->second.info;
	TileType type = tileset.type;

	if (tiles.empty()) return;

	if (type == TileType::Zone)
	{
		BuildZone(tiles);
		SOUND_MGR->PlaySfx("sound/SFX/Pop.wav");
		return;
	}
	else
	{
		if (type == TileType::Building)
		{
			//BuildingType buildingType;
			//switch (set)
			//{
			//case Action::PowerPlant:
			//	buildingType = BuildingType::PowerPlant;
			//	break;
			//case Action::GradeSchool:
			//case Action::College:
			//case Action::Library:
			//	buildingType = BuildingType::Education;
			//	break;
			//case Action::PoliceStation:
			//case Action::FireStation:
			//case Action::Hospital:
			//	buildingType = BuildingType::PublicService;
			//	break;
			//case Action::Park:
			//case Action::LargePark:
			//case Action::Zoo:
			//case Action::Stadium:
			//	buildingType = BuildingType::Recreation;
			//	break;
			//}

			BuildBuilding(tiles, set);

			switch (set)
			{
			case Action::PowerPlant:
				SOUND_MGR->PlaySfx("sound/SFX/PCOAL.wav");
				break;
			case Action::GradeSchool:
				SOUND_MGR->PlaySfx("sound/SFX/School.wav");
				break;
			case Action::College:
				SOUND_MGR->PlaySfx("sound/SFX/CAMPUS.wav");
				break;
			case Action::Library:
				SOUND_MGR->PlaySfx("sound/SFX/LIBRARY.wav");
				break;
			case Action::PoliceStation:
			case Action::FireStation:
			case Action::Hospital:
				SOUND_MGR->PlaySfx("sound/SFX/POLICE.wav");
				break;
			case Action::Park:
			case Action::LargePark:
				SOUND_MGR->PlaySfx("sound/SFX/PARK.wav");
				break;
			case Action::Stadium:
				SOUND_MGR->PlaySfx("sound/SFX/CrowdYay.wav");
				break;
			case Action::Zoo:
				SOUND_MGR->PlaySfx("sound/SFX/Zoo.wav");
				break;
				break;
			}
		}
		else if (type == TileType::Powerline)
		{
			BuildPowerline(tiles);
			SOUND_MGR->PlaySfx("sound/SFX/Electricity.wav");
		}
		else if (type == TileType::Road)
		{
			BuildRoad(tiles);
			SOUND_MGR->PlaySfx("sound/SFX/Jackhammer.wav");
		}
	}
}

void SimCityGameSystem::BuildZone(std::list<CellIndex>& tiles)
{
	std::list<CellIndex> possibleTiles;
	for (auto& currIndex : tiles)
	{
		auto& currTileInfo = mcv_Model->GetTileInfo(TileDepth::OnGround, currIndex);
		//if (currTileInfo.zone != ZoneType::None || currTileInfo.type != TileType::None || currTileInfo.subtype == "powerline")
		if (currTileInfo.type == TileType::None
			|| (currTileInfo.type == TileType::Building && currTileInfo.subtype == "trees")
			|| currTileInfo.type == TileType::Powerline)
		{
			possibleTiles.push_back(currIndex);
		}
	}

	ZoneType zone;
	if (GetCurrAction() != Action::ZoneResidential &&
		GetCurrAction() != Action::ZoneCommercial &&
		GetCurrAction() != Action::ZoneIndustrial)
	{
		zone = ZoneType::None;
	}
	else
	{
		zone = Tile::GetNameToZone(GetCurrTileSet().name);
	}
	SetZone(possibleTiles, zone);
}

void SimCityGameSystem::BuildBuilding(std::list<CellIndex>& tiles, const Action& set)
{
	Building* building = nullptr;

	switch (set)
	{
	case Action::PowerPlant:
		building = BuildPowerPlant();
		break;
	case Action::GradeSchool:
	case Action::College:
	case Action::Library:
		building = BuildEducationBuilding();
		break;
	case Action::PoliceStation:
	case Action::FireStation:
	case Action::Hospital:
		building = BuildPublicServiceBuilding();
		break;
	case Action::Park:
	case Action::LargePark:
	case Action::Zoo:
	case Action::Stadium:
		building = BuildRecreationBuilding();
		break;
		break;
	}

	auto it = m_TileSet.find(set)->second.info;
	if (!building)return;

	building->INITIALIZE();
	building->RESET();
	m_WantsToAddBuildings.push(building);
	building->BuildBuilding(tiles);

	for (auto& currIndex : tiles)
	{
		m_BuildingMap[currIndex.y][currIndex.x] = building;
	}

	BuildPowerlink(tiles, building->GetBuildingInfo().buildingType == BuildingType::PowerPlant ? building->GetBuildingInfo().buildingId : -1);
	SetZone(tiles, ZoneType::None);


	mcv_Model->SetTiles(tiles, it.type, it.subtype, it.name, it.lotSize != sf::Vector2u(1, 1));
}

void SimCityGameSystem::BuildAutoBuilding(std::list<CellIndex>& tiles, const TileResData& info)
{
	Building* building = new NormalBuilding();
	building->INITIALIZE();
	building->RESET();
	m_WantsToAddBuildings.push(building);
	building->BuildBuilding(tiles);
	for (auto& currIndex : tiles)
	{
		m_BuildingMap[currIndex.y][currIndex.x] = building;
	}
	BuildPowerlink(tiles, -1);
	SetZone(tiles, ZoneType::None);
	mcv_Model->SetTiles(tiles, Tile::GetTypeToEnum(info.type), info.subtype, info.name, info.lotSize != sf::Vector2u(1, 1));
}

PowerPlantBuilding* SimCityGameSystem::BuildPowerPlant()
{
	PowerPlantBuilding* building = new PowerPlantBuilding();
	m_PowerPlantBuildings.push_back(building);
	return building;
}

EducationBuilding* SimCityGameSystem::BuildEducationBuilding()
{
	EducationBuilding* building = new EducationBuilding();
	return building;
}

PublicServiceBuilding* SimCityGameSystem::BuildPublicServiceBuilding()
{
	PublicServiceBuilding* building = new PublicServiceBuilding();
	return building;
}

RecreationBuilding* SimCityGameSystem::BuildRecreationBuilding()
{
	RecreationBuilding* building = new RecreationBuilding();
	return building;
}

void SimCityGameSystem::BuildPowerline(std::list<CellIndex>& tiles, int powerplantId)
{
	auto& tileset = m_TileSet.find(Action::PowerLine)->second.info;
	mcv_Model->SetTiles(tiles, tileset.type, tileset.subtype, tileset.name, tileset.lotSize != sf::Vector2u(1, 1));
	BuildPowerlink(tiles);
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
					UpdatePowerlink(tiles, m_ElecSupply[temp.y][temp.x]);
					return;
				}
			}
		}
	}
	if (powerplantId == -1)
		mcv_Model->SetTiles(tiles, TileType::Other, "", "power_outage_lightning");
}

void SimCityGameSystem::BuildRoad(std::list<CellIndex>& tiles)
{
	std::queue<CellIndex> q;
	for (auto& line : tiles)
	{
		if (m_RoadSupply[line.y][line.x] != 4)
		{
			m_RoadSupply[line.y][line.x] = 4;
			q.push(line);
		}
	}

	std::vector<std::vector<bool>> visited = std::vector<std::vector<bool>>
		(mcv_Model->m_CellCount.y, std::vector<bool>(mcv_Model->m_CellCount.x, false));

	while (!q.empty())
	{
		CellIndex currIndex = q.front();
		q.pop();
		int nextDepth = m_RoadSupply[currIndex.y][currIndex.x] - 1;

		if (visited[currIndex.y][currIndex.x] || nextDepth == 0)
			continue;
		visited[currIndex.y][currIndex.x] = true;
		for (int i = 0; i < 4; i++)
		{
			CellIndex currIndex_currIndex = currIndex + Tile::d[i];
			if (mcv_Model->IsValidTileIndex(currIndex_currIndex) && m_RoadSupply[currIndex_currIndex.y][currIndex_currIndex.x] < nextDepth)
			{
				m_RoadSupply[currIndex_currIndex.y][currIndex_currIndex.x] = nextDepth;
				q.push(currIndex_currIndex);
			}
		}
	}

	SetZone(tiles, ZoneType::None);

	auto& tileset = m_TileSet.find(Action::Road)->second.info;
	mcv_Model->SetTiles(tiles, tileset.type, tileset.subtype, tileset.name, tileset.lotSize != sf::Vector2u(1, 1));

}

CellIndex SimCityGameSystem::GetBuildPossiblePos(ZoneType zone, std::list<CellIndex>& tiles) const
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
				if (!mcv_Model->IsValidTileIndex(currIndex_curIndex))
				{
					canBuild = false;
					break;
				}

				if (m_ZoneInfos[currIndex_curIndex.y][currIndex_curIndex.x] != zone ||
					!mcv_Model->IsPossibleToBuild(currIndex_curIndex, TileType::Building, ""))
				{
					canBuild = false;
					break;
				}
			}

			if (canBuild)
			{
				bool elecSupply = false;
				bool roadSupply = false;
				for (auto& currIndex : tiles)
				{
					auto currIndex_curIndex = currIndex + sf::Vector2i(i, j);
					if (mcv_Model->IsValidTileIndex(currIndex_curIndex) && CheckElecSupply(currIndex_curIndex))
					{
						elecSupply = true;
						break;
					}
				}

				for (auto& currIndex : tiles)
				{
					auto currIndex_curIndex = currIndex + sf::Vector2i(i, j);
					if (mcv_Model->IsValidTileIndex(currIndex_curIndex) && CheckRoadSupply(currIndex_curIndex))
					{
						roadSupply = true;
						break;
					}
				}

				if (elecSupply && roadSupply)
					possiblePos.push_back({ i,j });
			}
		}
	}

	if (possiblePos.empty())
		return { -1,-1 };
	else
		return possiblePos[Utils::RandomRange(0, possiblePos.size() - 1)];
}

bool SimCityGameSystem::CheckElecSupply(const CellIndex& tileIndex) const
{
	for (int i = 0; i < 4; i++)
	{
		CellIndex currIndex = tileIndex + Tile::d[i];
		if (mcv_Model->IsValidTileIndex(currIndex) && m_ElecSupply[currIndex.y][currIndex.x] > 0)
			return true;
	}
	return false;
}

bool SimCityGameSystem::CheckRoadSupply(const CellIndex& tileIndex) const
{
	return m_RoadSupply[tileIndex.y][tileIndex.x] > 0;
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
	else if (originTile.type == TileType::Road)
		DestroyRoad(tileIndex);

	SOUND_MGR->PlaySfx("sound/SFX/Bulldozer-WithLoop.wav");
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

void SimCityGameSystem::DestroyRoad(const CellIndex& tileIndex)
{
	m_RoadSupply[tileIndex.y][tileIndex.x] = 0;
	UpdateRoadlink();
}

void SimCityGameSystem::SetZone(std::list<CellIndex> tiles, const ZoneType& zone)
{
	for (auto& currIndex : tiles)
	{
		m_ZoneInfos[currIndex.y][currIndex.x] = zone;
	}

	if (zone == ZoneType::None)
	{
		mcv_Model->SetTiles(tiles, TileType::Terrain, "terrain", "land");
	}
	else
	{
		Action t;
		if (zone == ZoneType::Residential)
			t = Action::ZoneResidential;
		else if (zone == ZoneType::Commercial)
			t = Action::ZoneCommercial;
		else if (zone == ZoneType::Industrial)
			t = Action::ZoneIndustrial;

		auto& tileset = m_TileSet.find(t)->second.info;
		mcv_Model->SetTiles(tiles, tileset.type, tileset.subtype, tileset.name);
	}
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
			{
				elecUpdate.push_back(currIndex_currIndex);
				m_ElecSupply[currIndex_currIndex.y][currIndex_currIndex.x] = powerplantId;
			}
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

void SimCityGameSystem::UpdateRoadlink()
{
	std::queue<CellIndex> q;
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			if (m_RoadSupply[j][i] != 4)
			{
				m_RoadSupply[j][i] = 0;
			}
			else
			{
				q.push({ i,j });
			}
		}
	}

	std::vector<std::vector<bool>> visited = std::vector<std::vector<bool>>
		(mcv_Model->m_CellCount.y, std::vector<bool>(mcv_Model->m_CellCount.x, false));

	while (!q.empty())
	{
		CellIndex currIndex = q.front();
		q.pop();
		int nextDepth = m_RoadSupply[currIndex.y][currIndex.y] - 1;

		if (visited[currIndex.y][currIndex.x] || nextDepth == 0)
			continue;
		visited[currIndex.y][currIndex.x] = true;
		for (int i = 0; i < 4; i++)
		{
			CellIndex currIndex_currIndex = currIndex + Tile::d[i];
			if (mcv_Model->IsValidTileIndex(currIndex_currIndex) && m_RoadSupply[currIndex_currIndex.y][currIndex_currIndex.x] < nextDepth)
			{
				q.push(currIndex_currIndex);
			}
		}
	}
}

//int SimCityGameSystem::Find(int groupId)
//{
//	if (m_ElecGroup[groupId] == groupId)return groupId;
//	else m_ElecGroup[groupId] = Find(m_ElecGroup[groupId]);
//}
//
//void SimCityGameSystem::Union(int groupId1, int groupId2)
//{
//	groupId1 = Find(groupId1);
//	groupId2 = Find(groupId2);
//
//	if (groupId1 > groupId2)
//		std::swap(groupId1, groupId2);
//
//	m_ElecGroup[groupId2] = groupId1;
//}

void SimCityGameSystem::SetTileSet()
{
	rapidcsv::Document doc("datatables/tileset.csv", rapidcsv::LabelParams(0, 0));
	int row = doc.GetRowCount();

	for (int j = 0; j < row; j++)
	{
		ActionSet action;
		action.action = (Action)j;
		std::vector<std::string> strings = doc.GetRow<std::string>(j);
		action.name = strings[0];
		if (strings[1] == "" && strings[2] == "")
		{
			DATATABLE_TILERES->SetTileInfo(action.info, -1);
			action.cost = 0;
		}
		else
		{
			DATATABLE_TILERES->SetTileInfo(action.info, doc.GetCell<int>("id", j));
			action.cost = doc.GetCell<int>("cost", j);
		}
		std::vector<std::string> sub;
		SetStringToVectorElements(strings[3], sub);
		if (!sub.empty())
		{
			for (auto& subname : sub)
			{
				Action subaction = (Action)doc.GetRowIdx(subname);
				action.sub.push_back({ subaction, doc.GetCell<std::string>("name", (int)subaction) });
			}
		}
		m_TileSet.insert({ action.action, action });
	}

	/*TileInfo road;
	road.type = TileType::Road;
	road.subtype = "road";
	road.name = "road_1";
	road.lotSize = DATATABLE_TILERES->GetTileRes(road.type, road.subtype, road.name).lotSize;
	m_TileSet.insert({ Action::Road, road });

	TileInfo rail;
	rail.type = TileType::Rail;
	rail.subtype = "rail";
	rail.name = "rail_1";
	rail.lotSize = DATATABLE_TILERES->GetTileRes(rail.type, rail.subtype, rail.name).lotSize;
	m_TileSet.insert({ Action::Rail, rail });

	TileInfo powerline;
	powerline.type = TileType::Powerline;
	powerline.subtype = "powerline";
	powerline.name = "powerline_1";
	powerline.lotSize = DATATABLE_TILERES->GetTileRes(powerline.type, powerline.subtype, powerline.name).lotSize;
	m_TileSet.insert({ Action::PowerLine, powerline });

	TileInfo powerplant;
	powerplant.type = TileType::Building;
	powerplant.subtype = "power_plant";
	powerplant.name = "coal";
	powerplant.lotSize = DATATABLE_TILERES->GetTileRes(powerplant.type, powerplant.subtype, powerplant.name).lotSize;
	m_TileSet.insert({ Action::PowerPlant, powerplant });

	TileInfo residential;
	residential.type = TileType::Zone;
	residential.subtype = "";
	residential.name = "zone_1";
	residential.lotSize = DATATABLE_TILERES->GetTileRes(residential.type, residential.subtype, residential.name).lotSize;
	m_TileSet.insert({ Action::ZoneResidential, residential });

	TileInfo commercial;
	commercial.type = TileType::Zone;
	commercial.subtype = "";
	commercial.name = "zone_3";
	commercial.lotSize = DATATABLE_TILERES->GetTileRes(commercial.type, commercial.subtype, commercial.name).lotSize;
	m_TileSet.insert({ Action::ZoneCommercial, commercial });

	TileInfo industrial;
	industrial.type = TileType::Zone;
	industrial.subtype = "";
	industrial.name = "zone_5";
	industrial.lotSize = DATATABLE_TILERES->GetTileRes(industrial.type, industrial.subtype, industrial.name).lotSize;
	m_TileSet.insert({ Action::ZoneIndustrial, industrial });

	TileInfo destroy;
	destroy.type = TileType::Building;
	destroy.subtype = "rubble";
	destroy.name = "rubble_1";
	destroy.lotSize = DATATABLE_TILERES->GetTileRes(destroy.type, destroy.subtype, destroy.name).lotSize;
	m_TileSet.insert({ Action::Destroy, destroy });*/

}

void SimCityGameSystem::SetMenuSet()
{
	rapidcsv::Document doc("datatables/menuset.csv", rapidcsv::LabelParams(0, 0));
	int row = doc.GetRowCount();

	for (int j = 0; j < row; j++)
	{
		MenuSet menu;
		menu.menu = (Menu)j;
		menu.name = doc.GetCell<std::string>("name", j);
		m_MenuSet.insert({ menu.menu, menu });
	}

	for (int j = 0; j < row; j++)
	{
		MenuSet& menu = m_MenuSet.find((Menu)j)->second;
		std::vector<std::string> strings = doc.GetRow<std::string>(j);
		if (strings[1] != "")
		{
			std::vector<std::string> sub;
			SetStringToVectorElements(strings[1], sub);
			if (!sub.empty())
			{
				for (auto& subname : sub)
				{
					Menu subaction = (Menu)doc.GetRowIdx(subname);
					menu.sub.push_back({ subaction, m_MenuSet.find(subaction)->second });
				}
			}
		}
	}

	m_MenuSet.find(Menu::Save)->second.func = std::bind(&SimCityGameSystem::SaveTileDepthFile, this);
}

void SimCityGameSystem::SetStringToVectorElements(const std::string& str, std::vector<std::string>& vec)
{
	vec.clear();
	if (str == "")return;
	int curr = 0;
	while (true)
	{
		auto it = str.find(',', curr);
		if (it == std::string::npos)
		{
			vec.push_back(str.substr(curr, str.length() - curr));
			return;
		}
		vec.push_back(str.substr(curr, it - curr));
		curr = it + 1;
	}
}