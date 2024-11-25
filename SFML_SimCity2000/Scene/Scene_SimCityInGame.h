#pragma once

class SimCityGameSystem;
class SimCityCursor;
class SimCityMenuBar;
class SimCityButtonBar;
class TileGrid;
class Tile;
class TileModel;
class TileController;
class TileView;

struct InGameInfo
{
	int rotation;		//0,1,2,3,
	int currSelect;		//-1 none
};

class Scene_SimCityInGame :
    public SceneBase
{
public:
	Scene_SimCityInGame();
	~Scene_SimCityInGame();


protected:
	bool Initialize() override;
	//void Reset() override;
	void Enter() override;
	void Update(float dt) override;
	void ShowSceneImgui() override;
	//void LateUpdate(float dt) override;
	//void FixedUpdate(float dt) override;
	//void Release() override;



	SimCityGameSystem*	m_GameSystem;
	SimCityCursor*		m_Cursor;
	SimCityMenuBar*		m_MenuBar;
	SimCityButtonBar*	m_ButtonBar;
	TileGrid*			m_TileGrid;
	Tile*				m_Tile;

	TileModel*			m_TileModel;
	TileController*		m_TileController;
	TileView*			m_TileView;

	sf::Vector2f		m_TilePosition;
};

