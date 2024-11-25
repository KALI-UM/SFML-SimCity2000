#include "pch.h"
#include "EducationBuilding.h"

EducationBuilding::EducationBuilding()
{
    m_BuildingInfo.buildingType = BuildingType::Education;
}

EducationBuilding::~EducationBuilding()
{
}

bool EducationBuilding::Initialize()
{
    m_BuildingInfo.name = "GradeSchool";
    m_BuildingInfo.elecUsage = 0;
    m_BuildingInfo.waterUsage = 0;
    m_BuildingInfo.texUsage = 0;

	return false;
}

void EducationBuilding::SupplyUpdate(float dt)
{
}

void EducationBuilding::UsageUpdate(float dt)
{
}
