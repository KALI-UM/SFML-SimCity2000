#pragma once
#include "Building.h"
class NormalBuilding :
    public Building
{
public:
	NormalBuilding();
	~NormalBuilding();

	bool Initialize() override;

	void SupplyUpdate(float dt);
	void UsageUpdate(float dt);
};

