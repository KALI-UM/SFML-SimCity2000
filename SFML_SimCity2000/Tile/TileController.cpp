#include "pch.h"
#include "TileController.h"
#include "SimCityGameSystem.h"
#include "TileModel.h"
#include "TileView.h"
#include "TileResTable.h"
#include "SimCityButtonBar.h"
#include "SimCityMenuBar.h"
#include "SimCityCursor.h"

TileController::TileController(SimCityGameSystem* sys, TileModel* model, TileView* view, int viewIndex)
	:m_GameSystem(sys), mcv_Model(model), mcv_View(view), m_ViewIndex(viewIndex)
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
	m_ButtonBar->SetButtonFunc(std::bind(&TileController::SetCurrButton, this, std::placeholders::_1));
	for (int i = 0; i < 18; i++)
	{
		m_ButtonBar->SetSubButton((Action)i, m_GameSystem->GetActionSet(Action(i)).sub);
	}

	for (int i = 0; i < 2; i++)
	{
		m_MenuBar->SetMenuString((Menu)i, m_GameSystem->GetMenuSet((Menu)i).name);
		m_MenuBar->SetSubButton((Menu)i, m_GameSystem->GetMenuSet(Menu(i)).sub);
	}

	m_ButtonBar->SetCursor(m_Cursor);
	m_MenuBar->SetCursor(m_Cursor);

	m_ButtonBar->setPosition(0, 30);
	m_MenuBar->setPosition(0, 0);

	SetCurrButton(Action::Move);
}

void TileController::Update(float dt)
{
	m_MousePrevTile = m_MouseOverlaidTile;
	m_MouseOverlaidTile = mcv_View->GetTileCoordinatedIndex(INPUT_MGR->GetMouseViewPos(m_ViewIndex));
	if (m_MousePrevTile != m_MouseOverlaidTile)
		m_PrevTile = m_MousePrevTile;

	if (!m_ButtonBar->GetHasFocus()&&!m_MenuBar->GetHasFocus())
	{
		SetCurrCusor(m_GameSystem->GetCurrAction());

		switch (m_CurrStatus)
		{
		case ControlStatus::None:
			UpdateNone(dt);
			break;
		case ControlStatus::Place:
			UpdatePlace(dt);
			break;
		case ControlStatus::Destroy:
			UpdateDestroy(dt);
			break;
		case ControlStatus::Drag:
			UpdateDrag(dt);
			break;
		}
	}
	else
	{
		m_Cursor->SetCursorMode(Action::NotUse22);
	}

	//sf::Vector2f moveoffset = { INPUT_MGR->GetAxisRaw(Axis::Horizontal) * dt * 100, -INPUT_MGR->GetAxisRaw(Axis::Vertical) * dt * 100 };
	//GAME_MGR->MoveView(m_ViewIndex, moveoffset);

	if (INPUT_MGR->GetKeyDown(sf::Keyboard::LBracket))
	{
		SetCurrButton(Action::ZoomOut);
	}
	else if (INPUT_MGR->GetKeyDown(sf::Keyboard::RBracket))
	{
		SetCurrButton(Action::ZoomIn);
	}
}

void TileController::SetMenuBar(SimCityMenuBar* bar)
{
	m_MenuBar = bar;
	SetChildObj(bar, false);
}

void TileController::SetButtonBar(SimCityButtonBar* bar)
{
	m_ButtonBar = bar;
	SetChildObj(bar, false);
}

void TileController::SetCusor(SimCityCursor* cursor)
{
	m_Cursor = cursor;
	SetChildObj(cursor, false);
}

void TileController::UpdateNone(float dt)
{
	if (m_GameSystem->GetCurrAction() == Action::Move)
	{
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Left))
		{
			GAME_MGR->SetViewCenter(m_ViewIndex, INPUT_MGR->GetMouseViewPos(m_ViewIndex));
		}
	}
}

void TileController::UpdatePlace(float dt)
{
	if (!mcv_Model->IsValidTileIndex(m_MouseOverlaidTile))return;

	if (INPUT_MGR->GetMouseDown(sf::Mouse::Left))
	{
		m_DragStartTile = m_MouseOverlaidTile;
		SetNXMTiles(m_GameSystem->GetCurrTileSet().lotSize, m_DragStartTile);

		m_CurrStatus = ControlStatus::Drag;
	}
	else
	{
		mcv_View->ColorizeTile(ColorPalette::Gray, m_GameSystem->GetCurrTileSet().lotSize, m_MouseOverlaidTile);
	}
}

void TileController::UpdateDestroy(float dt)
{
	if (!mcv_Model->IsValidTileIndex(m_MouseOverlaidTile))return;

	if (INPUT_MGR->GetMouse(sf::Mouse::Left))
	{
		if (INPUT_MGR->GetMouseDrag(sf::Mouse::Left))
			mcv_Model->SetTempEffectTiles(m_MousePrevTile, TileType::Other, "", "bulldozer_1");

		if (m_MousePrevTile != m_MouseOverlaidTile)
		{
			Set1x1Tile(m_MouseOverlaidTile);
			if (!m_SelectingTiles.empty())
			{
				m_GameSystem->DestroySomething(m_SelectingTiles.front());
			}
		}
	}

	mcv_View->ColorizeTile(ColorPalette::Gray, m_GameSystem->GetCurrTileSet().lotSize, m_MouseOverlaidTile);
}

void TileController::UpdateDrag(float dt)
{
	if (INPUT_MGR->GetMouseUp(sf::Mouse::Left))
	{
		m_GameSystem->BuildSomething(m_SelectingTiles, m_GameSystem->GetCurrAction());
		m_SelectingTiles.clear();
		m_CurrStatus = ControlStatus::Place;
		return;
	}

	if (INPUT_MGR->GetMouseDrag(sf::Mouse::Left) && m_DragStartTile != m_MouseOverlaidTile)
	{
		if (mcv_Model->IsValidTileIndex(m_DragStartTile) && mcv_Model->IsValidTileIndex(m_MouseOverlaidTile))
		{
			auto& currbttset = m_GameSystem->GetCurrAction();
			if (currbttset == Action::Road || currbttset == Action::Rail || currbttset == Action::PowerLine)
			{
				SetLineIntersectedTiles(m_DragStartTile, m_MouseOverlaidTile);
			}

			else if (currbttset == Action::ZoneResidential || currbttset == Action::ZoneCommercial || currbttset == Action::ZoneIndustrial)
			{
				SetRangeIntersectedTiles(m_DragStartTile, m_MouseOverlaidTile, false);
			}
		}
	}
}

void TileController::SetCurrButton(Action btt)
{
	switch (btt)
	{
	case Action::Bulldozer:
		SetCurrButton(Action::DestroyNormal);
		return;
		break;
	case Action::Landscape:
	case Action::Dispatch:
		m_CurrStatus = ControlStatus::None;
		break;
	case Action::PowerSupply:
		SetCurrButton(Action::PowerLine);
		return;
		break;
	case Action::WaterSupply:
	case Action::Religion:
		m_CurrStatus = ControlStatus::None;
		break;
	case Action::Road:
		m_CurrStatus = ControlStatus::Place;
		break;
	case Action::Rail:
		m_CurrStatus = ControlStatus::Place;
		break;
	case Action::Port:
	case Action::ZoneResidential:
	case Action::ZoneCommercial:
	case Action::ZoneIndustrial:
		m_CurrStatus = ControlStatus::Place;
		break;
	case Action::Education:
		SetCurrButton(Action::GradeSchool);
		return;
	case Action::PublicService:
		SetCurrButton(Action::PoliceStation);
		return;
		return;
	case Action::Recreation:
		SetCurrButton(Action::Park);
		return;
		break;
	case Action::ZoomIn:
		GAME_MGR->SetViewZoom(m_ViewIndex, 0.5f);
		m_CurrStatus = ControlStatus::None;
		return;
		break;
	case Action::ZoomOut:
		GAME_MGR->SetViewZoom(m_ViewIndex, 2.0f);
		m_CurrStatus = ControlStatus::None;
		return;
		break;
	case Action::Move:
		m_CurrStatus = ControlStatus::None;
		break;
	case Action::NotUse18:
	case Action::NotUse19:
	case Action::NotUse20:
	case Action::NotUse21:
	case Action::NotUse22:
	case Action::NotUse23:
	case Action::NotUse24:
	case Action::NotUse25:
		m_CurrStatus = ControlStatus::None;
		break;
	case Action::DestroyNormal:
	case Action::DestroyZone:
		m_CurrStatus = ControlStatus::Destroy;
		break;
	case Action::Trees:
	case Action::Water:
		m_CurrStatus = ControlStatus::None;
		break;
	case Action::PowerLine:
	case Action::PowerPlant:
	case Action::GradeSchool:
	case Action::College:
	case Action::Library:
	case Action::PoliceStation:
	case Action::FireStation:
	case Action::Hospital:
	case Action::Park:
	case Action::LargePark:
	case Action::Zoo:
	case Action::Stadium:
		m_CurrStatus = ControlStatus::Place;
		break;
	}

	SetCurrCusor(btt);
	m_GameSystem->SetCurrTileSet(btt);
}

void TileController::SetCurrCusor(Action btt)
{
	switch (btt)
	{
	case Action::Bulldozer:
	case Action::DestroyNormal:
	case Action::DestroyZone:
		m_Cursor->SetCursorMode(Action::Bulldozer);
		break;
	case Action::Landscape:
	case Action::Trees:
	case Action::Water:
		m_Cursor->SetCursorMode(Action::Landscape);
		break;
	case Action::Dispatch:
		break;
	case Action::PowerSupply:
	case Action::PowerLine:
	case Action::PowerPlant:
		m_Cursor->SetCursorMode(Action::PowerSupply);
		break;
	case Action::Education:
	case Action::GradeSchool:
	case Action::College:
	case Action::Library:
		m_Cursor->SetCursorMode(Action::Education);
		break;
	case Action::PublicService:
	case Action::PoliceStation:
	case Action::FireStation:
	case Action::Hospital:
		m_Cursor->SetCursorMode(Action::PublicService);
		break;
	case Action::Recreation:
	case Action::Park:
	case Action::LargePark:
	case Action::Zoo:
	case Action::Stadium:
		m_Cursor->SetCursorMode(Action::Recreation);
		break;
	default:
		m_Cursor->SetCursorMode(btt);
		break;
	}
}

void TileController::Set1x1Tile(const CellIndex& tileIndex, bool checkPossible)
{
	m_SelectingTiles.clear();
	if (checkPossible)
	{
		if (!mcv_Model->IsPossibleToBuild(tileIndex, m_GameSystem->GetCurrTileSet().type, m_GameSystem->GetCurrTileSet().subtype))
			return;
	}
	PushToSelectingTiles(tileIndex);
}

void TileController::SetLineIntersectedTiles(const CellIndex& startIndex, const CellIndex& endIndex, bool checkPossible)
{
	m_SelectingTiles.clear();

	sf::Vector2i slope = startIndex - endIndex;
	sf::Vector2i currIndex = startIndex;
	int x0 = startIndex.x;
	int y0 = startIndex.y;
	int x1 = endIndex.x;
	int y1 = endIndex.y;

	//Bresenham's Line Algorithm
	//선의 기울기는 1과 0사이
	//x를 증가시킬 때마다 y 

	int dx = abs(slope.x);
	int dy = abs(slope.y);
	int sx = (startIndex.x < endIndex.x) ? 1 : -1;
	int sy = (startIndex.y < endIndex.y) ? 1 : -1;

	//slope err보정값
	int err = dx - dy;

	while (true)
	{
		if (checkPossible)
		{
			if (!mcv_Model->IsPossibleToBuild(currIndex, m_GameSystem->GetCurrTileSet().type, m_GameSystem->GetCurrTileSet().subtype))
				return;
		}

		// 현재 타일 색칠
		PushToSelectingTiles(currIndex);
		if (currIndex == endIndex)break;

		int e2 = 2 * err;
		bool needToMoveX = e2 > -dy;
		// x 방향으로 이동
		if (needToMoveX)
		{
			err -= dy;
			currIndex.x += sx;
		}

		bool needToMoveY = e2 < dx;
		// y 방향으로 이동
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

void TileController::SetRangeIntersectedTiles(const CellIndex& startIndex, const CellIndex& endIndex, bool checkPossible)
{
	m_SelectingTiles.clear();

	for (int j = std::min(startIndex.y, endIndex.y); j <= std::max(startIndex.y, endIndex.y); j++)
	{
		for (int i = std::min(startIndex.x, endIndex.x); i <= std::max(startIndex.x, endIndex.x); i++)
		{
			CellIndex currIndex = { i,j };
			if (checkPossible)
			{
				if (!mcv_Model->IsPossibleToBuild(currIndex, m_GameSystem->GetCurrTileSet().type, m_GameSystem->GetCurrTileSet().subtype))
					continue;
			}
			PushToSelectingTiles(currIndex);
		}
	}
}

void TileController::SetNXMTiles(const sf::Vector2u& lot, const CellIndex& centerIndex, bool checkPossible)
{
	m_SelectingTiles.clear();

	CellIndex startIndex;
	if (lot == sf::Vector2u(1, 1))
	{
		Set1x1Tile(centerIndex, checkPossible);
		return;
	}
	else if (lot == sf::Vector2u(2, 2))
	{
		startIndex = centerIndex + sf::Vector2i(-1, 0);
	}
	else if (lot == sf::Vector2u(3, 3))
	{
		startIndex = centerIndex + sf::Vector2i(-1, -1);
	}
	else if (lot == sf::Vector2u(4, 4))
	{
		startIndex = centerIndex + sf::Vector2i(-2, -1);
	}

	for (int j = startIndex.y; j < startIndex.y + lot.y; j++)
	{
		for (int i = startIndex.x; i < startIndex.x + lot.x; i++)
		{
			CellIndex currIndex = { i,j };
			if (checkPossible)
			{
				if (!mcv_Model->IsPossibleToBuild(currIndex, m_GameSystem->GetCurrTileSet().type, m_GameSystem->GetCurrTileSet().subtype))
					return;
			}

		}
	}

	for (int j = startIndex.y; j < startIndex.y + lot.y; j++)
	{
		for (int i = startIndex.x; i < startIndex.x + lot.x; i++)
		{
			CellIndex currIndex = { i,j };
			PushToSelectingTiles(currIndex);
		}
	}
}

void TileController::PushToSelectingTiles(const CellIndex& tileIndex)
{
	m_SelectingTiles.push_back(tileIndex);
	mcv_View->ColorizeTile(ColorPalette::Gray, tileIndex);
}
