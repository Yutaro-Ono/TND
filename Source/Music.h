#pragma once
#include <SDL_mixer.h>
#include <iostream>

class Music
{


public:

	Music();
	~Music();

	void Play();
	void FadeInMusic(int in_fadeTime);
	bool LoadMusic(const std::string& in_fileName);

	static void Stop();
	static void FadeOutMusic(int in_fadeTime);
	static bool IsPlaying();

private:

	Mix_Music* m_music;


};