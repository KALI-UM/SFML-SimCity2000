#include "pch.h"
#include "SimCityGameSystem.h"
#include "BuildingGenerator.h"

SimCityGameSystem::SimCityGameSystem()
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
