#pragma once
#include "Building.h"

class RecreationBuilding
	:public Building
{
public:
	RecreationBuilding();
	~RecreationBuilding();

	bool Initialize() override;

	void SupplyUpdate(float dt);
	void UsageUpdate(float dt);

};