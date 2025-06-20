#pragma once
#include "GameObject.h"


class DSprite;
class Background_Lobby :
    public GameObject
{
public:
	Background_Lobby();
	~Background_Lobby();

	bool Initialize() override ;
	void Reset()override;
	void Update(float dt)override;

	void FadeOutBackGround(float duration);

private:
	DSprite* m_Background;

	float m_FadeOutDuration;
	float m_FadeOutSpeed=-1;
	float m_CurrvColorValue;
};

