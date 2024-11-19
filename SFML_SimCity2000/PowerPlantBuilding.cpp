#include "pch.h"
#include "PowerPlantBuilding.h"

PowerPlantBuilding::PowerPlantBuilding()
{
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

void PowerPlantBuilding::GameSysSpeedUpdate(float dt)
{
    m_ElecAmount = m_SupplyHour * dt;
}
