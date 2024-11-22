#include "pch.h"
#include "SoundManager.h"


void SoundManager::Initialize(int totalChannels)
{
	//UINT numOutputDevices = waveOutGetNumDevs();
	m_Listener.setGlobalVolume(m_GlobalVolume);
	for (int i = 0; i < totalChannels; ++i)
	{
		auto sound = new sf::Sound();
		sound->setRelativeToListener(true);
		m_WaitingSfx.push_back(sound);
	}

	m_Bgm.setRelativeToListener(true);
}

void SoundManager::Release()
{
	if (!m_IsSoundDeviceValid)return;

	for (auto sound : m_WaitingSfx)
	{
		delete sound;
	}
	m_WaitingSfx.clear();
	for (auto sound : m_PlayingSfx)
	{
		delete sound;
	}
	m_WaitingSfx.clear();
}

void SoundManager::Update(float dt)
{
	if (!m_IsSoundDeviceValid)return;

	auto it = m_PlayingSfx.begin();
	while (it != m_PlayingSfx.end())
	{
		if ((*it)->getStatus() == sf::Sound::Stopped)
		{
			m_WaitingSfx.push_back(*it);
			it = m_PlayingSfx.erase(it);
			if (m_PlayingSfx.empty())break;
		}
		else
		{
			++it;
		}
	}

	if (m_Bgm.getStatus() == sf::Sound::Playing)
	{
		float currv = m_Bgm.getVolume();
		if (m_BgmData.fadeInSpeed > 0)
		{
			currv += m_BgmData.fadeInSpeed * dt;
			if (currv > m_BgmData.defaultVolume)
			{
				m_Bgm.setVolume(m_BgmData.defaultVolume);
				m_BgmData.fadeInSpeed = -1;
			}
		}

		if (m_BgmData.fadeOutSpeed > 0)
		{
			currv += m_BgmData.fadeOutSpeed * dt;
			if (currv < m_BgmData.fadeOutVolume || currv < 0.1f)
			{
				m_Bgm.stop();
				m_Bgm.setVolume(m_BgmData.defaultVolume);
				m_BgmData.fadeOutSpeed = -1;
			}
		}
		m_Bgm.setVolume(currv);
	}
}

void SoundManager::SetGlobalVolume(float volume)
{
	if (!m_IsSoundDeviceValid)return;

	m_GlobalVolume = volume;
	m_Listener.setGlobalVolume(volume);
}

void SoundManager::PlayBgm(std::string id, bool loop, bool fadeIn, float fadeTime, float startvolume , bool startRandomPos)
{
	if (!m_IsSoundDeviceValid)return;


	PlayBgm(*ResourceManager<sf::SoundBuffer>::GetInstance()->GetByFilepath(id),
		loop,
		fadeIn,
		fadeTime,
		startvolume,
		startRandomPos);

}

void SoundManager::PlayBgm(sf::SoundBuffer& buffer, bool loop, bool fadeIn, float fadeTime, float startvolume, bool startRandomPos)
{
	if (!m_IsSoundDeviceValid)return;

	m_Bgm.stop();
	m_BgmData.duration = buffer.getDuration().asSeconds();

	if (startRandomPos)
	{
		m_Bgm.setPlayingOffset(sf::seconds(Utils::RandomRange(0.0f, m_BgmData.duration - 10)));
	}


	if (fadeIn)
	{
		if (!SetFadeInSpeed(m_BgmData, fadeTime, startvolume)) startvolume = m_BgmVolume;
		m_Bgm.setVolume(startvolume);
	}
	else
	{
		m_Bgm.setVolume(m_BgmVolume);
	}

	m_Bgm.setLoop(loop);
	m_Bgm.setBuffer(buffer);
	m_Bgm.play();
}

void SoundManager::StopBgm(bool fadeOut, float fadeTime, float endvolume)
{
	if (!m_IsSoundDeviceValid)return;

	if (fadeOut)
	{
		if (SetFadeOutSpeed(m_BgmData, fadeTime, endvolume)) return;
	}
	m_Bgm.stop();
}

void SoundManager::PlaySfx(std::string id, bool loop)
{
	if (!m_IsSoundDeviceValid)return;

	PlaySfx(*ResourceManager<sf::SoundBuffer>::GetInstance()->GetByFilepath(id), loop);
}

void SoundManager::PlaySfx(sf::SoundBuffer& buffer, bool loop)
{
	if (!m_IsSoundDeviceValid)return;

	//nullcheck ÇÊ¿äÇÔ
	sf::Sound* sound = nullptr;
	if (m_WaitingSfx.empty())
	{
		sound = m_PlayingSfx.front();
		m_PlayingSfx.pop_front();
		sound->stop();
	}
	else
	{
		sound = m_WaitingSfx.front();
		m_WaitingSfx.pop_front();
	}

	sound->setVolume(m_SfxVolume);
	sound->setBuffer(buffer);
	sound->setLoop(loop);
	sound->play();
	m_PlayingSfx.push_back(sound);
}

void SoundManager::StopAllSfx()
{
	if (!m_IsSoundDeviceValid)return;

	for (auto sound : m_PlayingSfx)
	{
		sound->stop();
		m_WaitingSfx.push_back(sound);
	}
	m_PlayingSfx.clear();
}

bool SoundManager::SetFadeInSpeed(SoundData& data, float fadeTime, float start)
{
	data.defaultVolume = m_BgmVolume;
	if (start > data.defaultVolume)return false;

	if (data.duration < fadeTime)
		fadeTime = data.duration;
	data.fadeInSpeed = (data.defaultVolume - start) / fadeTime;
	return true;
}

bool SoundManager::SetFadeOutSpeed(SoundData& data, float fadeTime, float end)
{
	data.defaultVolume = m_BgmVolume;
	if (end > data.defaultVolume)return false;

	if (data.duration < fadeTime)
		fadeTime = data.duration;
	data.fadeOutVolume = end;
	data.fadeOutSpeed = (data.defaultVolume - end) / fadeTime;
	return true;
}