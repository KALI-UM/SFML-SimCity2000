#include "pch.h"
#include "PublicServiceBuilding.h"

PublicServiceBuilding::PublicServiceBuilding()
{
    m_BuildingInfo.buildingType = BuildingType::PublicService;
}

PublicServiceBuilding::~PublicServiceBuilding()
{
}

bool PublicServiceBuilding::Initialize()
{
    return false;
}

void PublicServiceBuilding::SupplyUpdate(float dt)
{
}

void PublicServiceBuilding::UsageUpdate(float dt)
{
}
