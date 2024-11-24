#pragma once
#include "Building.h"

class PowerPlantBuilding :
	public Building
{
public:
	PowerPlantBuilding();
	~PowerPlantBuilding();

	bool Initialize() override;

	void SupplyUpdate(float dt);
	void UsageUpdate(float dt);

	float m_SupplyMonth = 100;
	float m_ElecAmount = 0;
};

