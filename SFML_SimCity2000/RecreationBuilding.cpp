#include "pch.h"
#include "RecreationBuilding.h"

RecreationBuilding::RecreationBuilding()
{
	m_BuildingInfo.buildingType = BuildingType::Recreation;
}

RecreationBuilding::~RecreationBuilding()
{
}

bool RecreationBuilding::Initialize()
{
	return false;
}

void RecreationBuilding::SupplyUpdate(float dt)
{
}

void RecreationBuilding::UsageUpdate(float dt)
{
}
