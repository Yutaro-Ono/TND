//-----------------------------------------------------------------------+
//
// Canvas�N���X (UI�N���X�����[���h�N���X���Q�Ƃł���悤�ɂ���)
//
//-----------------------------------------------------------------------+
#pragma once
#include <vector>

class Canvas
{
public:


	Canvas(class GameWorld* in_world);     // �R���X�g���N�^
	~Canvas();    // �f�X�g���N�^

	void Update(float in_deltaTime);

	void AddUI(class UIScreen* in_ui) { m_interfaces.push_back(in_ui); }

	class GameWorld* GetWorld() { return m_world; }

	class PlayerControlUI* GetPlayerControlUI() { return m_pControlUI; }

	class ScoreUI* GetScoreUI() { return m_scoreUI; }

private:


	class GameWorld* m_world;      // �Q�[�����[���h

	class PlayerControlUI* m_pControlUI;

	class ScoreUI* m_scoreUI;                       // �X�R�AUI

	std::vector<class UIScreen*> m_interfaces;      // �C���^�[�t�F�[�X

};