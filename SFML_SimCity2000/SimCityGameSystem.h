#pragma once
#include "Tile.h"

enum class GameStatus
{
	Pause,
	Play,
};

struct CityInfo
{
	std::string name;
	int monthPassed;		//몇달지났는지
	int population;			//총인구
	int tresury;			//국고
	int tax;				//세금
};

struct LinkedGroup
{
	int groupId;
	float ammount;
};

class Building;
class PowerPlantBuilding;
class TileModel;
class BuildingGenerator;
class SimCityGameSystem :
	public GameObject
{
protected:
	friend class Scene_SimCityInGame;
	TileModel* mcv_Model;

private:
	SimCityGameSystem(TileModel* model);
public:
	~SimCityGameSystem();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;

	float GetGameSysSpeed() const { return m_CurrStatus == GameStatus::Pause ? 0.0f : m_PlaySpeed; }

	void PushToEmptyZone(const CellIndex& tileIndex, ZoneType zone);
	void RemoveToEmptyZone(const CellIndex& tileIndex, ZoneType zone);

	void BuildBuilding(std::list<CellIndex>& tiles, Building* building);
	void BuildPowerPlant(std::list<CellIndex>& tiles);
	void BuildPowerlink(std::list<CellIndex>& tiles, int powerplantId = -1);
	void DestroyPowerlink(std::list<CellIndex>& tiles);


	void UpdatePowerlink(std::list<CellIndex>& tiles, int powerplantId = -1);
	void UpdatePowerlink();
	void ResetPowerlink();
protected:
	GameStatus	m_CurrStatus = GameStatus::Pause;
	float		m_PlaySpeed = 1.0f;

	std::vector<BuildingGenerator>		m_BuildingGenerator;
	std::list<Building*>				m_Buildings;
	std::list<PowerPlantBuilding*>		m_PowerPlantBuilding;

	std::vector<std::vector<int>>		m_ElecSupply;
	std::vector<std::vector<int>>		m_ElecGroupId;


private:
	int Find(int groupId);
	void Union(int groupId1, int groupId2);
	//임시
	std::vector<int> m_ElecGroup;
};


