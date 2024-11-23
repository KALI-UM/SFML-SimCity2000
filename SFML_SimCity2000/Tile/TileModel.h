#pragma once
#include "Tile.h"

//MCV-Model
//타일 데이터
//타일 이미지가 변경되면 뷰에게 알려야한다.
class TileController;
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
	const int m_MaxDepth = 4;

	void InitializeTerrainDepth();
	void InitializeOnGroundDepth();
	void InitilaizeEffectDepth();

	void SetTileUpdateFunc(std::function<void(const TileDepth&, const CellIndex&)> func) { m_WhenNeedsToUpdateTileFunc = func; };
	void SetTempEffectTileUpdateFunc(std::function<void(const CellIndex&)> func) { m_WhenNeedsToUpdateTempEffectTileFunc = func; };

	bool IsValidTileIndex(const CellIndex& tileIndex) const;
	const TileInfo& GetTileInfo(const TileDepth& depth, const CellIndex& tileIndex) const;
	TileShapeType GetTileShapeType(const TileDepth& depth, const CellIndex& tileIndex) const;

	void SetTiles(std::list<CellIndex>& tiles, TileType type, const std::string& subtype, const std::string& name, bool isNot1x1 = false);
	void SetTempEffectTiles(const CellIndex& tileIndex, TileType type, const std::string& subtype, const std::string& name);
protected:
	void SetTile(const CellIndex& tileIndex, const TileDepth& depth, TileType type, const std::string& subtype, const std::string& name, bool isConnectable = false, bool truetile = true);
public:
	bool IsPossibleToBuild(const CellIndex& tileIndex, const TileType& type, const SUBTYPE& subtype);
protected:
	std::vector<std::vector<std::vector<TileInfo>>>		m_TileInfos;

	std::function<void(const TileDepth&, const CellIndex&)> m_WhenNeedsToUpdateTileFunc;
	void RequestUpdateTile(const TileDepth& depth, const CellIndex& tileIndex);
	std::function<void(const CellIndex&)> m_WhenNeedsToUpdateTempEffectTileFunc;
	void RequestTempEffectTile(const CellIndex& tileIndex);

	void SetConnection(const CellIndex& tileIndex, const TileDepth& depth, TileType type);
	void SetConnectionPowerline(const CellIndex& tileIndex, const TileDepth& depth);
	std::string GetConnectedTileName(std::string& name, int connection);

	int i = 0;
};

