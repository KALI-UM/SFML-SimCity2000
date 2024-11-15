#pragma once

#include "imgui.h"
class ImGuiManager
{
public:
	static bool Init(sf::RenderWindow* target);
	static void Release();
	static void	PollEvent(const sf::Event& ev);
	static void SetDocking(bool enabled);

	static void SetShowDemo(bool enabled);

	static void	Begin(const sf::Time& dt);
	static bool IsFocusedWindow();
	static void ActivateInputAll(bool enabled);
	static void ActivateInputKeyboard(bool enabled);
	static void ActivateInputMouse(bool enabled);
	static void	End();
	static void	ShowDemo();

	static void ShowSceneImGui();

private:
	inline static sf::RenderWindow* s_CurrWindow = nullptr;
	inline static bool s_EnableSceneImgui = true;
};
