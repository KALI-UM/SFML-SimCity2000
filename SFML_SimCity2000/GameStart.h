#pragma once
#include "GameObject.h"

class DText;
class GameStart : public GameObject
{
public:
	GameStart();
	~GameStart();

	bool Initialize();
	void Reset();
	void Update(float dt);

	DText* m_GameStart;

protected:


};

