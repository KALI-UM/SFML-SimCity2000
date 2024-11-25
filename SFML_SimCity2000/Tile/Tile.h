#pragma once

typedef sf::Vector2i CellIndex;
typedef std::string TYPE;
typedef std::string SUBTYPE;
typedef std::string NAME;
typedef std::string ZONE;
typedef int ID;

struct TileResData
{
	int id;
	std::string filename;
	TYPE type;
	SUBTYPE subtype;
	NAME name;
	ZONE zone;
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
	Effect,
};


struct TileInfo
{
	CellIndex index;

	TileType type = TileType::None;
	SUBTYPE subtype;
	std::string name;
	std::string filepath;
	sf::Vector2u lotSize = {1,1};
	int connection=-1;
	TileType prevtype = TileType::None;
	CellIndex ower;
};

class Tile
{
public:
	static void Initialize();

	static std::string GetTypeToString(TileType type);
	static TileType GetTypeToEnum(const std::string& type);

	static std::string GetZoneToString(ZoneType type);
	static ZoneType GetZoneToEnum(const std::string& type);
	static std::string GetZoneToName(ZoneType type);
	static ZoneType GetNameToZone(const NAME& name);

	static const CellIndex d[8];

	static void SetLotSet();
	static std::vector<std::list<CellIndex>> lotSet;
};