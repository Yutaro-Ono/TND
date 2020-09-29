#include "GameConfig.h"
#include <windows.h>
#include <sstream>
#include <array>

// コンフィグのロード
bool GameConfig::LoadConfig()
{
	// ファイルの読み込み
	std::ifstream file("TheNightDriver.ini");
	// エラーで終了
	if (!file.is_open())
	{
		std::cerr << "ERROR: Category: " << "Settings: iniファイルを開けません" << std::endl;
		return false;
	}

	// ファイルパス指定
	std::string filePath = ".\\TheNightDriver.ini";

    //---------------------------------------------------------------+
	// 設定情報のロード
	//---------------------------------------------------------------+
	// 画面設定
	m_fullScreen = (bool)atoi(GetKeyValue(filePath, "Screen", "fullScreen").c_str());
	m_screenWidth = atoi(GetKeyValue(filePath, "Screen", "ScreenWidth").c_str());
	m_screenHeight = atoi(GetKeyValue(filePath, "Screen", "ScreenHeight").c_str());

	file.close();

	return true;
}

// iniから指定したセクション[]内の、指定したキーの値を取得
// 引数 : 1 = iniファイルパス, 2 = セクション, 3 = キー名
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
