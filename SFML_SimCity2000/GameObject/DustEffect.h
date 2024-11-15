#pragma once
#include "EffectObject.h"
class DustEffect :
    public EffectObject
{
public:
	void InitForObjectPool();

public:
	DustEffect();
	~DustEffect();

	bool Initialize();
	void Reset();

private:
	virtual void EffectPlay(float dt);
	virtual void SetPosition();

	float m_Offset;
};

