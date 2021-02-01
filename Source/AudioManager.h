#pragma once

#include <SDL_mixer.h>
#include <unordered_map>
#include <string>
#include "Sound.h"
#include "Music.h"

class AudioManager
{


public:

	AudioManager();
	~AudioManager();

	Sound* GetSound(const std::string& in_fileName);
	void   RemoveSound(const std::string& in_fileName);                    // サウンドを削除
	void   PlaySoundTND(const std::string& in_fileName);                   // サウンド再生
	void   StopSound(const std::string& in_fileName);                      // サウンド停止
	bool   IsPlayingSound(const std::string& in_fileName);                 // サウンドが再生中か
	void   SetSoundVolume(const std::string& in_filename, int in_vol);     // 指定のサウンドチャンクの音量をセット


	Music* GetMusic(const std::string& in_fileName);
	void   SetMusicVolume(const std::string& in_filename, int in_vol);     // 指定のミュージックの音量をセット
	void   RemoveMusic(const std::string& in_fileName);
	void   PlayFadeInMusic(const std::string& in_fileName, int in_fadeTime);
	void   PlayMusic(const std::string& in_fileName);
	void   FadeOutMusic(int in_fadeTime);          // ミリ秒
	void   StopMusic();
	bool   IsPlayingMusic();

	void ShutDown();

private:
	std::unordered_map<std::string, class Sound*> m_sounds;           // サウンドデータのMap
	std::unordered_map<std::string, class Music*> m_musics;           // ミュージック(ファイル名、データ)


};