#pragma once
#include "SceneBase.h"

class Character;
class Scene_Loading :
    public SceneBase
{
	Scene_Loading();
	~Scene_Loading();

protected:
	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;
	void Release() override;
private:
	Character* m_Player;
	float m_StartTime;
	float m_PauseTime;
};

