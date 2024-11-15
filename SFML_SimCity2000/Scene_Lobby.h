#pragma once
#include "SceneBase.h"
class Background_Lobby;
class SoundPlayer;
class DustEffect;
class GameStart;
class Button;
class MouseCursor;
class Scene_Lobby :
    public SceneBase
{
public:
	Scene_Lobby();
	~Scene_Lobby();

protected:
	bool Initialize() override;
	void Reset() override;
	void Enter() override;
	void Update(float dt) override;
	void Release() override;


private:
	Background_Lobby* m_LobbyImage;
	std::vector< DustEffect*> m_Effects;
	GameStart*		m_GameStart;
	Button*			m_Button;
	MouseCursor*	m_Cursor;
	float			m_FadeSpeed;
};

