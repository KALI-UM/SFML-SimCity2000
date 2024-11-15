#include "pch.h"
#include "SoundObject.h"

SoundData::SoundData(const std::string& filepath, float volume)
	:m_IsValid(false), defaultVolume(volume* GAME_MGR->GetGlobalVolume()), m_SoundDuration(0)
{
	sf::SoundBuffer* soundbuff = ResourceManager<sf::SoundBuffer>::GetInstance()->GetByFilepath(filepath);
	if (soundbuff)
	{
		SetIsValid(true);
		m_Sound.setBuffer(*soundbuff);
		m_Sound.setVolume(defaultVolume);
		m_SoundDuration = m_Sound.getBuffer()->getDuration().asSeconds();
	}
}

SoundData::SoundData(const SoundData& other)
	:m_IsValid(false), defaultVolume(other.defaultVolume),m_SoundDuration(other.m_SoundDuration)
{
	if (other.m_Sound.getBuffer())
	{
		SetIsValid(true);
		m_Sound = other.m_Sound;
		m_Sound.setVolume(defaultVolume * GAME_MGR->GetGlobalVolume());
	}
}

SoundData::SoundData(SoundData&& other)
	:m_IsValid(other.m_IsValid), defaultVolume(other.defaultVolume), m_SoundDuration(other.m_SoundDuration)
{
	if (other.m_Sound.getBuffer())
	{
		m_Sound = other.m_Sound;
	}
	else
	{
		other.SetIsValid(false);
	}
}

SoundData::~SoundData()
{
}

void SoundData::Play()
{
	m_Sound.play();
}

void SoundData::Pause()
{
	m_Sound.pause();
}

void SoundData::Stop()
{
	if (GetIsPlaying())
		m_Sound.stop();
}

void SoundData::SetLoop(bool loop)
{
	m_Sound.setLoop(loop);
}

bool SoundData::GetIsPlaying() const
{
	return ((int)m_Sound.getStatus() == (int)sf::Sound::Status::Playing);
}

