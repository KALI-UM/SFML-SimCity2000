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

enum class TileShapeType
{
	Convex,
	Diamond,
	Rectangle,
	Max,
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


enum class ZoneType
{
	Residential,
	Commercial,
	Industrial,
	Military,
	Airport,
	Seaport,
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
	ZoneType zone = ZoneType::None;
	bool	isHighZone = false;
	SUBTYPE subtype;
	std::string name;
	std::string filepath;
	sf::Vector2u lotSize = {1,1};
	int connection=-1;
	TileType prevtype = TileType::None;
};

class Tile
{
public:
	static std::string GetTypeToString(TileType type);
	static TileType GetTypeToEnum(const std::string& type);

	static std::string GetZoneToString(ZoneType type);
	static ZoneType GetZoneToEnum(const std::string& type);

};