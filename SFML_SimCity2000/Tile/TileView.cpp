#include "pch.h"
#include "TileView.h"
#include "TileModel.h"
#include "TileViewChild.h"
#include "DTile.h"

TileView::TileView(TileModel* model)
	:mcv_Model(model)
{
	mcv_Model->SetTileUpdateFunc(std::bind(&TileView::PushToSpriteUpdateQue, this, std::placeholders::_1, std::placeholders::_2));
	m_DepthViews.resize(mcv_Model->m_MaxDepth);
}

TileView::~TileView()
{
}

bool TileView::Initialize()
{
	if (!mcv_Model)return false;
	
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
		if(depth)
		depth->SetTileTransform(trans);
	}
}

void TileView::SetDepthView(const TileDepth& depth, TileViewChild* child)
{
	GameObject::SetChildObj((GameObject*)child);
	m_DepthViews[(int)depth] = child;
	child->m_Depth = depth;
}

sf::Vector2f TileView::GetTileCoordinatedPos(const sf::Vector2f& pos) const
{
	return m_TileTransform.getInverse().transformPoint(pos);
}

CellIndex TileView::GetTileCoordinatedIndex(const sf::Vector2f& pos, bool isTilepos) const
{
	sf::Vector2f tilepos = isTilepos ? pos : GetTileCoordinatedPos(pos);
	return sf::Vector2i(tilepos.x / mcv_Model->m_CellSize.x, tilepos.y / mcv_Model->m_CellSize.y);
}

sf::Vector2f TileView::GetTileCoordinatedCenterPosByTileIndex(const CellIndex& tileIndex)
{
	return 1.5f * sf::Vector2f(mcv_Model->m_CellSize.x * tileIndex.x, mcv_Model->m_CellSize.y * tileIndex.y);
}

int TileView::GetDrawableIndexByTileIndex(const CellIndex& tileIndex) const
{
	return tileIndex.x + tileIndex.y * mcv_Model->m_CellCount.x;
}

void TileView::ColorizeTile(const sf::Color& color, const CellIndex& tileIndex)
{
	auto tile = m_DepthViews[(int)TileDepth::Terrain]->m_TileDrawable[tileIndex.y][tileIndex.x];
	tile->SetFillColor(color);
	m_ColorizedTiles.push(tileIndex);
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

void TileView::PushToSpriteUpdateQue(const TileDepth& depth, const CellIndex& tileIndex)
{
	m_SpriteUpdateQueue.push({ depth, tileIndex });
}

void TileView::UpdateTileSprite()
{
	while (!m_SpriteUpdateQueue.empty())
	{
		TileDepth& currdepth = m_SpriteUpdateQueue.front().first;
		sf::Vector2i& currIndex = m_SpriteUpdateQueue.front().second;
		auto& currTile = m_DepthViews[(int)currdepth]->m_TileDrawable[currIndex.y][currIndex.x];
		currTile->SetTexture(mcv_Model->GetTileInfo(currdepth, currIndex).filepath, { 1,1 });
		m_SpriteUpdateQueue.pop();
	}
}
