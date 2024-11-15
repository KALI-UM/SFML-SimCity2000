#pragma once
#include "SFML/Audio.hpp"

class SoundObject
{
public:
	SoundObject(const std::string& filepath, float volume);
	SoundObject(const SoundObject& other);
	SoundObject(SoundObject&& other);
	~SoundObject();

	void Play();
	void Pause();
	void Stop();
	void SetLoop(bool loop);

	bool GetIsValid()const { return m_IsValid; }
	void SetIsValid(bool valid) { m_IsValid = valid; }
	bool GetIsPlaying()const;

private:
	bool			m_IsValid;
	sf::Sound		m_Sound;

	float			defaultVolume;
	float			m_SoundDuration;
};

