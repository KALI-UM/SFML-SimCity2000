#pragma once
#include "Tile.h"
struct BuildingInfo
{
	std::string name;
	std::vector<CellIndex> position;

	int		buildingId;
	float	elecUsage;
	float	waterUsage;
	float	texUsage;
	bool	isSupplied;

	int		elecGroupId = 0;
	int		waterGroupId = 0;
};

class SimCityGameSystem;
class Building :
	public GameObject
{
public:
	Building() {};
	virtual ~Building();

	void SetGameSystem(SimCityGameSystem* gameSys) { m_GameSystem = gameSys; }
	const BuildingInfo& GetBuildingInfo() const { return m_BuildingInfo; }
	void BuildBuilding(std::list<CellIndex>& tiles);
private:
	void Reset() override;
	void Update(float dt) final;
	void LateUpdate(float dt) final;
protected:
	virtual void SupplyUpdate(float dt) = 0;
	virtual void UsageUpdate(float dt) = 0;

protected:
	const SimCityGameSystem*	m_GameSystem;
	BuildingInfo				m_BuildingInfo;
};
