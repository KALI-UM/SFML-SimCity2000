#pragma once
#include "Tile.h"

class TileView;
class DTile;
class TileViewChild
	:public GameObject
{
	friend class TileView;
protected:
	TileView* mcv_View;
	TileDepth m_Depth;
public:
	TileViewChild(TileView* view);
	~TileViewChild();

	bool Initialize() override;
	void Reset() override;
	//void Update(float dt) override;
	void LateUpdate(float dt) override;
	//void FixeUpdate(float dt) override;
	void PostRender();
	void Release() override;

private:
	void SetTileTransform(const sf::Transform& trans);


protected:
	std::vector<std::vector<DTile*>> m_TileDrawable;
	bool SortTile(const DrawableObject* dobj1, const DrawableObject* dobj2) const;
};
