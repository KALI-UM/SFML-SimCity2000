#pragma once
#include "Tile.h"

class TileModel;
class TileViewChild;
class DTile;
class TileView :
	public GameObject
{
protected:
	TileModel* mcv_Model;
	std::vector<TileViewChild*> m_DepthViews;
public:
	const TileModel* const GetModel()const { return mcv_Model; }
	const sf::Vector2f m_TileOffset = { -1,0 };
public:
	TileView(TileModel* model);
	~TileView();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	//void FixeUpdate(float dt) override;
	void PostRender();
	void Release() override;

	void			SetTileTransform(const sf::Vector2f& zero, const sf::Transform& trans);
	sf::Transform	GetTileTransform() const { return m_TileTransform; }
	void SetDepthView(const TileDepth& depth, TileViewChild* child);


	sf::Vector2f	GetTileCoordinatedPos(const sf::Vector2f& pos) const;
	CellIndex		GetTileCoordinatedIndex(const sf::Vector2f& pos, bool isTilepos = false) const;
	sf::Vector2f	GetTileCoordinatedCenterPosByTileIndex(const CellIndex& tileIndex);
	int				GetDrawableIndexByTileIndex(const CellIndex& tileIndex) const;

	void ColorizeTile(const sf::Color& color, const CellIndex& tileIndex);
	void ColorizeTile(const sf::Color& color, const sf::Vector2u& lot, const CellIndex& centerIndex);

protected:
	sf::Transform	m_TileTransform;


	void ResetColorizedTile();
	void ResetTempEffectTile();
	std::queue<CellIndex> m_ColorizedTiles;


	void PushToSpriteUpdateQue(const TileDepth& depth, const CellIndex& tileIndex);
	std::queue<std::pair<TileDepth, CellIndex>> m_SpriteUpdateQueue;
	void PushToTempEffectUpdateQue(const CellIndex& tileIndex);
	std::list<CellIndex> m_TempEffectTiles;

	void UpdateTileSprite();
};

