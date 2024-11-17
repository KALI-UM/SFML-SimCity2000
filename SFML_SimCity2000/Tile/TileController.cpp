#include "pch.h"
#include "TileController.h"
#include "TileModel.h"
#include "TileView.h"

TileController::TileController(TileModel* model, TileView* view, int viewIndex)
	:mcv_Model(model), mcv_View(view), m_ViewIndex(viewIndex)
{
}

TileController::~TileController()
{
}

bool TileController::Initialize()
{
	if (!mcv_Model || !mcv_View)return false;

	return false;
}

void TileController::Reset()
{
}

void TileController::Update(float dt)
{
	switch (m_CurrStatus)
	{
	case ControlStatus::None:
		UpdateNone(dt);
		break;
	case ControlStatus::Place:
		UpdatePlace(dt);
		break;
	case ControlStatus::Drag:
		UpdateDrag(dt);
		break;
	}
}

void TileController::UpdateNone(float dt)
{

}

void TileController::UpdatePlace(float dt)
{
	m_MouseOverlaidTile = mcv_View->GetTileCoordinatedIndex(INPUT_MGR->GetMouseViewPos(m_ViewIndex));
	if (!mcv_Model->IsValidTileIndex(m_MouseOverlaidTile))return;

	if (INPUT_MGR->GetMouseDown(sf::Mouse::Left))
	{
		m_DragStartTile = m_MouseOverlaidTile;
		if (mcv_Model->IsPossibleToBuild(m_DragStartTile, m_CurrTileType))
			PushToSelectingTiles(m_DragStartTile);

		m_CurrStatus = ControlStatus::Drag;
	}
	else
	{
		mcv_View->ColorizeTile(ColorPalette::Gray, m_MouseOverlaidTile);
	}
}

void TileController::UpdateDrag(float dt)
{
	m_MouseOverlaidTile = mcv_View->GetTileCoordinatedIndex(INPUT_MGR->GetMouseViewPos(m_ViewIndex));
	if (INPUT_MGR->GetMouseUp(sf::Mouse::Left))
	{
		mcv_Model->SetTiles(m_SelectingTiles, m_CurrTileType);
		m_CurrStatus = ControlStatus::Place;
		m_SelectingTiles.clear();
		return;
	}


	if (INPUT_MGR->GetMouseDrag(sf::Mouse::Left) && m_DragStartTile != m_MouseOverlaidTile)
	{
		if (mcv_Model->IsValidTileIndex(m_DragStartTile) && mcv_Model->IsValidTileIndex(m_MouseOverlaidTile))
			SetLineIntersectedTiles(m_DragStartTile, m_MouseOverlaidTile);
	}
}

void TileController::SetLineIntersectedTiles(const CellIndex& startIndex, const CellIndex& endIndex)
{
	m_SelectingTiles.clear();

	sf::Vector2i slope = startIndex - endIndex;
	sf::Vector2i currIndex = startIndex;
	int x0 = startIndex.x;
	int y0 = startIndex.y;
	int x1 = endIndex.x;
	int y1 = endIndex.y;

	//Bresenham's Line Algorithm
	//���� ����� 1�� 0����
	//x�� ������ų ������ y 

	int dx = abs(slope.x);
	int dy = abs(slope.y);
	int sx = (startIndex.x < endIndex.x) ? 1 : -1;
	int sy = (startIndex.y < endIndex.y) ? 1 : -1;

	//slope err������
	int err = dx - dy;

	while (true)
	{
		if (!mcv_Model->IsPossibleToBuild(currIndex, m_CurrTileType))return;

		// ���� Ÿ�� ��ĥ
		PushToSelectingTiles(currIndex);
		if (currIndex == endIndex)break;

		int e2 = 2 * err;
		bool needToMoveX = e2 > -dy;
		// x �������� �̵�
		if (needToMoveX)
		{
			err -= dy;
			currIndex.x += sx;
		}

		bool needToMoveY = e2 < dx;
		// y �������� �̵�
		if (needToMoveY)
		{
			err += dx;
			currIndex.y += sy;

			if (needToMoveX)
			{
				//PushToSelectingTiles({ currIndex.x - sx, currIndex.y });
				PushToSelectingTiles({ currIndex.x, currIndex.y - sy });
			}
		}
	}
}

void TileController::PushToSelectingTiles(const CellIndex& tileIndex)
{
	m_SelectingTiles.push_back(tileIndex);
	mcv_View->ColorizeTile(ColorPalette::Gray, tileIndex);
}
