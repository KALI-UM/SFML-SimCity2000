#include "pch.h"
#include "Scene_SimCityInGame.h"
#include "SimCityCursor.h"
#include "TileGrid.h"
#include "Tile.h"

Scene_SimCityInGame::Scene_SimCityInGame()
	:SceneBase("InGame",3,3)
{
}

Scene_SimCityInGame::~Scene_SimCityInGame()
{
}

bool Scene_SimCityInGame::Initialize()
{
	SetLayerViewIndex(0, 0);
	SetLayerViewIndex(1, 1);
	SetLayerViewIndex(2, 2);

	m_Cursor = AddGameObject(2, new SimCityCursor("background/1013.png", 2));

	m_Tile = AddGameObject(0, new Tile());
	m_Tile->SetCellSize({ 45,45 });
	m_TileGrid = AddGameObject(1, new TileGrid());
	m_TileGrid->SetCellSize({ 45,45 });

	return true;
}

void Scene_SimCityInGame::Enter()
{
}

void Scene_SimCityInGame::Update(float dt)
{

	if (INPUT_MGR->GetKey(sf::Keyboard::Num1))
	{

	}
}
