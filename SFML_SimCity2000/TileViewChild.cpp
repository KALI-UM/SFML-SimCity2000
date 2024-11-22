#include "pch.h"
#include "TileViewChild.h"
#include "TileView.h"
#include "TileModel.h"
#include "DTile.h"

TileViewChild::TileViewChild(TileView* view)
	:mcv_View(view)
{
}

TileViewChild::~TileViewChild()
{
}

bool TileViewChild::Initialize()
{
	if (!mcv_View)return false;

	const auto& cellCount = mcv_View->GetModel()->m_CellCount;
	const auto& cellSize = mcv_View->GetModel()->m_CellSize;

	m_TileDrawable = std::vector<std::vector<DTile*>>(cellCount.y, std::vector<DTile*>(cellCount.x));
	for (int j = 0; j < cellCount.y; j++)
	{
		for (int i = 0; i < cellCount.x; i++)
		{
			auto& tileInfo = mcv_View->GetModel()->GetTileInfo(m_Depth, { i,j });
			DTile* tileSprite = new DTile(tileInfo.filepath);
			tileSprite->SetShapeLot(mcv_View->GetModel()->GetTileShapeType(m_Depth, { i,j }), tileInfo.lotSize);
			tileSprite->SetDebugDraw(false);
			tileSprite->SetOrigin(OriginType::BC, mcv_View->m_TileOffset);
			tileSprite->setLocalPosition({ (i)*cellSize.x, (j + 1) * cellSize.y });
			SetDrawable(tileSprite);
			m_TileDrawable[j][i] = tileSprite;
		}
	}
	return true;
}

void TileViewChild::Reset()
{
	const auto& cellCount = mcv_View->GetModel()->m_CellCount;
	const auto& cellSize = mcv_View->GetModel()->m_CellSize;

	for (int j = 0; j < cellCount.y; j++)
	{
		for (int i = 0; i < cellCount.x; i++)
		{
			auto& tileSprite = m_TileDrawable[j][i];
			auto& tileInfo = mcv_View->GetModel()->GetTileInfo(m_Depth, { i,j });
			tileSprite->SetShapeLot(mcv_View->GetModel()->GetTileShapeType(m_Depth, { i,j }), tileInfo.lotSize);
			tileSprite->SetDebugDraw(false);
			tileSprite->setLocalPosition({ (i)*cellSize.x, (j + 1) * cellSize.y });
			tileSprite->SetOrigin(OriginType::BC, mcv_View->m_TileOffset);
		}
	}
}

void TileViewChild::LateUpdate(float dt)
{
}

void TileViewChild::PostRender()
{
}

void TileViewChild::Release()
{
}

void TileViewChild::SetTileTransform(const sf::Transform& trans)
{
	for (auto& tile : m_Drawable)
	{
		tile->setLocalPosition(trans.transformPoint(tile->getLocalPosition()));
	}
}

