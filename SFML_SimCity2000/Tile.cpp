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

	line = new DLine({ 0,0 }, { 0,0 }, ColorPalette::Cyan);
	line->SetDebugDraw(false);
	SetDrawable(line);
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
		line->SetPoint(INPUT_MGR->GetMouseViewPos(0), INPUT_MGR->GetMouseViewPos(0));
	}

	if (INPUT_MGR->GetMouseDrag(sf::Mouse::Left) && tileindex != clickindex && IsValidTileIndex(tileindex) && IsValidTileIndex(clickindex))
	{
		GetLineIntersectedTilesByTileIndex(tileindex, clickindex);
	}
	else if (IsValidTileIndex(tileindex))
	{
		//ColorizeTile(ColorPalette::Gray, tileindex);
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

sf::Vector2f Tile::GetTileCoordinatedCenterPosByTileIndex(const sf::Vector2i& tileIndex)
{
	return 1.5f * sf::Vector2f(m_CellSize.x * tileIndex.x, m_CellSize.y * tileIndex.y);
}

bool Tile::IsValidTileIndex(const sf::Vector2i& tileIndex) const
{
	return tileIndex.x >= 0 && tileIndex.x < m_CellCount.x && tileIndex.y >= 0 && tileIndex.y < m_CellCount.y;
}

bool Tile::IsValidTileIndex(int x, int y) const
{
	return IsValidTileIndex(x, y);
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

void Tile::ColorizeTile(const sf::Color& color, const sf::Vector2i& cellIndex)
{
	auto tile = GetDrawable(cellIndex.x + cellIndex.y * m_CellCount.x);
	tile->SetFillColor(color);
	m_ColorizedTile.push(tile);
}

void Tile::ResetColorizedTile()
{
	int size = 0;
	while (!m_ColorizedTile.empty())
	{
		auto tile = m_ColorizedTile.front();
		tile->SetFillColor(ColorPalette::White);
		m_ColorizedTile.pop();
	}
}

void Tile::GetLineIntersectedTilesByTileIndex(const sf::Vector2i& startIndex, const sf::Vector2i& endIndex)
{
	sf::Vector2i slope = endIndex - startIndex;
	int x0 = startIndex.x;
	int y0 = startIndex.y;
	int x1 = endIndex.x;
	int y1 = endIndex.y;

	//Bresenham's Line Algorithm
	//선의 기울기는 1과 0사이
	//x를 증가시킬 때마다 y 


	int dx = abs(slope.x);
	int dy = abs(slope.y);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;

	//slope err보정값
	int err = dx - dy;

	while (true)
	{
		// 현재 타일 색칠
		ColorizeTile(ColorPalette::Gray, {x0, y0});

		// 종료 조건: 시작점이 끝점에 도달하면 루프를 종료
		if (x0 == x1 && y0 == y1)
			break;

		int e2 = 2 * err;

		// x 방향으로 이동
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}

		// y 방향으로 이동
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}

}

