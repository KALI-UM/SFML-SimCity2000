#pragma once

class SceneBase;
class SoundPlayer;
class SceneManager :
	public Singleton <SceneManager>
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
	SoundPlayer* m_CurrSoundPlayer;

	SceneBase* m_EmptyScene;
};

