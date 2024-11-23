#include "pch.h"
#include "SceneBase.h"
#include "SoundPlayer.h"

SceneBase::SceneBase(const std::string& name, unsigned int layercnt, int viewcnt)
	:m_ViewCnt(viewcnt+1), m_Name(name), m_UILayerIndex(layercnt), m_UIViewIndex(viewcnt)
{
	m_GameObjects.resize(layercnt+1);
	m_LayerIndex.resize(layercnt+1);
	m_ViewInfo.resize(m_ViewCnt);
	for (int i = 0; i < m_ViewCnt; i++)
	{
		m_ViewInfo[i].viewIndex = i;
		m_ViewInfo[i].needPriority = true;
	}

	int layerIndex = 0;
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		m_LayerIndex[layerIndex] = it;
		layerIndex++;
	}

	//UI레이어는 UI뷰로 설정
	m_LayerIndex[m_UILayerIndex]->viewIndex = m_UIViewIndex;
}

SceneBase::~SceneBase()
{
}

bool SceneBase::INITIALIZE()
{
	bool result = Initialize();
	RegisterGameObject();
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
	ImGuiManager::SetShowDemo(false);
	GAME_MGR->ResizeViews(m_ViewCnt);
	for (int i = 0; i < m_ViewCnt; i++)
	{
		GAME_MGR->SetViewNeedPriority(i, m_ViewInfo[i].needPriority);
	}

	Enter();
}

void SceneBase::UPDATE(float dt)
{
	RegisterGameObject();
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
			//if (gobj->GetIsVisible() && !gobj->GetIsChildObj())
			if (!gobj->GetIsChildObj())
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
			//if (gobj->GetIsVisible() && !gobj->GetIsChildObj())
			if (!gobj->GetIsChildObj())
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

void SceneBase::SetViewNeedPriority(int viewIndex, bool needPriority)
{
	m_ViewInfo[viewIndex].needPriority = needPriority;
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
			int viewIndex = m_LayerIndex[layerIndex]->viewIndex;
			bool needPriority = m_ViewInfo[viewIndex].needPriority;
			if (gobj->GetIsVisible())
			{
				//if (gobj->GetIsDrawSelf())
				//{
				//	//게임오브젝트 내의 DrawableObject를 직접 드로우한다
				//	//GAME_MGR->PushDrawableObject(viewIndex, gobj);
				//	//를 하고싶음
				//}
				//else
				//{

				if (needPriority)
				{
					//게임오브젝트 내의 DrawableObject를 드로우큐에 넣는다
					for (int i = 0; i < gobj->GetDrawbleCount(); i++)
					{
						if (gobj->GetIsVisible(i))
						{
							DrawableObject* dobj = gobj->GetDrawableObj(i);
							if (GAME_MGR->GetViewRect(viewIndex).intersects(dobj->GetGlobalBounds()))
							{
								GAME_MGR->PushDrawableObject_PQ(viewIndex, dobj);
#ifdef _DEBUG
								if (dobj->GetDebugDraw())
									GAME_MGR->PushDebugDrawObject(viewIndex, dobj->GetDebugDraw());
#endif // _DEBUG
							}
						}
					}
				}
				else
				{
					for (int i = 0; i < gobj->GetDrawbleCount(); i++)
					{
						if (gobj->GetIsVisible(i))
						{
							DrawableObject* dobj = gobj->GetDrawableObj(i);
							if (GAME_MGR->GetViewRect(viewIndex).intersects(dobj->GetGlobalBounds()))
							{
								GAME_MGR->PushDrawableObject_Q(viewIndex, dobj);
#ifdef _DEBUG
								if (dobj->GetDebugDraw())
									GAME_MGR->PushDebugDrawObject(viewIndex, dobj->GetDebugDraw());
#endif // _DEBUG
							}
						}
					}
				}
				//}
			}
		}
	}
}

void SceneBase::RegisterGameObject()
{
	while (!m_WantsToAdd.empty())
	{
		int layerIndex = m_WantsToAdd.front().first;
		GameObject* target = m_WantsToAdd.front().second;
		m_WantsToAdd.pop();
		GetGameObjectsLayerIter(layerIndex).push_back(target);
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
