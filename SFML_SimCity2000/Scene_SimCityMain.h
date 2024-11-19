#pragma once
class Scene_SimCityMain :
    public SceneBase
{
public:
    Scene_SimCityMain();
    ~Scene_SimCityMain();

protected:
	bool Initialize() override;
	void Reset() override;
	void Enter() override;
	void Update(float dt) override;
	void Exit() override;
	//void ShowSceneImgui() override;
	//void LateUpdate(float dt) override;
	//void FixedUpdate(float dt) override;
	//void Release() override;

};

