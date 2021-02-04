#include "AudioManager.h"
#include "Sound.h"


AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
	if (m_sounds.size() > 0)
	{
		ShutDown();
	}
}

Sound * AudioManager::GetSound(const std::string & in_fileName)
{
	Sound* sound = nullptr;
	auto iter = m_sounds.find(in_fileName);

	if (iter != m_sounds.end())
	{
		sound = iter->second;
	}
	else
	{
		sound = new Sound();
		if (sound->LoadSound(in_fileName))
		{
			m_sounds.emplace(in_fileName, sound);
		}
		else
		{
			printf("Music File Load Failed : %s\n", in_fileName.c_str());
			delete sound;
			sound = nullptr;
		}
	}

	return sound;
}

void AudioManager::RemoveSound(const std::string & in_fileName)
{
	// �T�E���h���폜
	auto iter = m_sounds.find(in_fileName);

	if (iter != m_sounds.end())
	{
		printf("Sound File Release : %s\n", iter->first.c_str());
		delete iter->second;
		m_sounds.erase(iter);
	}
}

void AudioManager::PlaySoundTND(const std::string& in_fileName)
{

	m_sounds[in_fileName]->Play();

}


void AudioManager::StopSound(const std::string& in_fileName)
{
	m_sounds[in_fileName]->Stop();
}

bool AudioManager::IsPlayingSound(const std::string & in_fileName)
{
	return m_sounds[in_fileName]->IsPlaying();
}

/// <summary>
/// �w��̃T�E���h�`�����N�̉��ʂ��Z�b�g
/// </summary>
/// <param name="in_filename"> ���ʂ�ύX�������o�^�T�E���h�� </param>
/// <param name="in_vol"> ���� </param>
void AudioManager::SetSoundVolume(const std::string& in_filename, int in_vol)
{
	m_sounds[in_filename]->SetChunkVolume(in_vol);
}

Music * AudioManager::GetMusic(const std::string & in_fileName)
{
	Music* music = nullptr;
	auto iter = m_musics.find(in_fileName);

	if (iter != m_musics.end())
	{
		music = iter->second;
	}
	else
	{
		music = new Music();

		if (music->LoadMusic(in_fileName))
		{
			m_musics.emplace(in_fileName, music);
		}
		else
		{
			printf("Music File Load Failed : %s\n", in_fileName.c_str());
			delete music;
			music = nullptr;
		}
	}

	return music;
}

/// <summary>
/// �w��̃~���[�W�b�N�̉��ʂ��Z�b�g
/// </summary>
/// <param name="in_filename"> ���ʕύX�������~���[�W�b�N�� </param>
/// <param name="in_vol"> ���� </param>
void AudioManager::SetMusicVolume(const std::string& in_filename, int in_vol)
{
	m_musics[in_filename]->SetMusicVolume(in_vol);
}

void AudioManager::RemoveMusic(const std::string & in_fileName)
{
	auto iter = m_musics.find(in_fileName);

	if (iter != m_musics.end())
	{
		printf("Music File Release : %s\n", iter->first.c_str());
		delete iter->second;

		m_musics.erase(iter);
	}
}

void AudioManager::PlayFadeInMusic(const std::string & in_fileName, int in_fadeTime)
{
	m_musics[in_fileName]->FadeInMusic(in_fadeTime);
}

void AudioManager::PlayMusic(const std::string & in_fileName)
{
	m_musics[in_fileName]->Play();
}

void AudioManager::FadeOutMusic(int in_fadeTime)
{
	Music::FadeOutMusic(in_fadeTime);
}

void AudioManager::StopMusic()
{
	Music::Stop();
}

bool AudioManager::IsPlayingMusic()
{
	return Music::IsPlaying();
}

void AudioManager::ShutDown()
{
	// �T�E���h�j��
	for (auto iter : m_sounds)
	{
		printf("Sound File Release : %s\n", iter.first.c_str());
		delete iter.second;
	}

	// �~���[�W�b�N�j��
	for (auto iter : m_musics)
	{
		printf("Music File Release : %s\n", iter.first.c_str());
		delete iter.second;
	}

	m_sounds.clear();
	m_musics.clear();
}
