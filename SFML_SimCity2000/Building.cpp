#include "pch.h"
#include "Building.h"
#include "SimCityGameSystem.h"

Building::~Building()
{
}

void Building::BuildBuilding(std::list<CellIndex>& tiles)
{
    m_BuildingInfo.position.clear();
    for (auto& currIndex : tiles)
    {
        m_BuildingInfo.position.push_back(currIndex);
    }
}

void Building::Reset()
{
    if (!m_GameSystem)
        SetIsValid(false);
}

void Building::Update(float dt)
{
    SupplyUpdate(m_GameSystem->GetGameSysSpeed() * dt);
}

void Building::LateUpdate(float dt)
{
    UsageUpdate(m_GameSystem->GetGameSysSpeed() * dt);
}


