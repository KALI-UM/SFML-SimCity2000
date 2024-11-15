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
	return false;
}

void Tile::Reset()
{
}

void Tile::Update(float dt)
{
	ResetColorizedTile();

	sf::Vector2i tileindex = GetTileCoordinatedIndex(INPUT_MGR->GetMouseViewPos(0));

	if (tileindex.x >= 0 && tileindex.x < m_CellCount.x && tileindex.y >= 0 && tileindex.y < m_CellCount.y)
		ColorizeTile(ColorPalette::Gray, tileindex);
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

sf::Vector2i Tile::GetTileCoordinatedIndex(const sf::Vector2f& pos) const
{
	sf::Vector2f tilepos = GetTileCoordinatedPos(pos);
	return sf::Vector2i(tilepos.x/m_CellSize.x, tilepos.y/m_CellSize.y);
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
	while (!m_ColorizedTile.empty())
	{
		auto tile = m_ColorizedTile.front();
		tile->SetFillColor(ColorPalette::White);
		m_ColorizedTile.pop();
	}
}
