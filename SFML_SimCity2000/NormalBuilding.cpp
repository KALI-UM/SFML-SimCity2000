#include "pch.h"
#include "NormalBuilding.h"

NormalBuilding::NormalBuilding()
{
    m_BuildingInfo.buildingType = BuildingType::None;
}

NormalBuilding::~NormalBuilding()
{
}

bool NormalBuilding::Initialize()
{
    return false;
}

void NormalBuilding::SupplyUpdate(float dt)
{
}

void NormalBuilding::UsageUpdate(float dt)
{
}
