//-----------------------------------------------------------------------+
// �X�R�AUI�N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"

class ScoreUI : public UIScreen
{


public:

	ScoreUI(class Canvas* in_canvas);                   // �R���X�g���N�^
	~ScoreUI();                                          // �f�X�g���N�^

	void Update(float in_deltaTime) override;            // �X�V����

	void Draw(class Shader* in_shader) override;                  // �`�揈��

	//-------------------------------------------------+
	// Setter/Getter
	//-------------------------------------------------+
	void AddScore();    // �X�R�A�̉��Z����
	void SubScore();    // �X�R�A�̌��Z����
	// ���Z���X�R�A�̃Z�b�g�Ƃ���ɔ����e�N�X�`������
	void SetAddScore(int in_score);

	// ���݂̃X�R�A (���Z���܂�)
	int GetScore() { return m_score + m_addScore; }

private:

	class Canvas* m_canvas;

	class Texture* m_scoreTex[2];
	class Texture* m_addScoreTex[2];
	class Texture* m_subScoreTex[2];

	Vector2 m_scoreTexPos;
	Vector2 m_addScoreTexPos;
	Vector2 m_subScoreTexPos;

	int m_score;      // ���t���[���̃X�R�A
	int m_addScore;      // �X�R�A�̒ǉ����i�[�p

	bool m_drawSubScore;

	int m_fontSize;

	// �X�R�A���Z���ɗp����J�E���^�[
	bool m_startSub;
	unsigned int m_time;
	unsigned int m_coolTime;
};