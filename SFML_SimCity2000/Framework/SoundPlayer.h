#pragma once

namespace sf
{
	class Sound;
}

struct SoundData;
class MusicObject;
class SoundPlayer :
	public GameObject
{
public:
	SoundPlayer();
	~SoundPlayer();

	bool Initialize() override;
	void Reset()override;
	void Update(float dt)override;
	void Release() override;

	void StopAllSound();
	void PlayEffect(const std::string& filepath);
	void PlayBGM(const std::string& filepath, bool fadeIn = false, float durtaion = 0, float startVolume = 0);
	void StopBGM(const std::string& filepath);

	void FadeOutBGM(const std::string& filepath, float duration, float endVolume = 0);
	//void FadeInBGM(const std::string& filepath, float duration, float startVolume = 0);
private:
	std::unordered_map<std::string, SoundData*> m_Effects;
	std::unordered_map<std::string, MusicObject*> m_BGMs;
};
