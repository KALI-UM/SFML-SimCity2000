#include "pch.h"
#include "Scene_Test.h"
#include "TileGrid.h"
#include "MouseCursor.h"
#include "DustEffect.h"

Scene_Test::Scene_Test()
	:SceneBase("Test", 3, 3)
{
}

Scene_Test::~Scene_Test()
{
}

bool Scene_Test::Initialize()
{
	SetLayerViewIndex(0, 0);
	SetLayerViewIndex(1, 1);
	SetLayerViewIndex(2, 2);
	m_Cursor = AddGameObject(2, new MouseCursor("background/1013.png", 2));

	m_TileGrid = AddGameObject(1, new TileGrid());
	m_TileGrid->SetCellSize({ 45,45 });
	m_ObjectPool.Initialize(this, 10, ExpandOption::MakeNew, 2);
	return true;
}

void Scene_Test::Enter()
{
	GAME_MGR->SetViewSize(0, { 0,0,(float)GAME_MGR->GetWindow()->getSize().x, (float)GAME_MGR->GetWindow()->getSize().y });
	GAME_MGR->SetViewSize(1, { 0,0,(float)GAME_MGR->GetWindow()->getSize().x, (float)GAME_MGR->GetWindow()->getSize().y });

	sf::Transform t;
	t.scale(1.0f, 0.5f);
	t.rotate(-45);

	m_TileGrid->SetTileTransform({ 0,0 }, t);

	void (SoundManager:: * func)(std::string, bool) = &SoundManager::PlaySfx;
	m_Cursor->SetMouseDownFunc(sf::Mouse::Left, std::bind(func, SOUND_MGR, "sound/ButtonClick.wav", false));
}

void Scene_Test::Update(float dt)
{
	m_ObjectPool.Update(dt);

	sf::Vector2f moveoffset = { INPUT_MGR->GetAxisRaw(Axis::Horizontal) * dt * 100, -INPUT_MGR->GetAxisRaw(Axis::Vertical) * dt * 100 };
	GAME_MGR->MoveView(0, moveoffset);
	GAME_MGR->MoveView(1, moveoffset);

	if (INPUT_MGR->GetMouseDown(sf::Mouse::Left))
	{
		m_ObjectPool.Take()->Effect(3, INPUT_MGR->GetMouseDefaultViewPos());
	}

}
