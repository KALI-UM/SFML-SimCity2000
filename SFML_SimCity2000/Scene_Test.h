#pragma once

class TileGrid;
class Tile;
class MouseCursor;
class DustEffect;

class Scene_Test :
    public SceneBase
{
public:
    Scene_Test();
    ~Scene_Test();


protected:
	bool Initialize() override;
	//void Reset() override;
	void Enter() override;
	void Update(float dt) override;
	//void LateUpdate(float dt) override;
	//void FixedUpdate(float dt) override;
	//void Release() override;

	MouseCursor* m_Cursor;
	TileGrid* m_TileGrid;
	Tile* m_Tile;

	sf::Vector2f tilepos;

	ObjectPool<DustEffect> m_ObjectPool;
};




