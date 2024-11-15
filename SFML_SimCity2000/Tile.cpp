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

	sf::Vector2f startCenter = GetTileCoordinatedCenterPosByTileIndex(startIndex);
	sf::Vector2f endCenter = GetTileCoordinatedCenterPosByTileIndex(endIndex);
	line->SetPoint(GAME_MGR->GetViewToScreenPos(0, m_TileTransform.transformPoint(startCenter)).To<float>(), GAME_MGR->GetViewToScreenPos(0, m_TileTransform.transformPoint(endCenter)).To<float>());

	sf::Vector2f slope = startCenter - endCenter;
	if (slope.x == 0)
	{
		for (int y = std::min(startIndex.y, endIndex.y); y <= std::max(startIndex.y, endIndex.y); y++)
		{
			ColorizeTile(ColorPalette::Gray, { startIndex.x,y });
		}
		return;
	}
	else if (slope.y == 0)
	{
		for (int x = std::min(startIndex.x, endIndex.x); x <= std::max(startIndex.x, endIndex.x); x++)
		{
			ColorizeTile(ColorPalette::Gray, { x,startIndex.y });
		}
		return;
	}

	float threshold = 5;
	sf::FloatRect grid(0, 0, m_CellSize.x, m_CellSize.y);
	for (int y = std::min(startIndex.y, endIndex.y); y <= std::max(startIndex.y, endIndex.y); y++)
	{
		for (int x = std::min(startIndex.x, endIndex.x); x <= std::max(startIndex.x, endIndex.x); x++)
		{
			grid.left = x * m_CellSize.x;
			grid.top = y * m_CellSize.y;

			int test = 0;
			// 타일의 네 경계 검사
			sf::Vector2f tx = { Utils::LineEquationX(startCenter, endCenter, grid.top), grid.top };               // 상단
			sf::Vector2f bx = { Utils::LineEquationX(startCenter, endCenter, grid.top + grid.height), grid.top + grid.height };  // 하단
			sf::Vector2f ly = { grid.left, Utils::LineEquationY(startCenter, endCenter, grid.left) };             // 왼쪽
			sf::Vector2f ry = { grid.left + grid.width, Utils::LineEquationY(startCenter, endCenter, grid.left + grid.width) };  // 오른쪽

			bool intersects = false;

			// 상단 경계와 교차
			if (grid.contains(tx)) intersects = true;
			// 하단 경계와 교차
			if (grid.contains(bx)) intersects = true;
			// 왼쪽 경계와 교차
			if (grid.contains(ly)) intersects = true;
			// 오른쪽 경계와 교차
			if (grid.contains(ry)) intersects = true;

			// 교차하는 타일이 있으면 색칠
			if (intersects) {
				ColorizeTile(sf::Color::Black, { x,y });

				////float topx = Utils::LineEquationX(startCenter, endCenter, grid.top);
				////if (topx >= grid.left && topx <= grid.left + grid.width)
				////	test++;
				////float bottomx = Utils::LineEquationX(startCenter, endCenter, grid.top + grid.height);
				////if (bottomx >= grid.left && bottomx <= grid.left + grid.width)
				////	test++;
				////float lefty = Utils::LineEquationY(startCenter, endCenter, grid.left);
				////if (lefty >= grid.top && lefty <= grid.top + grid.height)
				////	test++;
				////float righty = Utils::LineEquationY(startCenter, endCenter, grid.left + grid.width);
				////if (righty >= grid.top && righty <= grid.top + grid.height)
				////	test++;

				//sf::Vector2f tx = { Utils::LineEquationX(startCenter, endCenter, grid.top),grid.top };
				//sf::Vector2f bx = { Utils::LineEquationX(startCenter, endCenter, grid.top), grid.top + grid.height };
				//sf::Vector2f ly = { grid.left, Utils::LineEquationY(startCenter, endCenter, grid.left) };
				//sf::Vector2f ry = { grid.left + grid.width, Utils::LineEquationY(startCenter, endCenter, grid.left + grid.width) };

				//if (grid.contains(tx) || grid.contains(bx) || grid.contains(ly) || grid.contains(ry))
				//	test++;

				////if (Utils::CircleContainPoint(tx, threshold, { grid.left,grid.top }))
				////	test++;
				////if (Utils::CircleContainPoint(tx, threshold, { grid.left + grid.width,grid.top }))
				////	test++;

				////if (Utils::CircleContainPoint(bx, threshold, { grid.left,grid.top }))
				////	test++;

				////if (grid.contains(Utils::LineEquationX(startCenter, endCenter, grid.top), grid.top))
				////	test++;
				////if (grid.contains(Utils::LineEquationX(startCenter, endCenter, grid.top), grid.top + grid.height))
				////	test++;
				////if (grid.contains(grid.left, Utils::LineEquationY(startCenter, endCenter, grid.left)))
				////	test++;
				////if (grid.contains(grid.left + grid.width, Utils::LineEquationY(startCenter, endCenter, grid.left + grid.width)))
				////	test++;

				//if (test != 0)
				//	ColorizeTile(sf::Color::Black, { x,y });

			}
		}
	}

}

