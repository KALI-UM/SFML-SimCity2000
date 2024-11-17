#pragma once
#include "Tile.h"

//MCV-Model
//타일 데이터
//타일 이미지가 변경되면 뷰에게 알려야한다.
class TileController;
class TileView;
class TileModel
	:public GameObject
{
public:
	TileModel(const sf::Vector2u& cellCnt = { 100,100 }, const sf::Vector2f& cellSize = { 100, 100 });
	~TileModel();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;
	//void LateUpdate(float dt) override;
	//void FixeUpdate(float dt) override;
	//void Release() override;

	const sf::Vector2f m_CellSize;
	const sf::Vector2u m_CellCount;
	const int m_MaxDepth = 3;

	void SetTileUpdateFunc(std::function<void(const TileDepth&, const CellIndex&)> func) { m_WhenNeedsToUpdateTileFunc = func; };

	bool IsValidTileIndex(const CellIndex& tileIndex) const;
	const TileInfo& GetTileInfo(const TileDepth& depth, const CellIndex& tileIndex) const;

	void SetTiles(std::list< CellIndex>& tiles, TileType type);
	void SetTile(const CellIndex& tileIndex, TileType type);

	TileDepth GetTileDepth(const TileType& type);
	bool IsPossibleToBuild(const CellIndex& tileIndex, TileType type);

protected:
	std::vector<std::vector<std::vector<TileInfo>>> m_TileInfos;


	std::string GetTypeToString(TileType type) const;
	TileType GetTypeToEnum(const std::string& type) const;
	int GetConnectVaule(const TileDepth& depth, const CellIndex& tileIndex, TileType type);
	std::string GetConnectedTileName(int connection);

	void RequestUpdateTile(const TileDepth& depth, const CellIndex& tileIndex);
	std::function<void(const TileDepth&, const CellIndex&)> m_WhenNeedsToUpdateTileFunc;
};

