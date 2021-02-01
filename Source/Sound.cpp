#include "Sound.h"
#include <map>



Sound::Sound()
	:m_chunk(nullptr)
	,m_channel(-1)
	,m_volume(10)
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

/// <summary>
/// サウンドチャンクの音量をセット
/// </summary>
/// <param name="in_vol"> 音量 (1 ～ 128) </param>
void Sound::SetChunkVolume(int in_vol)
{
	m_volume = in_vol;
	Mix_VolumeChunk(m_chunk, m_volume);
}

bool Sound::LoadSound(const std::string & in_fileName)
{
	// チャンクにロード
	m_chunk = Mix_LoadWAV(in_fileName.c_str());
	// 音量をセット
	Mix_VolumeChunk(m_chunk, m_volume);

	return m_chunk != nullptr;
}

bool Sound::IsPlaying()
{
	return m_channel >= 0 && Mix_Playing(m_channel);
}
