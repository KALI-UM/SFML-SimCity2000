#include "pch.h"
#include "Framework.h"
#include <windows.h>
#include <uxtheme.h>
#pragma comment(lib, "uxtheme.lib")
#include "resource.h"


void Framework::Initialize(int width, int height, const std::string& name)
{
	m_MainWindow.create(sf::VideoMode(width, height), name, sf::Style::None);

	//sf::Cursor cursor;
	//// 만약 사용자 정의 커서가 있다면 해제
	//cursor.loadFromSystem(sf::Cursor::Arrow);
	//m_MainWindow.setMouseCursor(cursor); // 커서를 시스템 기본으로 설정 후, 숨기기 시도
	//m_MainWindow.setMouseCursorVisible(false);

	HWND hwnd = m_MainWindow.getSystemHandle();

	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	style |= WS_OVERLAPPEDWINDOW;
	SetWindowLong(hwnd, GWL_STYLE, style);
	//HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
	//SetMenu(hwnd, hMenu);
	//DrawMenuBar(hwnd);
	SetWindowTheme(hwnd, L" ", L" ");

	

	Utils::Initialize();

	SOUND_MGR->Initialize();
	INPUT_MGR->Initialize();
	GAME_MGR->Initialize(&m_MainWindow);
	//ImGuiManager::Init(&m_MainWindow);

	SOUND_MGR->SetGlobalVolume(80);
}

void Framework::Do()
{
	m_MainWindow.setFramerateLimit(60);
	sf::Time dt = clock.restart();

	while (m_MainWindow.isOpen())
	{
		m_RealDeltaTime = m_DeltaTime = dt.asSeconds();
		m_DeltaTime *= m_TimeScale;
		m_RealTime += m_RealDeltaTime;
		m_Time += m_DeltaTime;

		// 이벤트 루프
		INPUT_MGR->Clear();
		sf::Event event;

		while (m_MainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_MainWindow.close();
			//ImGuiManager::PollEvent(event);
			INPUT_MGR->UpdateEvent(event);
			GAME_MGR->UpdateEvent(event);

			//if (event.type == sf::Event::GainedFocus) {
			//	// 포커스 얻음
			//	m_MainWindow.setMouseCursorVisible(false);
			//}
			//else if (event.type == sf::Event::LostFocus) {
			//	// 포커스 잃음
			//	m_MainWindow.setMouseCursorVisible(true); // 필요시 켤 수 있음
			//}
		}

		// 업데이트
		if (m_RealTime - m_FixedTimePrev > m_FixedTimeStamp)
		{
			m_FixedTimePrev = m_RealTime;
			GAME_MGR->FixedUpdate(m_RealDeltaTime);
		}

		INPUT_MGR->Update(m_RealDeltaTime);
		SOUND_MGR->Update(m_RealDeltaTime);
		GAME_MGR->Update(m_DeltaTime);

		GAME_MGR->LateUpdate(m_DeltaTime);

		// 드로우
		m_MainWindow.clear();

		//std::cout << 1.0f / m_RealDeltaTime << std::endl;

		GAME_MGR->Render();

		{ // ImGui Layer
	/*		ImGuiManager::Begin(dt);
			GAME_MGR->ImGuiUpdate();
			ImGuiManager::End();*/
		}

		m_MainWindow.display();

		dt = clock.restart();
	}
}

void Framework::Release()
{
	GAME_MGR->Release();
	SOUND_MGR->Release();
	ImGuiManager::Release();
}

