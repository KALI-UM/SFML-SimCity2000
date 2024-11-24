#include "pch.h"
#include "Scene_SimCityInGame.h"
#include "SimCityGameSystem.h"
#include "SimCityCursor.h"
#include "SimCityMenuBar.h"
#include "SimCityButtonBar.h"
#include "TileGrid.h"
#include "Tile.h"
#include "TileModel.h"
#include "TileController.h"
#include "TileView.h"
#include "TileViewChild.h"

Scene_SimCityInGame::Scene_SimCityInGame()
	:SceneBase("InGame", 3, 2)
{
}

Scene_SimCityInGame::~Scene_SimCityInGame()
{
}

bool Scene_SimCityInGame::Initialize()
{
	SetLayerViewIndex(0, 0);
	SetLayerViewIndex(1, 0);
	SetLayerViewIndex(2, 0);
	SetViewNeedPriority(0, false);


	//m_Tile = AddGameObject(0, new Tile());
	//m_Tile->SetCellSize({ 45,45 });
	m_TileGrid = AddGameObject(1, new TileGrid());
	m_TileGrid->SetIsVisible(false);
	m_TileGrid->SetCellCount({ 128,128 });
	m_TileGrid->SetCellSize({ 45,45 });

	m_TileModel = AddGameObject(0, new TileModel({ 128,128 }, { 45,45 }));
	m_TileView = AddGameObject(0, new TileView(m_TileModel));
	m_TileView->SetDepthView(TileDepth::Terrain, AddGameObject(0, new TileViewChild(m_TileView)));
	m_TileView->SetDepthView(TileDepth::OnGround, AddGameObject(1, new TileViewChild(m_TileView)));
	m_TileView->SetDepthView(TileDepth::Effect, AddGameObject(2, new TileViewChild(m_TileView)));
	m_GameSystem = AddGameObject(m_UILayerIndex, new SimCityGameSystem(m_TileModel));
	m_TileController = AddGameObject(m_UILayerIndex, new TileController(m_GameSystem, m_TileModel, m_TileView, 0));

	m_Cursor = AddGameObject(m_UILayerIndex, new SimCityCursor("ui/cursor.png", m_UIViewIndex));
	m_MenuBar = AddGameObject(m_UILayerIndex, new SimCityMenuBar(m_UIViewIndex));
	m_ButtonBar = AddGameObject(m_UILayerIndex, new SimCityButtonBar(m_UIViewIndex));
	m_TileController->SetMenuBar(m_MenuBar);
	m_TileController->SetButtonBar(m_ButtonBar);
	m_TileController->SetCusor(m_Cursor);
	return true;
}

void Scene_SimCityInGame::Enter()
{

	SOUND_MGR->PlayBgm("sound/BGM/bgm.mp3", true, true, 30, 10, true);
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

}

void Scene_SimCityInGame::ShowSceneImgui()
{
	sf::Vector2i tileIndex = m_TileController->GetMouseOverlaidTileIndex();
	sf::Vector2i clickedIndex = m_TileController->GetMousePrevTileIndex();
	//sf::Vector2i clickedIndex = m_TileView->GetTileCoordinatedIndex(GAME_MGR->GetScreenToViewPos(0, INPUT_MGR->GetPrevMouseDown(sf::Mouse::Left)));
	ImGui::Begin("Tile Menu");
	std::string forCurrIndex = "Current Index : {" + std::to_string(tileIndex.x) + ", " + std::to_string(tileIndex.y) + "}";
	ImGui::Text(forCurrIndex.c_str());
	std::string forPrevIndex = "Prev Index : {" + std::to_string(clickedIndex.x) + ", " + std::to_string(clickedIndex.y) + "}";
	ImGui::Text(forPrevIndex.c_str());

	/*ImGui::Image()*/

	//if (ImGui::Button("Road"))
	//{
	//	m_TileController->SetCurrButton(TileSet::Road);
	//}
	//if (ImGui::Button("Powerline"))
	//{
	//	m_TileController->SetCurrButton(TileSet::Powerlink);
	//}
	//if (ImGui::Button("Zone"))
	//{
	//	m_TileController->SetCurrButton(TileSet::Zone_C);
	//}
	//if (ImGui::Button("PowerPlace"))
	//{
	//	m_TileController->SetCurrButton(TileSet::Powerplant);
	//}
	//if (ImGui::Button("Destroy"))
	//{
	//	m_TileController->SetCurrButton(TileSet::Destroy);
	//}
	if (ImGui::Button("Save"))
	{
		m_GameSystem->SaveTileDepthFile();
	}
	//if(ImGui::ImageButton())
	ImGui::End();
}
