#include "pch.h"
#include "TileGrid.h"
#include "DSprite.h"
#include "DLine.h"

TileGrid::TileGrid()
{
}

TileGrid::~TileGrid()
{
}

bool TileGrid::Initialize()
{
	std::vector<sf::Vector2f> rowPoints;
	std::vector<sf::Vector2f> colPoints;
	for (int i = 0; i < m_CellCount.x; i++)
	{
		rowPoints.push_back({ 0,					i * m_CellSize.y });
		rowPoints.push_back({ m_CellSize.x * m_CellCount.x,	i * m_CellSize.y });

	}
	for (int i = 0; i < (int)m_CellCount.y; i++)
	{
		colPoints.push_back({ i * m_CellSize.x,		0 });
		colPoints.push_back({ i * m_CellSize.x ,	m_CellSize.y * m_CellCount.y });
	}
	DLine* gridRow = new DLine(rowPoints.data(), rowPoints.size(), sf::Color::Red, false);
	DLine* gridCol = new DLine(colPoints.data(), colPoints.size(), sf::Color::Blue, false);
	SetDrawable(gridRow);
	SetDrawable(gridCol);

	return false;
}

void TileGrid::Reset()
{

}

void TileGrid::Update(float dt)
{
}

void TileGrid::SetTileTransform(const sf::Vector2f& zero, const sf::Transform& trans)
{
	m_TileTransform = trans;
	setPosition(zero);
	for (auto& tile : m_Drawables)
	{
		DLine* line = dynamic_cast<DLine*>(tile);
		for (int index = 0; index < line->GetPointsSize(); index++)
		{
			line->SetPoint(index, m_TileTransform.transformPoint(line->GetPoint(index)));
		}
	}
}
