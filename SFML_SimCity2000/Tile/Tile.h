#pragma once


typedef sf::Vector2i CellIndex;

enum class TileType
{
	Terrain,
	Building,
	Zone,
	Road,
	Rail,
	Highway,
	Underground,
	Other,
	None,
};

struct TileInfo
{
	TileType type = TileType::None;
	std::string name;
	std::string filepath;
	sf::Vector2u lotSize;

	CellIndex index;
};

enum class TileDepth
{
	UnderGround,
	Terrain,
	OnGround,
};

class Tile :
	public GameObject
{
public:
	Tile();
	~Tile();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;
	//void LateUpdate(float dt) override;
	//void FixeUpdate(float dt) override;
	//void Release() override;
	void SetTileTransform(const sf::Vector2f& zero, const sf::Transform& trans);
	void SetCellSize(const sf::Vector2f& cell) { m_CellSize = cell; }

	sf::Vector2f GetTileCoordinatedPos(const sf::Vector2f& pos) const;
	CellIndex GetTileCoordinatedIndex(const sf::Vector2f& pos, bool isTilepos = false) const;
	sf::Vector2f GetTileCoordinatedCenterPosByTileIndex(const CellIndex& tileIndex);

	bool IsValidTileIndex(const CellIndex& tileIndex) const;

protected:
	std::vector<std::vector<TileInfo>> m_TileInfos;

	sf::Vector2f m_CellSize = { 100.f,100.f };
	sf::Vector2u m_CellCount = { 100,100 };
	sf::Transform m_TileTransform;

	void TileSpriteUpdate(std::list<CellIndex>& tiles);

	std::string GetTypeToString(TileType type) const;
	TileType GetTypeToEnum(const std::string& type)const;

	int GetDrawableIndexByTileIndex(const CellIndex& tileIndex) const;

	std::queue<DrawableObject*> m_ColorizedTiles;
	void ColorizeTile(const sf::Color& color, const CellIndex& tileIndex);
	void ResetColorizedTile();

	CellIndex clickindex;

	std::list<CellIndex> m_SelectingTiles;
	void PushToSelectingTiles(const CellIndex& tileIndex, TileType type);
	void SetLineIntersectedTiles(const CellIndex& startIndex, const CellIndex& endIndex);

	bool SetTiles(std::list< CellIndex>& tiles, TileType type);
	std::string GetConnectedTileName(int connection);

	bool IsPossibleToBuild(const CellIndex& tileIndex, TileType type);
};

