#include "pch.h"
#include "TileView.h"
#include "TileModel.h"
#include "TileViewChild.h"
#include "DTile.h"

TileView::TileView(TileModel* model)
	:mvc_Model(model)
{
	mvc_Model->SetTileUpdateFunc(std::bind(&TileView::PushToSpriteUpdateQue, this, std::placeholders::_1, std::placeholders::_2));
	mvc_Model->SetTempEffectTileUpdateFunc(std::bind(&TileView::PushToTempEffectUpdateQue, this, std::placeholders::_1));
	m_DepthViews.resize(mvc_Model->m_MaxDepth);
}

TileView::~TileView()
{
}

bool TileView::Initialize()
{
	if (!mvc_Model)return false;

	return true;
}

void TileView::Reset()
{

}

void TileView::Update(float dt)
{
}

void TileView::LateUpdate(float dt)
{
	UpdateTileSprite();
}

void TileView::PostRender()
{
	ResetColorizedTile();
	ResetTempEffectTile();
}

void TileView::Release()
{
}

void TileView::SetTileTransform(const sf::Vector2f& zero, const sf::Transform& trans)
{
	m_TileTransform = trans;
	setPosition(zero);
	for (auto& depth : m_DepthViews)
	{
		if (depth)
			depth->SetTileTransform(trans);
	}
}

void TileView::SetDepthView(const TileDepth& depth, TileViewChild* child)
{
	GameObject::SetChildObj((GameObject*)child);
	m_DepthViews[(int)depth] = child;
	child->m_Depth = depth;
}

//좌표를 타일 좌표계로 변환
sf::Vector2f TileView::GetTileCoordinatedPos(const sf::Vector2f& pos) const
{
	return m_TileTransform.getInverse().transformPoint(pos);
}

//타일 좌표계로 변환된 Position을 타일 크기로 나눠 타일 인덱스를 구한다.
CellIndex TileView::GetTileCoordinatedIndex(const sf::Vector2f& pos, bool isTilepos) const
{
	sf::Vector2f tilepos = isTilepos ? pos : GetTileCoordinatedPos(pos);
	return sf::Vector2i(tilepos.x / mvc_Model->m_CellSize.x, tilepos.y / mvc_Model->m_CellSize.y);
}

sf::Vector2f TileView::GetTileCoordinatedCenterPosByTileIndex(const CellIndex& tileIndex)
{
	return 1.5f * sf::Vector2f(mvc_Model->m_CellSize.x * tileIndex.x, mvc_Model->m_CellSize.y * tileIndex.y);
}

int TileView::GetDrawableIndexByTileIndex(const CellIndex& tileIndex) const
{
	return tileIndex.x + tileIndex.y * mvc_Model->m_CellCount.x;
}

void TileView::ColorizeTile(const sf::Color& color, const CellIndex& tileIndex)
{
	if (!mvc_Model->IsValidTileIndex(tileIndex))
		return;

	auto tile = m_DepthViews[(int)TileDepth::Terrain]->m_TileDrawable[tileIndex.y][tileIndex.x];
	tile->SetFillColor(color);
	m_ColorizedTiles.push(tileIndex);
}

void TileView::ColorizeTile(const sf::Color& color, const sf::Vector2u& lot, const CellIndex& centerIndex)
{
	CellIndex startIndex;
	if (lot == sf::Vector2u(1, 1))
		startIndex = centerIndex;
	else if (lot == sf::Vector2u(2, 2))
		startIndex = centerIndex + sf::Vector2i(-1, 0);
	else if (lot == sf::Vector2u(3, 3))
		startIndex = centerIndex + sf::Vector2i(-1, -1);
	else if (lot == sf::Vector2u(4, 4))
		startIndex = centerIndex + sf::Vector2i(-2, -1);

	for (int j = startIndex.y; j < startIndex.y + lot.y; j++)
	{
		for (int i = startIndex.x; i < startIndex.x + lot.x; i++)
		{
			CellIndex currIndex = { i,j };
			ColorizeTile(color, currIndex);
		}
	}
}

void TileView::ResetColorizedTile()
{
	while (!m_ColorizedTiles.empty())
	{
		auto tile = m_DepthViews[(int)TileDepth::Terrain]->m_TileDrawable[m_ColorizedTiles.front().y][m_ColorizedTiles.front().x];
		tile->SetFillColor(ColorPalette::White);
		m_ColorizedTiles.pop();
	}
}

void TileView::ResetTempEffectTile()
{
	if (!m_TempEffectTiles.empty())
		mvc_Model->SetTiles(m_TempEffectTiles, TileType::Other, "", "");
	m_TempEffectTiles.clear();
}

void TileView::PushToSpriteUpdateQue(const TileDepth& depth, const CellIndex& tileIndex)
{
	m_SpriteUpdateQueue.push({ depth, tileIndex });
}

void TileView::PushToTempEffectUpdateQue(const CellIndex& tileIndex)
{
	PushToSpriteUpdateQue(TileDepth::Effect, tileIndex);
	m_TempEffectTiles.push_back(tileIndex);
}

void TileView::UpdateTileSprite()
{
	while (!m_SpriteUpdateQueue.empty())
	{
		TileDepth& currdepth = m_SpriteUpdateQueue.front().first;
		sf::Vector2i& currIndex = m_SpriteUpdateQueue.front().second;
		m_DepthViews[(int)currdepth]->NeedPriorityUpdate();
		auto& currTile = m_DepthViews[(int)currdepth]->m_TileDrawable[currIndex.y][currIndex.x];
		auto& currTileInfo = mvc_Model->GetTileInfo(currdepth, currIndex);
		currTile->SetTexture(currTileInfo.filepath, mvc_Model->GetTileShapeType(currdepth, currIndex), currTileInfo.lotSize);
		currTile->SetOrigin(OriginType::BC, m_TileOffset);
		currTile->SetTilePriorityValue();
		m_SpriteUpdateQueue.pop();
	}
}
