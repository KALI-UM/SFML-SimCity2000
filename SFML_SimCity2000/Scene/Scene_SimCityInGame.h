#pragma once

class SimCityCursor;
class TileGrid;
class Tile;

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




	SimCityCursor*		m_Cursor;
	TileGrid*			m_TileGrid;
	Tile*				m_Tile;

	sf::Vector2f		m_TilePosition;
};

