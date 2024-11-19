#include "pch.h"
#include "Scene_SimCityInGame.h"
#include "SimCityCursor.h"
#include "TileGrid.h"
#include "Tile.h"
#include "TileModel.h"
#include "TileController.h"
#include "TileView.h"
#include "TileViewChild.h"

Scene_SimCityInGame::Scene_SimCityInGame()
	:SceneBase("InGame", 3, 3)
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

	//m_Tile = AddGameObject(0, new Tile());
	//m_Tile->SetCellSize({ 45,45 });
	m_TileGrid = AddGameObject(1, new TileGrid());
	m_TileGrid->SetCellCount({ 128,128 });
	m_TileGrid->SetCellSize({ 45,45 });

	m_TileModel = AddGameObject(0, new TileModel({ 128,128 }, { 45,45 }));
	m_TileView = AddGameObject(0, new TileView(m_TileModel));
	m_TileView->SetDepthView(TileDepth::Terrain, AddGameObject(0, new TileViewChild(m_TileView)));
	m_TileView->SetDepthView(TileDepth::OnGround, AddGameObject(1, new TileViewChild(m_TileView)));
	m_TileController = AddGameObject(0, new TileController(m_TileModel, m_TileView, 0));

	return true;
}

void Scene_SimCityInGame::Enter()
{
	SOUND_MGR->PlayBgm("sound/BGM/bgm.mp3", true, true, 30, 10);
	GAME_MGR->SetViewSize(0, { 0,0,(float)GAME_MGR->GetWindow()->getSize().x, (float)GAME_MGR->GetWindow()->getSize().y });
	GAME_MGR->SetViewSize(1, { 0,0,(float)GAME_MGR->GetWindow()->getSize().x, (float)GAME_MGR->GetWindow()->getSize().y });

	sf::Transform tileTransform;
	tileTransform.scale(1.0f, 0.5f);
	tileTransform.rotate(-45);

	//m_Tile->SetTileTransform({ 0,0 }, tileTransform);
	m_TileGrid->SetTileTransform({ 0,0 }, tileTransform);
	m_TileView->SetTileTransform({ 0,0 }, tileTransform);
}

void Scene_SimCityInGame::Update(float dt)
{

	sf::Vector2f moveoffset = { INPUT_MGR->GetAxisRaw(Axis::Horizontal) * dt * 100, -INPUT_MGR->GetAxisRaw(Axis::Vertical) * dt * 100 };
	GAME_MGR->MoveView(0, moveoffset);
	GAME_MGR->MoveView(1, moveoffset);


	if (INPUT_MGR->GetKey(sf::Keyboard::Num1))
	{

	}
}

void Scene_SimCityInGame::ShowSceneImgui()
{
	sf::Vector2i tileIndex = m_TileView->GetTileCoordinatedIndex(INPUT_MGR->GetMouseViewPos(0));
	sf::Vector2i clickedIndex = m_TileView->GetTileCoordinatedIndex(GAME_MGR->GetScreenToViewPos(0, INPUT_MGR->GetPrevMouseDown(sf::Mouse::Left)));
	ImGui::Begin("Tile Menu");
	std::string forCurrIndex = "Current Index : {" + std::to_string(tileIndex.x) + ", " + std::to_string(tileIndex.y) + "}";
	ImGui::Text(forCurrIndex.c_str());
	std::string forPrevIndex = "Prev Index : {" + std::to_string(clickedIndex.x) + ", " + std::to_string(clickedIndex.y) + "}";
	ImGui::Text(forPrevIndex.c_str());

	/*ImGui::Image()*/

	if (ImGui::Button("Road"))
	{
		m_TileController->SetCurrTile(TileType::Road, "road", "road_1");
	}
	if (ImGui::Button("Powerline"))
	{
		m_TileController->SetCurrTile(TileType::Powerline, "powerline", "powerline_1");
	}
	if (ImGui::Button("Zone"))
	{
		m_TileController->SetCurrTile(TileType::Zone, "", "zone_1");
	}
	if (ImGui::Button("PowerPlace"))
	{
		m_TileController->SetCurrTile(TileType::Building, "power_plant", "coal");
	}

	//if(ImGui::ImageButton())
	ImGui::End();
}
