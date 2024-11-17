#pragma once
#include "GameObject.h"
class TileGrid :
	public GameObject
{
public:
	TileGrid();
	~TileGrid();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;
	//void LateUpdate(float dt) override;
	//void FixeUpdate(float dt) override;
	//void Release() override;

	void SetCellSize(const sf::Vector2f& cell) { m_CellSize = cell; }
	void SetTileTransform(const sf::Vector2f& zero, const sf::Transform& trans);

protected:
	sf::Vector2f m_CellSize = { 100.f,100.f };
	sf::Transform m_TileTransform;

};

