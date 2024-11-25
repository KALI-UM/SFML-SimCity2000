#pragma once
#include "Building.h"
class PublicServiceBuilding :
    public Building
{
public:
	PublicServiceBuilding();
	~PublicServiceBuilding();

	bool Initialize() override;

	void SupplyUpdate(float dt);
	void UsageUpdate(float dt);
};

