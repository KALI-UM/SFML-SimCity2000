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

enum class Action
{
	Bulldozer,
	Landscape,
	Dispatch,
	PowerSupply,
	WaterSupply,
	Religion,
	Road,
	Rail,
	Port,
	ZoneResidential,
	ZoneCommercial,
	ZoneIndustrial,
	Education,
	PublicService,
	Recreation,
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

	//서브버튼
	DestroyNormal,
	DestroyZone,
	Trees,
	Water,
	PowerLine,
	PowerPlant,
	GradeSchool,
	College,
	Library,
	PoliceStation,
	FireStation,
	Hospital,
	Park,
	LargePark,
	Zoo,
	Stadium,
};
enum class Menu
{
	File,
	Speed,
	Save,
};

namespace std {
	template <> struct hash<Action> {
		size_t operator() (const Action& t) const { return size_t(t); }
	};
}

namespace std {
	template <> struct hash<Menu> {
		size_t operator() (const Menu& t) const { return size_t(t); }
	};
}

struct ActionSet
{
	Action		action;
	std::string name;
	int			cost;
	TileInfo	info;
	std::vector<std::pair<Action, std::string>> sub;
};

struct MenuSet
{
	Menu		menu;
	std::string name;
	std::vector<std::pair<Menu, MenuSet&>> sub;
	std::function<void()> func;
};

class Building;
class PowerPlantBuilding;
class EducationBuilding;
class PublicServiceBuilding;
class RecreationBuilding;

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

	std::unordered_map<Action, ActionSet> m_TileSet;
	std::unordered_map<Menu, MenuSet> m_MenuSet;
	Action m_CurrAction;
	void SetTileSet();
	void SetMenuSet();
public:
	void SetCurrTileSet(Action set);
	const Action& GetCurrAction() const { return m_CurrAction; }
	const ActionSet& GetActionSet(Action action) const { return m_TileSet.find(action)->second; }
	const TileInfo& GetCurrTileSet()const { return m_TileSet.find(m_CurrAction)->second.info; }
	const MenuSet& GetMenuSet(Menu menu) const { return m_MenuSet.find(menu)->second; }

	void BuildRawThing(const CellIndex& tileIndex, const TileResData& data);
	void BuildSomething(std::list<CellIndex>& tiles, const Action& set);
	void BuildZone(std::list<CellIndex>& tiles);
	void BuildBuilding(std::list<CellIndex>& tiles, const Action& set);
	void BuildAutoBuilding(std::list<CellIndex>& tiles, const TileResData& info);

	PowerPlantBuilding* BuildPowerPlant();
	EducationBuilding* BuildEducationBuilding();
	PublicServiceBuilding* BuildPublicServiceBuilding();
	RecreationBuilding* BuildRecreationBuilding();

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
	float		m_UpdateCycle = 24 * 30;			//1초가 1시간, 기본 업데이트 사이클은 1달에 한번

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
	////임시
	//std::vector<int> m_ElecGroup;

	static void SetStringToVectorElements(const std::string& str, std::vector<std::string>& vec);
};


