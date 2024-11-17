#include "pch.h"
#include "Tile.h"
#include "DTile.h"
#include "TileResTable.h"

Tile::Tile()
{
}

Tile::~Tile()
{
}

bool Tile::Initialize()
{
	m_TileInfos = std::vector<std::vector<TileInfo>>(m_CellCount.y, std::vector<TileInfo>(m_CellCount.x));

	rapidcsv::Document doc("datatables/tiletest.csv", rapidcsv::LabelParams(-1, -1));
	int row = doc.GetRowCount();
	int col = doc.GetColumnCount();
	for (int j = 0; j < m_CellCount.y; j++)
	{
		for (int i = 0; i < m_CellCount.x; i++)
		{
			auto& currTileInfo = m_TileInfos[j][i];
			currTileInfo.index = { i,j };
			currTileInfo.type = (TileType)doc.GetCell<int>(i * 2, j);
			currTileInfo.name = doc.GetCell<std::string>(i * 2 + 1, j);
			currTileInfo.filepath = DATATABLE_TILERES->GetTileFilePath(GetTypeToString(currTileInfo.type), m_TileInfos[j][i].name);
			DTile* tileSprite = new DTile(currTileInfo.filepath);
			tileSprite->SetLot({ 1, 1 });
			tileSprite->SetDebugDraw(false);
			tileSprite->SetOrigin(OriginType::BC, { -1.0f,-3.0f });
			tileSprite->setLocalPosition({ (i)*m_CellSize.x, (j + 1) * m_CellSize.y });
			SetDrawable(tileSprite);
		}
	}

	return false;
}

void Tile::Reset()
{
}

void Tile::Update(float dt)
{
	ResetColorizedTile();

	sf::Vector2i tileindex = GetTileCoordinatedIndex(INPUT_MGR->GetMouseViewPos(0));

	if (INPUT_MGR->GetMouseDown(sf::Mouse::Left))
	{
		clickindex = tileindex;
	}

	if (INPUT_MGR->GetMouseDrag(sf::Mouse::Left) && tileindex != clickindex && IsValidTileIndex(tileindex) && IsValidTileIndex(clickindex))
	{
		SetLineIntersectedTiles(tileindex, clickindex);
	}
	else if (IsValidTileIndex(tileindex))
	{
		ColorizeTile(ColorPalette::Gray, tileindex);
	}

	if (INPUT_MGR->GetMouseUp(sf::Mouse::Left))
	{
		SetTiles(m_SelectingTiles, TileType::Road);
	}
}

void Tile::SetTileTransform(const sf::Vector2f& zero, const sf::Transform& trans)
{
	m_TileTransform = trans;
	setPosition(zero);
	for (auto& tile : m_Drawable)
	{
		tile->setLocalPosition(m_TileTransform.transformPoint(tile->getLocalPosition()));
	}
}

sf::Vector2f Tile::GetTileCoordinatedPos(const sf::Vector2f& pos) const
{
	return m_TileTransform.getInverse().transformPoint(pos);
}

sf::Vector2i Tile::GetTileCoordinatedIndex(const sf::Vector2f& pos, bool isTilepos) const
{
	sf::Vector2f tilepos = isTilepos ? pos : GetTileCoordinatedPos(pos);
	return sf::Vector2i(tilepos.x / m_CellSize.x, tilepos.y / m_CellSize.y);
}

sf::Vector2f Tile::GetTileCoordinatedCenterPosByTileIndex(const CellIndex& tileIndex)
{
	return 1.5f * sf::Vector2f(m_CellSize.x * tileIndex.x, m_CellSize.y * tileIndex.y);
}

bool Tile::IsValidTileIndex(const CellIndex& tileIndex) const
{
	return tileIndex.x > 0 && tileIndex.x < m_CellCount.x - 1 && tileIndex.y > 0 && tileIndex.y < m_CellCount.y - 1;
}

void Tile::TileSpriteUpdate(std::list<CellIndex>& tiles)
{
	for (auto& currIndex : tiles)
	{
		DTile* currTile = dynamic_cast<DTile*>(GetDrawableObj(GetDrawableIndexByTileIndex(currIndex)));

		currTile->SetTexture(m_TileInfos[currIndex.y][currIndex.x].filepath, { 1,1 });
	}
}

std::string Tile::GetTypeToString(TileType type) const
{
	switch (type)
	{
	case TileType::Terrain:
		return "terrain";
		break;
	case TileType::Building:
		return "building";
		break;
	case TileType::Zone:
		return "zone";
		break;
	case TileType::Road:
		return "road";
		break;
	case TileType::Rail:
		return "rail";
		break;
	case TileType::Highway:
		return "highway";
		break;
	case TileType::Underground:
		return "underground";
		break;
	case TileType::Other:
		return "other";
		break;
	}
}

TileType Tile::GetTypeToEnum(const std::string& type) const
{
	if (type == "")
		return TileType::None;
	if (type == "terrain")
		return TileType::Terrain;
	if (type == "building")
		return TileType::Building;
	if (type == "zone")
		return TileType::Zone;
	if (type == "road")
		return TileType::Road;
	if (type == "rail")
		return TileType::Rail;
	if (type == "highway")
		return TileType::Highway;
	if (type == "underground")
		return TileType::Underground;
	if (type == "other")
		return TileType::Other;

	return TileType::None;
}

int Tile::GetDrawableIndexByTileIndex(const CellIndex& tileIndex) const
{
	return tileIndex.x + tileIndex.y * m_CellCount.x;
}

void Tile::ColorizeTile(const sf::Color& color, const CellIndex& tileIndex)
{
	auto tile = GetDrawableObj(GetDrawableIndexByTileIndex(tileIndex));
	tile->SetFillColor(color);
	m_ColorizedTiles.push(tile);
}

void Tile::ResetColorizedTile()
{
	int size = 0;
	while (!m_ColorizedTiles.empty())
	{
		auto tile = m_ColorizedTiles.front();
		tile->SetFillColor(ColorPalette::White);
		m_ColorizedTiles.pop();
	}
}

void Tile::PushToSelectingTiles(const CellIndex& tileIndex, TileType type)
{
	m_SelectingTiles.push_back(tileIndex);
	ColorizeTile(ColorPalette::Gray, tileIndex);
	//IsPossibleToBuild(tileIndex, type) ? ColorizeTile(ColorPalette::Gray, tileIndex) : ColorizeTile(ColorPalette::Red, tileIndex);
}

void Tile::SetLineIntersectedTiles(const CellIndex& startIndex, const CellIndex& endIndex)
{
	m_SelectingTiles.clear();

	sf::Vector2i slope = startIndex - endIndex;
	sf::Vector2i currIndex = startIndex;
	int x0 = startIndex.x;
	int y0 = startIndex.y;
	int x1 = endIndex.x;
	int y1 = endIndex.y;

	//Bresenham's Line Algorithm
	//선의 기울기는 1과 0사이
	//x를 증가시킬 때마다 y 


	int dx = abs(slope.x);
	int dy = abs(slope.y);
	int sx = (startIndex.x < endIndex.x) ? 1 : -1;
	int sy = (startIndex.y < endIndex.y) ? 1 : -1;

	//slope err보정값
	int err = dx - dy;

	while (true)
	{
		// 현재 타일 색칠
		PushToSelectingTiles(currIndex, TileType::Road);
		if (currIndex == endIndex)break;

		int e2 = 2 * err;
		bool needToMoveX = e2 > -dy;
		// x 방향으로 이동
		if (needToMoveX)
		{
			err -= dy;
			currIndex.x += sx;
		}

		bool needToMoveY = e2 < dx;
		// y 방향으로 이동
		if (needToMoveY)
		{
			err += dx;
			currIndex.y += sy;

			if (needToMoveX)
			{
				PushToSelectingTiles({ currIndex.x - sx, currIndex.y }, TileType::Road);
				//PushToSelectingTiles({ currIndex.x, currIndex.y - sy }, TileType::Road);
			}
		}
	}
}

bool Tile::SetTiles(std::list<CellIndex>& tiles, TileType type)
{

	for (auto& currIndex : tiles)
	{
		if (!IsPossibleToBuild(currIndex, type))
			return false;
	}

	for (auto& currIndex : tiles)
	{
		auto& currTileInfo = m_TileInfos[currIndex.y][currIndex.x];
		currTileInfo.type = type;
	}

	for (auto& currIndex : tiles)
	{
		auto& currTileInfo = m_TileInfos[currIndex.y][currIndex.x];

		int connect = 0;
		//시작지점이거나 끝지점이면 한면만 이어져 있더라도 두면이 이어져 있는 타일로
		if (currIndex == *tiles.begin() || currIndex == *(--tiles.end()))
		{
			if (currTileInfo.type == m_TileInfos[currIndex.y][currIndex.x - 1].type || currTileInfo.type == m_TileInfos[currIndex.y][currIndex.x + 1].type)
			{
				connect |= 1 << 0;
				connect |= 1 << 1;
			}
			else
			{
				connect |= 1 << 2;
				connect |= 1 << 3;
			}
		}
		else
		{
			if (currTileInfo.type == m_TileInfos[currIndex.y][currIndex.x - 1].type)
				connect |= 1 << 0;
			if (currTileInfo.type == m_TileInfos[currIndex.y][currIndex.x + 1].type)
				connect |= 1 << 1;
			if (currTileInfo.type == m_TileInfos[currIndex.y - 1][currIndex.x].type)
				connect |= 1 << 2;
			if (currTileInfo.type == m_TileInfos[currIndex.y + 1][currIndex.x].type)
				connect |= 1 << 3;
		}

		currTileInfo.name = "road_" + GetConnectedTileName(connect);
		currTileInfo.filepath = DATATABLE_TILERES->GetTileFilePath(GetTypeToString(currTileInfo.type), currTileInfo.name);
	}

	TileSpriteUpdate(tiles);
	tiles.clear();
	return true;
}

std::string Tile::GetConnectedTileName(int connection)
{
	if (connection == 3)return "1";
	if (connection == 12)return "2";
	if (connection == 72)return "3";
	if (connection == 33)return "4";
	if (connection == 132)return "5";
	if (connection == 18)return "6";
	if (connection == 10)return "7";
	if (connection == 9)return "8";
	if (connection == 5)return "9";
	if (connection == 6)return "10";
	if (connection == 14)return "11";
	if (connection == 11)return "12";
	if (connection == 13)return "13";
	if (connection == 7)return "14";
	if (connection == 15)return "15";
	return "";
}

bool Tile::IsPossibleToBuild(const CellIndex& tileIndex, TileType type)
{
	return m_TileInfos[tileIndex.y][tileIndex.x].type == TileType::Terrain || m_TileInfos[tileIndex.y][tileIndex.x].type == type;
}

