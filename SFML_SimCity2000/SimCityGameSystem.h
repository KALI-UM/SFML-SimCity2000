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
	int monthPassed;		//�����������
	int population;			//���α�
	int tresury;			//����
	int tax;				//����
};

class Building;
class TileModel;
class BuildingGenerator;
class SimCityGameSystem :
	public GameObject
{
protected:
	friend class Scene_SimCityInGame;
	TileModel* mcv_Model;
	
private:
	SimCityGameSystem();
public:
	~SimCityGameSystem();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;

	float GetGameSysSpeed() const { return m_CurrStatus == GameStatus::Pause ? 0.0f : m_PlaySpeed; }
	void PushToEmptyZone(const CellIndex& tileIndex, ZoneType zone);
	void RemoveToEmptyZone(const CellIndex& tileIndex, ZoneType zone);

protected:
	GameStatus	m_CurrStatus;
	float		m_PlaySpeed = 1.0f;

	std::vector<BuildingGenerator>		m_BuildingGenerator;
	std::list<Building*>				m_Buildings;

};

