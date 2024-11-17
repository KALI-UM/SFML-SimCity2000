#pragma once

class SceneBase;
class SceneManager
	:public Singleton <SceneManager>
{
public:
	bool Initialize();
	void Reset();
	void Update(float dt);
	void LateUpdate(float dt);
	void FixedUpdate(float dt);
	void ImGuiUpdate();
	void Release();
	void PreRender();
	void PostRender();

	std::string GetCurrentSceneName() const;
	void SetCurrentScene(const std::string& name);
	SceneBase* GetCurrentScene();
	bool ChangeScene(const std::string& name);
	void PushScene(SceneBase* scene);
private:
	void SetCurrentScene(SceneBase* scene);

	std::string m_CurrSceneName;
	SceneBase* m_CurrScene;
	SceneBase* m_WantsTo;
	std::unordered_map<std::string, SceneBase*> m_Scenes;

	SceneBase* m_EmptyScene;
};

