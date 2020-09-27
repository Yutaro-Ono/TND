#pragma once
#include <SDL/SDL_mixer.h>
#include <iostream>


class Sound
{


public:

	Sound();
	~Sound();

	void Play();
	void Stop();

	bool LoadSound(const std::string& in_fileName);

	bool IsPlaying();


private:

	Mix_Chunk* m_chunk;
	int m_channel;


};