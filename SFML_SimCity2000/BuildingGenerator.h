#pragma once
#include "Tile.h"

class Building;
class BuildingGenerator
{
public:
	BuildingGenerator(ZoneType type);
	~BuildingGenerator();

	bool Initialize();
	void Reset();
	void Update();

	void SetGetBuildPosFunc(std::function<sf::Vector2i(std::list<CellIndex>&)> func) { m_GetCanBuildPosFunc = func; };
	void SetBuildFunc(std::function<void(std::list<CellIndex>&, const TileResData&)> func) { m_GenerateBuildingFunc = func; };
protected:
	int m_RandomValue = 0;
	const ZoneType			m_Zone;

	std::function<sf::Vector2i(std::list<CellIndex>&)> m_GetCanBuildPosFunc;
	std::function<void(std::list<CellIndex>&, const TileResData&)> m_GenerateBuildingFunc;

};

