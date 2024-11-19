#pragma once

struct BuildingInfo
{
	std::string name;

	float	elecUsage;
	float	waterUsage;
	float	texUsage;
	bool	isSupplied;
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
private:
	void Reset() override;
	void Update(float dt) final;
protected:
	virtual void GameSysSpeedUpdate(float dt) = 0;

protected:
	const SimCityGameSystem*	m_GameSystem;
	BuildingInfo				m_BuildingInfo;
};

