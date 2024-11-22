#pragma once
#include "Tile.h"
#include "Building.h"

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

enum class ButtonSet
{
	Road,
	Rail,
	Powerlink,
	Powerplant,
	Zone,
	Destroy,
};

namespace std {
	template <> struct hash<ButtonSet> {
		size_t operator() (const ButtonSet& t) const { return size_t(t); }
	};
}

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

protected:
	std::unordered_map<ButtonSet, TileInfo> m_TileSet;
	ButtonSet m_CurrTileSet = ButtonSet::Road;
	void SetTileSet();
public:
	void SetCurrTileSet(ButtonSet set);
	const ButtonSet& GetCurrButtonSet() const { return m_CurrTileSet; }
	const TileInfo& GetCurrTileSet()const { return m_TileSet.find(m_CurrTileSet)->second; }

	void BuildSomething(std::list<CellIndex>& tiles);
	void BuildZone(std::list<CellIndex>& tiles);
	void BuildBuilding(std::list<CellIndex>& tiles);
	void BuildNoneBuilding(ZoneType zone, std::list<CellIndex>& tiles, const TileResData& info);
	PowerPlantBuilding* BuildPowerPlant(std::list<CellIndex>& tiles);
	void BuildPowerlink(std::list<CellIndex>& tiles, int powerplantId = -1);
	void BuildRoad(std::list<CellIndex>& tiles);

	CellIndex GetBuildPossiblePos(ZoneType zone, std::list<CellIndex>& tiles) const;
	bool CheckElecSupply(const CellIndex& tileIndex)const;
	bool CheckRoadSupply(const CellIndex& tileIndex)const;


	void DestroySomething(const CellIndex& tileIndex);
	void DestroyBuilding(const CellIndex& tileIndex);
	void DestroyPowerlink(const CellIndex& tileIndex);
	void DestroyRoad(const CellIndex& tileIndex);

protected:
	void UpdatePowerlink(std::list<CellIndex>& tiles, int powerplantId = -1);
	void UpdatePowerlink();
	void ResetPowerlink();
	void UpdateRoadlink();
protected:
	GameStatus	m_CurrStatus = GameStatus::Play;
	float		m_PlaySpeed = 1.0f;
	float		m_LastUpdate;
	float		m_UpdateCycle = 24 * 30;			//1초가 1시간, 기본 업데이트 사이클은 1달에 한번

	std::vector<BuildingGenerator>		m_BuildingGenerator;


	std::list<PowerPlantBuilding*>		m_PowerPlantBuildings;

	std::vector<std::vector<Building*>>		m_BuildingMap;
	std::vector<std::vector<int>>			m_ElecSupply;
	std::vector<std::vector<int>>			m_RoadSupply;
	std::vector<std::vector<ZoneType>>		m_ZoneInfos;


	std::vector<std::vector<int>>		m_ElecGroupId;


private:
	int Find(int groupId);
	void Union(int groupId1, int groupId2);
	//임시
	std::vector<int> m_ElecGroup;


};


