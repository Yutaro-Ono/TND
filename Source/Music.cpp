#include "Music.h"

Music::Music()
	:m_music(nullptr)
{
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
