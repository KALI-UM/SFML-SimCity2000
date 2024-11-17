#include "pch.h"
#include "SceneBase.h"
#include "SoundPlayer.h"

SceneBase::SceneBase(const std::string& name, unsigned int layercnt, int viewcnt)
	:m_ViewCnt(viewcnt), m_Name(name)
{
	m_GameObjects.resize(layercnt);
	m_LayerIndex.resize(layercnt);
	int viewIndex = 0;
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		m_LayerIndex[viewIndex] = it;
		viewIndex++;
	}
}

SceneBase::~SceneBase()
{
}

bool SceneBase::INITIALIZE()
{
	bool result = Initialize();
	for (auto& layer : m_GameObjects)
		for (auto& gobj : layer.gameObjects)
		{
			if (!gobj->GetIsChildObj())
				result &= gobj->INITIALIZE();
		}
	return result;
}

void SceneBase::RESET()
{
	for (auto& layer : m_GameObjects)
		for (auto& gobj : layer.gameObjects)
		{
			gobj->RESET();
		}
	Reset();
}

void SceneBase::ENTER()
{
	ImGuiManager::SetShowDemo(true);
	GAME_MGR->ResizeViews(m_ViewCnt);
	Enter();
}

void SceneBase::UPDATE(float dt)
{
	for (auto& layer : m_GameObjects)
		for (auto& gobj : layer.gameObjects)
		{
			if (gobj->GetIsValid() && !gobj->GetIsChildObj())
				gobj->UPDATE(dt);
		}
	Update(dt);
}

void SceneBase::LATEUPDATE(float dt)
{
	for (auto& layer : m_GameObjects)
		for (auto& gobj : layer.gameObjects)
		{
			if (gobj->GetIsValid() && !gobj->GetIsChildObj())
				gobj->LATEUPDATE(dt);
		}
	LateUpdate(dt);
	RemoveGameObject();
}

void SceneBase::FIXEDUPDATE(float dt)
{
	for (auto& layer : m_GameObjects)
		for (auto& gobj : layer.gameObjects)
		{
			if (gobj->GetIsValid() && !gobj->GetIsChildObj())
				gobj->FIXEDUPDATE(dt);
		}
	FixedUpdate(dt);
}

void SceneBase::IMGUIUPDATE()
{
	for (auto& layer : m_GameObjects)
		for (auto& gobj : layer.gameObjects)
		{
			if (gobj->GetIsValid())
				gobj->IMGUIUPDATE();
		}
	ImGuiUpdate();
}

void SceneBase::PRERENDER()
{
	for (auto& layer : m_GameObjects)
		for (auto& gobj : layer.gameObjects)
		{
			if (gobj->GetIsVisible() && !gobj->GetIsChildObj())
				gobj->PRERENDER();
		}
	PreRender();
	GAME_MGR->UpdateViewRect();
	PushToDrawQue();
}

void SceneBase::POSTRENDER()
{
	for (auto& layer : m_GameObjects)
		for (auto& gobj : layer.gameObjects)
		{
			if (gobj->GetIsVisible() && !gobj->GetIsChildObj())
				gobj->POSTRENDER();
		}
	PostRender();
}

void SceneBase::EXIT()
{
	FRAMEWORK->SetTimeScale(1);
	SOUND_MGR->StopAllSfx();
	SOUND_MGR->StopBgm();
	Exit();
}

void SceneBase::RELEASE()
{
	Release();
	for (auto& layer : m_GameObjects)
		for (auto& gobj : layer.gameObjects)
		{
			delete gobj;
		}
}

bool SceneBase::Initialize()
{
	return true;
}

void SceneBase::Reset()
{
}

void SceneBase::Enter()
{
}

void SceneBase::Update(float dt)
{
}

void SceneBase::LateUpdate(float dt)
{
}

void SceneBase::FixedUpdate(float dt)
{
}

void SceneBase::ImGuiUpdate()
{
}

void SceneBase::PreRender()
{
}

void SceneBase::PostRender()
{
}

void SceneBase::Exit()
{
}

void SceneBase::Release()
{
}

std::string SceneBase::GetName() const
{
	return m_Name;
}

void SceneBase::SetLayerViewIndex(int layerIndex, int viewIndex)
{
	m_LayerIndex[layerIndex]->viewIndex = viewIndex;
}

void SceneBase::RemoveGameObject(GameObjectInfo gobj)
{
	m_WantsToRemove.push(gobj);
}

void SceneBase::RemoveGameObject(int layerIndex, GameObject* gobj)
{
	RemoveGameObject({ layerIndex, gobj });
}

std::vector<GameObject*>& SceneBase::GetGameObjectsLayerIter(int index)
{
	return m_LayerIndex[index]->gameObjects;
}

void SceneBase::PushToDrawQue()
{
	for (int layerIndex = 0; layerIndex < m_GameObjects.size(); layerIndex++)
	{
		for (auto& gobj : GetGameObjectsLayerIter(layerIndex))
		{
			if (gobj->GetIsVisible())
			{
				int viewIndex = m_LayerIndex[layerIndex]->viewIndex;
				//if (gobj->GetIsDrawSelf())
				//{
				//	//게임오브젝트 내의 DrawableObject를 직접 드로우한다
				//	//GAME_MGR->PushDrawableObject(viewIndex, gobj);
				//	//를 하고싶음
				//}
				//else
				//{
					//게임오브젝트 내의 DrawableObject를 드로우큐에 넣는다
				for (int i = 0; i < gobj->GetDrawbleCount(); i++)
				{
					if (gobj->GetIsVisible(i))
					{
						DrawableObject* dobj = gobj->GetDrawableObj(i);
						if (GAME_MGR->GetViewRect(viewIndex).intersects(dobj->GetGlobalBounds()))
						{
							GAME_MGR->PushDrawableObject(viewIndex, dobj);
#ifdef _DEBUG
							if (dobj->GetDebugDraw())
								GAME_MGR->PushDebugDrawObject(viewIndex, dobj->GetDebugDraw());
#endif // _DEBUG
						}
					}
				}
				//}
			}
		}
	}
}

void SceneBase::RemoveGameObject()
{
	while (!m_WantsToRemove.empty())
	{
		int layerIndex = m_WantsToRemove.front().first;
		GameObject* target = m_WantsToRemove.front().second;

		std::remove(GetGameObjectsLayerIter(layerIndex).begin(), GetGameObjectsLayerIter(layerIndex).end(), target);

		m_WantsToRemove.pop();
	}
}
