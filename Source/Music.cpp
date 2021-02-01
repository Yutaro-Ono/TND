#include "Music.h"

Music::Music()
	:m_music(nullptr)
	,m_volume(6)
{
	Mix_VolumeMusic(m_volume);
}

Music::~Music()
{
	Mix_FreeMusic(m_music);
}

void Music::Play()
{
	if (m_music)
	{
		Mix_PlayMusic(m_music, -1);
	}
}

void Music::FadeInMusic(int in_fadeTime)
{
	if (m_music)
	{
		Mix_FadeInMusic(m_music, -1, in_fadeTime);
	}
}

bool Music::LoadMusic(const std::string & in_fileName)
{
	m_music = Mix_LoadMUS(in_fileName.c_str());

	return m_music != nullptr;
}

/// <summary>
/// âπäyÇÃâπó í≤êÆ (1 Å` 128)
/// </summary>
/// <param name="in_vol">îCà”ÇÃâπó (1 Å` 128)</param>
void Music::SetMusicVolume(int in_vol)
{
	m_volume = in_vol;
	Mix_VolumeMusic(m_volume);
}

void Music::Stop()
{
	Mix_HaltMusic();
}

void Music::FadeOutMusic(int in_fadeTime)
{
	Mix_FadeOutMusic(in_fadeTime);
}

bool Music::IsPlaying()
{
	return Mix_PlayingMusic();
}
