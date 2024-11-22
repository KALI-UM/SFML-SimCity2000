#include "pch.h"
#include "PowerPlantBuilding.h"

PowerPlantBuilding::PowerPlantBuilding()
{
    m_BuildingInfo.buildingType = BuildingType::PowerPlant;
}

PowerPlantBuilding::~PowerPlantBuilding()
{
}

bool PowerPlantBuilding::Initialize()
{
    m_BuildingInfo.name = "coal";
    m_BuildingInfo.elecUsage = 0;
    m_BuildingInfo.waterUsage = 0;
    m_BuildingInfo.texUsage = 0;

    return false;
}

void PowerPlantBuilding::SupplyUpdate(float dt)
{
    m_ElecAmount = m_SupplyMonth * dt;
}

void PowerPlantBuilding::UsageUpdate(float dt)
{
}
