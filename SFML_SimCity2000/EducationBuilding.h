#pragma once
#include "Building.h"

class EducationBuilding:
	public Building
{
public:
	EducationBuilding();
	~EducationBuilding();

	bool Initialize() override;

	void SupplyUpdate(float dt);
	void UsageUpdate(float dt);

};

