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
	int monthPassed;		//�����������
	int population;			//���α�
	int tresury;			//����
	int tax;				//����
};

//enum class TileSet
//{
//	Road,
//	Rail,
//	Powerlink,
//	Powerplant,
//	Zone_R,
//	Zone_C,
//	Zone_I,
//	Destroy,
//	None,
//};

enum class ButtonName
{
	Destroy,
	Tree,
	Alram,
	Elec,
	Water,
	Religion,
	Road,
	Rail,
	Port,
	ZoneResidential,
	ZoneCommercial,
	ZoneIndustrial,
	Education,
	PoliceStation,
	Park,
	ZoomIn,
	ZoomOut,
	Move,
	NotUse18,
	NotUse19,
	NotUse20,
	NotUse21,
	NotUse22,
	NotUse23,
	NotUse24,
	NotUse25,
	PowerLine,
	PowerPlant,
};

namespace std {
	template <> struct hash<ButtonName> {
		size_t operator() (const ButtonName& t) const { return size_t(t); }
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
	void LoadTileDepthFile();
	void LoadTerrainDepth();
	void LoadOnGroundDepth();

	void SaveTileDepthFile();
	void SaveTerrainDepth();
	void SaveOnGroundDepth();

	std::unordered_map<ButtonName, TileInfo> m_TileSet;
	ButtonName m_CurrTileSet;
	void SetTileSet();
public:
	void SetCurrTileSet(ButtonName set);
	const ButtonName& GetCurrButtonSet() const { return m_CurrTileSet; }
	const TileInfo& GetCurrTileSet()const { return m_TileSet.find(m_CurrTileSet)->second; }

	void BuildRawThing(const CellIndex& tileIndex, const TileResData& data);
	void BuildSomething(std::list<CellIndex>& tiles, const ButtonName& set);
	void BuildZone(std::list<CellIndex>& tiles);
	void BuildBuilding(std::list<CellIndex>& tiles, const BuildingType& type);
	void BuildAutoBuilding(std::list<CellIndex>& tiles, const TileResData& info);
	PowerPlantBuilding* BuildPowerPlant(std::list<CellIndex>& tiles);

	void BuildPowerline(std::list<CellIndex>& tiles, int powerplantId = -1);
	void BuildPowerlink(std::list<CellIndex>& tiles, int powerplantId = -1);
	void BuildRoad(std::list<CellIndex>& tiles);

	CellIndex GetBuildPossiblePos(ZoneType zone, std::list<CellIndex>& tiles) const;
	bool CheckElecSupply(const CellIndex& tileIndex)const;
	bool CheckRoadSupply(const CellIndex& tileIndex)const;

	void DestroySomething(const CellIndex& tileIndex);
	void DestroyBuilding(const CellIndex& tileIndex);
	void DestroyPowerlink(const CellIndex& tileIndex);
	void DestroyRoad(const CellIndex& tileIndex);

	void SetZone(std::list<CellIndex> tiles, const ZoneType& zone);

protected:
	void UpdatePowerlink(std::list<CellIndex>& tiles, int powerplantId = -1);
	void UpdatePowerlink();
	void ResetPowerlink();
	void UpdateRoadlink();
protected:
	GameStatus	m_CurrStatus = GameStatus::Play;
	float		m_PlaySpeed = 1.0f;
	float		m_LastUpdate;
	float		m_UpdateCycle = 24 * 30;			//1�ʰ� 1�ð�, �⺻ ������Ʈ ����Ŭ�� 1�޿� �ѹ�

	std::vector<BuildingGenerator>		m_BuildingGenerator;

	std::list<Building*>				m_Buildings;
	std::list<PowerPlantBuilding*>		m_PowerPlantBuildings;
	std::queue<Building*>				m_WantsToAddBuildings;

	std::vector<std::vector<Building*>>		m_BuildingMap;
	std::vector<std::vector<int>>			m_ElecSupply;
	std::vector<std::vector<int>>			m_RoadSupply;
	std::vector<std::vector<ZoneType>>		m_ZoneInfos;



private:
	//int Find(int groupId);
	//void Union(int groupId1, int groupId2);
	////�ӽ�
	//std::vector<int> m_ElecGroup;

	static void SetStringToVectorElements(const std::string& str, std::vector<std::string>& vec);
};


