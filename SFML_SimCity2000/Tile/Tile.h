#pragma once

typedef sf::Vector2i CellIndex;
typedef std::string TYPE;
typedef std::string SUBTYPE;
typedef std::string NAME;
typedef int ID;

struct TileResData
{
	int id;
	std::string filename;
	TYPE type;
	SUBTYPE subtype;
	NAME name;

	sf::Vector2u	lotSize;
	bool			animated;
};

enum class TileType
{
	Terrain,
	Building,
	Zone,
	Road,
	Rail,
	Powerline,
	Highway,
	Underground,
	Other,
	None,
};

struct TileTypeAttribute
{
	std::string type;
	int			depth;
	bool		connectable;
	std::unordered_set<SUBTYPE> canbesub;
};

enum class TileDepth
{
	UnderGround,
	Terrain,
	OnGround,
};


struct TileInfo
{
	CellIndex index;

	TileType type = TileType::None;
	SUBTYPE subtype;
	std::string name;
	std::string filepath;
	sf::Vector2u lotSize;
	int connection;
	bool waterSupply;
	bool electricSupply;
	TileType prevtype = TileType::None;
};

class Tile
{
public:
	static std::string GetTypeToString(TileType type);
	static TileType GetTypeToEnum(const std::string& type);

};