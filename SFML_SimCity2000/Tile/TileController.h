#pragma once
#include "Tile.h"

class TileModel;
class TileView;

enum class ControlStatus
{
	None,
	Place,
	Drag,
};

class TileController
	:public GameObject
{
protected:
	int			m_ViewIndex;
	TileModel*	mcv_Model;
	TileView*	mcv_View;
public:
	TileController(TileModel* model, TileView* view, int viewIndex);
	~TileController();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;
	//void LateUpdate(float dt) override;
	//void FixeUpdate(float dt) override;
	//void Release() override;

	void UpdateNone(float dt);
	void UpdatePlace(float dt);
	void UpdateDrag(float dt);


	CellIndex GetMouseOverlaidTile()const { return m_MouseOverlaidTile; }

	void SetLineIntersectedTiles(const CellIndex& startIndex, const CellIndex& endIndex);
	void PushToSelectingTiles(const CellIndex& tileIndex);
protected:
	ControlStatus	m_CurrStatus = ControlStatus::Place;
	TileType		m_CurrTileType = TileType::Road;
	CellIndex		m_MouseOverlaidTile;
	CellIndex		m_DragStartTile;
	std::list<CellIndex> m_SelectingTiles;

};

