#include "pch.h"
#include "SoundPlayer.h"

SoundPlayer::SoundPlayer()
{
}

SoundPlayer::~SoundPlayer()
{
}

bool SoundPlayer::Initialize()
{
	m_Effects.insert({ "sound/ButtonClick.wav", new SoundData("sound/ButtonClick.wav", 100) });
	m_Effects.insert({ "sound/NextScene.wav", new SoundData("sound/NextScene.wav", 80) });
	m_Effects.insert({ "sound/mon_die.wav", new SoundData("sound/mon_die.wav", 50 ) });
	m_Effects.insert({ "sound/baby_dragon_die.wav", new SoundData("sound/baby_dragon_die.wav", 70) });

	m_BGMs.insert({ "sound/dragon_flight.mp3", new MusicObject("sound/dragon_flight.mp3", 20) });
	m_BGMs.insert({ "sound/my_friend_dragon.mp3", new MusicObject("sound/my_friend_dragon.mp3", 20) });

	return true;
}

void SoundPlayer::Reset()
{
}

void SoundPlayer::Update(float dt)
{
	dt = FRAMEWORK->GetRealDeltaTime();
	for (auto it = m_BGMs.begin(); it != m_BGMs.end(); it++)
	{
		it->second->Update(dt);
	}
}

void SoundPlayer::Release()
{
	//해제해줘야함
}

void SoundPlayer::StopAllSound()
{
	for (auto it = m_BGMs.begin(); it != m_BGMs.end(); it++)
	{
		it->second->Stop();
	}
}

void SoundPlayer::PlayEffect(const std::string& filepath)
{
	auto sound = m_Effects.find(filepath);
	if (sound == m_Effects.end())
	{
		std::cout << "존재하지 않는 이펙트 사운드 입니다.\n";
		return;
	}
	sound->second->Play();
}

void SoundPlayer::PlayBGM(const std::string& filepath, bool fadeIn, float duration, float startVolume)
{
	auto music = m_BGMs.find(filepath);
	if (music == m_BGMs.end())
	{
		std::cout << "존재하지 않는 비지엠 사운드 입니다.\n";
		return;
	}
	music->second->Play(fadeIn, duration, startVolume);
}

void SoundPlayer::StopBGM(const std::string& filepath)
{
	auto music = m_BGMs.find(filepath);
	if (music == m_BGMs.end())
	{
		std::cout << "존재하지 않는 비지엠 사운드 입니다.\n";
		return;
	}
	music->second->Stop();
}

void SoundPlayer::FadeOutBGM(const std::string& filepath, float duration, float endVolume)
{
	auto music = m_BGMs.find(filepath);
	if (music == m_BGMs.end())
	{
		std::cout << "존재하지 않는 비지엠 사운드 입니다.\n";
		return;
	}
	music->second->Stop(true, duration, endVolume);
}
