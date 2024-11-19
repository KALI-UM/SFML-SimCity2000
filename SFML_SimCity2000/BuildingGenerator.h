#pragma once
#include "Tile.h"

class BuildingGenerator
{
public:
	BuildingGenerator(ZoneType type);
	~BuildingGenerator();

	bool Initialize();
	void Reset();
	void Update();

	void PushToEmptyZone(const CellIndex& tileIndex);
	void RemoveToEmptyZone(const CellIndex& tileIndex);

protected:
	int m_RandomValue = 0;
	const ZoneType			m_Zone;
	std::list<CellIndex>	m_EmptyZones;
};

