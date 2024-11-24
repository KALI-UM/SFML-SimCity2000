#pragma once
#include "Tile.h"

class SimCityGameSystem;
class TileModel;
class TileView;
enum class Action;
class SimCityMenuBar;
class SimCityButtonBar;
class SimCityCursor;
enum class ControlStatus
{
	None,
	Place,
	Destroy,
	Drag,
};

enum class TileSet;
class TileController
	:public GameObject
{
protected:
	int			m_ViewIndex;
	SimCityGameSystem* m_GameSystem;
	TileModel* mcv_Model;
	TileView* mcv_View;
public:
	TileController(SimCityGameSystem* sys, TileModel* model, TileView* view, int viewIndex);
	~TileController();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;
	//void LateUpdate(float dt) override;
	//void FixeUpdate(float dt) override;
	//void Release() override;
	void SetMenuBar(SimCityMenuBar* bar);
	void SetButtonBar(SimCityButtonBar* bar);
	void SetCusor(SimCityCursor* cursor);


	CellIndex GetMouseOverlaidTileIndex() const { return m_MouseOverlaidTile; }
	CellIndex GetDragStartTileIndex() const { return m_DragStartTile; }
	CellIndex GetMousePrevTileIndex() const { return m_MousePrevTile; }
	CellIndex GetPrevTileIndex() const { return m_PrevTile; }


	void UpdateNone(float dt);
	void UpdatePlace(float dt);
	void UpdateDestroy(float dt);
	void UpdateDrag(float dt);

	void SetCurrButton(Action btt);
	void SetCurrCusor(Action btt);

	void Set1x1Tile(const CellIndex& tileIndex, bool checkPossible = true);
	void SetLineIntersectedTiles(const CellIndex& startIndex, const CellIndex& endIndex, bool checkPossible = true);
	void SetRangeIntersectedTiles(const CellIndex& startIndex, const CellIndex& endIndex, bool checkPossible = true);
	void SetNXMTiles(const sf::Vector2u& lot, const CellIndex& centerIndex, bool checkPossible = true);

	void PushToSelectingTiles(const CellIndex& tileIndex);
protected:
	ControlStatus	m_CurrStatus = ControlStatus::Place;

	CellIndex		m_MouseOverlaidTile;
	CellIndex		m_MousePrevTile;
	CellIndex		m_DragStartTile;
	CellIndex		m_PrevTile;
	std::list<CellIndex> m_SelectingTiles;

	SimCityMenuBar*		m_MenuBar;
	SimCityButtonBar*	m_ButtonBar;
	SimCityCursor*		m_Cursor;

};

