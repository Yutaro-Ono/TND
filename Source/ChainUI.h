//-----------------------------------------------------------------------+
// �`�F�C��UI�N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"


class ChainUI : public UIScreen
{


public:

	ChainUI(class RuleScore* in_rule);                       // �R���X�g���N�^
	~ChainUI();                                              // �f�X�g���N�^

	void Update(float in_deltaTime) override;                // �X�V����

	void Draw(class Shader* in_shader) override;             // �`�揈��


private:

	class Texture* m_chain[2];

	class Texture* m_rankTex[2];
	class Texture* m_rankNum[2];
	class Texture* m_chainRank[2];

	class Texture* m_timeGauge;

	class SpriteComponent* m_sprite;

	int m_gaugeWidth;

	class RuleScore* m_rule;

	int m_chainNum;

	int m_rank;


};