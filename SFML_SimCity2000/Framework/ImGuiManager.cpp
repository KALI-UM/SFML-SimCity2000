#include "pch.h"
#include "ImguiManager.h"


bool ImGuiManager::Init(sf::RenderWindow* target)
{
	s_CurrWindow = target;
	bool result = ImGui::SFML::Init(*s_CurrWindow);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	return (result);
}

void ImGuiManager::Release()
{
	ImGui::SFML::Shutdown();
	//if (s_benabledimplot)
	//	implot::destroycontext();
}

void ImGuiManager::PollEvent(const sf::Event& ev)
{
	ImGui::SFML::ProcessEvent(*s_CurrWindow, ev);
}

void ImGuiManager::SetDocking(bool enabled)
{
	ImGuiIO& io = ImGui::GetIO();
	if (enabled)
	{
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	}
	else 
	{
		io.ConfigFlags &= ~ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;
	}
}

void ImGuiManager::SetShowDemo(bool enabled)
{
	s_EnableSceneImgui = enabled;
}

void ImGuiManager::Begin(const sf::Time& dt)
{
	ImGui::SFML::Update(*s_CurrWindow, dt);
	ImGui::SFML::SetCurrentWindow(*s_CurrWindow);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
 	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);
	}
	if (s_EnableSceneImgui)
	{
		ImGuiManager::ShowDemo();
	}

}

bool ImGuiManager::IsFocusedWindow()
{
	return ImGui::IsWindowFocused();
}

void ImGuiManager::ActivateInputAll(bool enabled)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (enabled)
	{
		io.WantCaptureKeyboard = true;
		io.WantCaptureMouse = true;
	}
	else
	{
		io.WantCaptureKeyboard = false; // 키보드 입력을 ImGui가 받지 않도록
		io.WantCaptureMouse = false;    // 마우스 입력을 ImGui가 받지 않도록
	}
}

void ImGuiManager::ActivateInputKeyboard(bool enabled)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (enabled)
		io.WantCaptureKeyboard = true;
	else
		io.WantCaptureKeyboard = false; // 키보드 입력을 ImGui가 받지 않도록
}

void ImGuiManager::ActivateInputMouse(bool enabled)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (enabled)
		io.WantCaptureMouse = true;
	else
		io.WantCaptureMouse = false;    // 마우스 입력을 ImGui가 받지 않도록
}

void ImGuiManager::End()
{
	ImGui::SFML::Render(*s_CurrWindow);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void ImGuiManager::ShowDemo()
{
	bool show_demo_window = true;
	// ImGui::ShowDemoWindow(&show_demo_window);
	static float f = 0.0f;
	static int counter = 0;

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	bool blockInput = true; 

	ImGui::Begin("Hello, world!");
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);         
	if (ImGui::Button("Button"))                          
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();

	ShowSceneImGui();
}

void ImGuiManager::ShowSceneImGui()
{
	SCENE_MGR->GetCurrentScene()->ShowSceneImgui();
}
