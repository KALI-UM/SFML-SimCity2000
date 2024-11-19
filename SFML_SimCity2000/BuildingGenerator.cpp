#include "pch.h"
#include "BuildingGenerator.h"

BuildingGenerator::BuildingGenerator(ZoneType type)
    :m_Zone(type)
{
}

BuildingGenerator::~BuildingGenerator()
{
}

bool BuildingGenerator::Initialize()
{
    return false;
}

void BuildingGenerator::Reset()
{
    m_RandomValue = 0;
}

void BuildingGenerator::Update()
{
    m_RandomValue += Utils::RandomRange(0, 100);
    if (m_RandomValue > 100)
    {
        //ºôµù»ý¼º
    }
}

void BuildingGenerator::PushToEmptyZone(const CellIndex& tileIndex)
{
    m_EmptyZones.push_back(tileIndex);
}

void BuildingGenerator::RemoveToEmptyZone(const CellIndex& tileIndex)
{
    auto it = std::find(m_EmptyZones.begin(), m_EmptyZones.end(), tileIndex);
    if (it != m_EmptyZones.end())
    {
        m_EmptyZones.remove(*it);
    }
}
