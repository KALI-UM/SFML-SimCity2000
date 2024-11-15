#pragma once
#include "SFML/Audio.hpp"

class MusicObject 
{
public:
	MusicObject(const std::string& filepath, float volume);
	MusicObject(const MusicObject& other) = delete;
	MusicObject(MusicObject&& other) = delete;
	~MusicObject();

	void Play(bool fadeIn = false, float duration = 0, float startvolume = 0);
	void Pause(bool fadeOut = false, float duration = 0, float endvolume = 0);
	void Stop(bool fadeOut = false, float duration = 0, float endvolume = 0);
	void SetLoop(bool loop);

	void Update(float dt);
	bool GetIsValid()const { return m_IsValid; }
	void SetIsValid(bool valid) { m_IsValid = valid; }
	bool GetIsPlaying()const;

private:
	void SetFadeInSpeed(float duration, float start);
	void SetFadeOutSpeed(float duration, float end);


	bool			m_IsValid;
	sf::Music		m_Music;

	float			m_DefaultVolume;
	float			m_FadeInSpeed;
	float			m_FadeOutVolume;
	float			m_FadeOutSpeed;
	float			m_MusicDuration;
};

