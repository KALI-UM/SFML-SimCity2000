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
			const auto& tileInfo = mcv_View->GetModel()->GetTileInfo(m_Depth, { i,j });
			tileSprite->SetShapeLot(mcv_View->GetModel()->GetTileShapeType(m_Depth, { i,j }), tileInfo.lotSize);
			tileSprite->SetDebugDraw(false);
			tileSprite->SetOrigin(OriginType::BC, mcv_View->m_TileOffset);
		}
	}
}

void TileViewChild::LateUpdate(float dt)
{
}

void TileViewChild::PreRender()
{
	if (m_NeedPriorityUpdate)
	{
		//타일은 매 프레임마다 순위경쟁을 하지 않고 벡터에서 순서가 정해진다.
		std::sort(m_Drawables.begin(), m_Drawables.end(),
			[&](const DrawableObject* d1, const DrawableObject* d2) { return TileViewChild::SortTile(d1, d2); });
	}
}

void TileViewChild::PostRender()
{
}

void TileViewChild::Release()
{
}

void TileViewChild::SetTileTransform(const sf::Transform& trans)
{
	for (auto& tile : m_Drawables)
	{
		tile->setLocalPosition(trans.transformPoint(tile->getLocalPosition()));
	}
}

bool TileViewChild::SortTile(const DrawableObject* dobj1, const DrawableObject* dobj2) const
{
	return dobj1->GetPriorityValue() < dobj2->GetPriorityValue();
}

