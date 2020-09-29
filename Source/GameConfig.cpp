#include "GameConfig.h"
#include <windows.h>
#include <sstream>
#include <array>

// �R���t�B�O�̃��[�h
bool GameConfig::LoadConfig()
{
	// �t�@�C���̓ǂݍ���
	std::ifstream file("TheNightDriver.ini");
	// �G���[�ŏI��
	if (!file.is_open())
	{
		std::cerr << "ERROR: Category: " << "Settings: ini�t�@�C�����J���܂���" << std::endl;
		return false;
	}

	// �t�@�C���p�X�w��
	std::string filePath = ".\\TheNightDriver.ini";

    //---------------------------------------------------------------+
	// �ݒ���̃��[�h
	//---------------------------------------------------------------+
	// ��ʐݒ�
	m_fullScreen = (bool)atoi(GetKeyValue(filePath, "Screen", "fullScreen").c_str());
	m_screenWidth = atoi(GetKeyValue(filePath, "Screen", "ScreenWidth").c_str());
	m_screenHeight = atoi(GetKeyValue(filePath, "Screen", "ScreenHeight").c_str());

	file.close();

	return true;
}

// ini����w�肵���Z�N�V����[]���́A�w�肵���L�[�̒l���擾
// ���� : 1 = ini�t�@�C���p�X, 2 = �Z�N�V����, 3 = �L�[��
std::string GameConfig::GetKeyValue(const std::string& filePath, const char* sectionName, const char* keyName) const
{

	if (filePath.empty())
	{
		return "";
	}

	std::array<char, MAX_PATH> buff = {};
	GetPrivateProfileStringA(sectionName, keyName, "", &buff.front(), static_cast<DWORD>(buff.size()), filePath.c_str());

	return &buff.front();
}
