#pragma once
#include "Building.h"
class PowerPlantBuilding :
	public Building
{
public:
	PowerPlantBuilding();
	~PowerPlantBuilding();

	bool Initialize() override;

	void GameSysSpeedUpdate(float dt);

	float m_SupplyHour = 100;
	float m_ElecAmount;
};

