#pragma once
#include "Tile.h"

class SimCityGameSystem;
class TileModel;
class TileView;

enum class ControlStatus
{
	None,
	Place,
	Destroy,
	Drag,
};

class TileController
	:public GameObject
{
protected:
	int			m_ViewIndex;
	SimCityGameSystem* m_GameSystem;
	TileModel*	mcv_Model;
	TileView*	mcv_View;
public:
	TileController(SimCityGameSystem* sys, TileModel* model, TileView* view, int viewIndex);
	~TileController();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;
	//void LateUpdate(float dt) override;
	//void FixeUpdate(float dt) override;
	//void Release() override;

	CellIndex GetMouseOverlaidTileIndex() const { return m_MouseOverlaidTile; }
	CellIndex GetDragStartTileIndex() const { return m_DragStartTile; }
	CellIndex GetMousePrevTileIndex() const { return m_MousePrevTile; }
	CellIndex GetPrevTileIndex() const { return m_PrevTile; }


	void UpdateNone(float dt);
	void UpdatePlace(float dt);
	void UpdateDestroy(float dt);
	void UpdateDrag(float dt);


	void SetCurrTile(const TileType& type, const SUBTYPE& subtype, const std::string& name);
	void SetDestroyStatus();

	void Set1x1Tile(const CellIndex& tileIndex);
	void SetLineIntersectedTiles(const CellIndex& startIndex, const CellIndex& endIndex);
	void SetRangeIntersectedTiles(const CellIndex& startIndex, const CellIndex& endIndex);
	void SetNXMTiles(const sf::Vector2u& lot, const CellIndex& centerIndex);

	void PushToSelectingTiles(const CellIndex& tileIndex);
protected:
	ControlStatus	m_CurrStatus = ControlStatus::Place;
	TileType		m_CurrTileType = TileType::Road;
	SUBTYPE			m_CurrSubType = "road";
	std::string		m_CurrTileName = "road_1";
	sf::Vector2u	m_CurrLotSize = { 1,1 };

	CellIndex		m_MouseOverlaidTile;
	CellIndex		m_MousePrevTile;
	CellIndex		m_DragStartTile;
	CellIndex		m_PrevTile;
	std::list<CellIndex> m_SelectingTiles;

};

