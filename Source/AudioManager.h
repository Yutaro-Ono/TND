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
	void   RemoveSound(const std::string& in_fileName);                    // �T�E���h���폜
	void   PlaySoundTND(const std::string& in_fileName);                   // �T�E���h�Đ�
	void   StopSound(const std::string& in_fileName);                      // �T�E���h��~
	bool   IsPlayingSound(const std::string& in_fileName);                 // �T�E���h���Đ�����
	void   SetSoundVolume(const std::string& in_filename, int in_vol);     // �w��̃T�E���h�`�����N�̉��ʂ��Z�b�g


	Music* GetMusic(const std::string& in_fileName);
	void   SetMusicVolume(const std::string& in_filename, int in_vol);     // �w��̃~���[�W�b�N�̉��ʂ��Z�b�g
	void   RemoveMusic(const std::string& in_fileName);
	void   PlayFadeInMusic(const std::string& in_fileName, int in_fadeTime);
	void   PlayMusic(const std::string& in_fileName);
	void   FadeOutMusic(int in_fadeTime);          // �~���b
	void   StopMusic();
	bool   IsPlayingMusic();

	void ShutDown();

private:
	std::unordered_map<std::string, class Sound*> m_sounds;           // �T�E���h�f�[�^��Map
	std::unordered_map<std::string, class Music*> m_musics;           // �~���[�W�b�N(�t�@�C�����A�f�[�^)


};