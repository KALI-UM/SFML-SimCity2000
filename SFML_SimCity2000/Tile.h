#pragma once

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
};

struct TileInfo
{
	TileType type;
	std::string name;
	std::string filepath;
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
	sf::Vector2i GetTileCoordinatedIndex(const sf::Vector2f& pos, bool isTilepos = false) const;
	sf::Vector2f GetTileCoordinatedCenterPosByTileIndex(const sf::Vector2i& tileIndex);

	bool IsValidTileIndex(const sf::Vector2i& tileIndex) const;
	bool IsValidTileIndex(int x, int y) const;

protected:
	DLine* line;

	std::vector<std::vector<TileInfo>> m_TileInfos;

	sf::Vector2f m_CellSize = { 100.f,100.f };
	sf::Vector2u m_CellCount = { 100,100 };
	sf::Transform m_TileTransform;


	std::string GetTypeToString(TileType type) const;


	std::queue<DrawableObject*> m_ColorizedTile;
	void ColorizeTile(const sf::Color& color, const sf::Vector2i& cellIndex);
	void ResetColorizedTile();

	sf::Vector2i clickindex;
	void GetLineIntersectedTilesByTileIndex(const sf::Vector2i& startIndex, const sf::Vector2i& endIndex);
	

};

