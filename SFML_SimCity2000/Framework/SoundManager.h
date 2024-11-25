#pragma once

//struct SoundObject
//{
//	float		m_DefaultVolume;
//	float		m_SoundDuration;
//};

struct SoundData
{
	float		defaultVolume;
	float		duration;

	float		fadeInSpeed;
	float		fadeOutVolume;
	float		fadeOutSpeed;
};

class SoundManager :
	public Singleton<SoundManager>
{
	friend class Singleton<SoundManager>;

protected:
	bool m_IsSoundDeviceValid = true;					//사운드 장치가 유효한지 확인할 수있는 방법이 없다...ㅜ
protected:
	SoundManager() = default;
	virtual ~SoundManager() = default;

	SoundManager(const SoundManager& other) = delete;
	SoundManager& operator=(const SoundManager& other) = delete;

public:
	void Initialize(int totalChannels = 64);
	void Release();
	void Update(float dt);

	void SetGlobalVolume(float volume);
	float GetGlobalVolume() const { return m_GlobalVolume; }


	void PlayBgm(std::string id, bool loop = true, bool fadeIn = false, float fadeTime = 0, float startvolume = 0, bool startRandomPos = false);
	void PlayBgm(sf::SoundBuffer& buffer, bool loop = true, bool fadeIn = false, float fadeTime = 0, float startvolume = 0, bool startRandomPos = false);
	void StopBgm(bool fadeOut = false, float fadeTime = 0, float endvolume = 0);

	void PlaySfx(std::string id, bool loop = false);
	void PlaySfx(sf::SoundBuffer& buffer, bool loop = false);

	void StopAllSfx();

private:
	bool SetFadeInSpeed(SoundData& data, float fadeTime, float start);
	bool SetFadeOutSpeed(SoundData& data, float fadeTime, float end);

protected:
	sf::Listener m_Listener;

	sf::Sound m_Bgm;
	SoundData m_BgmData;

	std::list<sf::Sound*> m_PlayingSfx;
	std::list<sf::Sound*> m_WaitingSfx;

	float m_GlobalVolume = 100.f;
	float m_SfxVolume = 100.f;
	float m_BgmVolume = 50.f;

};

