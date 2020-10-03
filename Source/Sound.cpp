#include "Sound.h"
#include <map>



Sound::Sound()
	:m_chunk(nullptr)
	,m_channel(-1)
{
}

Sound::~Sound()
{
	Mix_FreeChunk(m_chunk);
}

void Sound::Play()
{
	m_channel = Mix_PlayChannel(-1, m_chunk, 0);
}

void Sound::Stop()
{
	if (m_channel == -1)
	{
		return;
	}

	if (Mix_Playing(m_channel))
	{
		Mix_HaltChannel(m_channel);
	}

	m_channel = -1;
}

bool Sound::LoadSound(const std::string & in_fileName)
{
	m_chunk = Mix_LoadWAV(in_fileName.c_str());
	return m_chunk != nullptr;
}

bool Sound::IsPlaying()
{
	return m_channel >= 0 && Mix_Playing(m_channel);
}
