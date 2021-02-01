#pragma once
#include <SDL_mixer.h>
#include <iostream>


class Sound
{


public:

	Sound();
	~Sound();

	void Play();
	void Stop();

	void SetChunkVolume(int in_vol);

	bool LoadSound(const std::string& in_fileName);

	bool IsPlaying();


private:

	Mix_Chunk* m_chunk;
	int m_channel;

	int m_volume;             // Œø‰Ê‰¹‰¹—Ê

};