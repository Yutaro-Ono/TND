#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>


class GameConfig
{
public:

	bool LoadConfig();

	std::string GetKeyValue(const std::string& filePath, const char* sectionName, const char* keyName) const;

	bool GetFullScreen() const { return m_fullScreen; }
	int GetScreenWidth() const { return m_screenWidth; }
	int GetScreenHeight() const { return m_screenHeight; }


private:



	// ƒXƒNƒŠ[ƒ“î•ñ
    bool m_fullScreen;
	int m_screenWidth, m_screenHeight;

	struct ScreenConfig* m_screenInfo;

};
