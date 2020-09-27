//-----------------------------------------------------------------------+
// �V�[���x�[�X�N���X.(���ۃN���X)
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

// �C���N���[�h�t�@�C��


class SceneBase
{
public:

	SceneBase() {};                               // �R���X�g���N�^
	virtual ~SceneBase() {};                      // �f�X�g���N�^

	virtual SceneBase* Update() = 0;         // �V�[���̍X�V����(���݂̃V�[���̃|�C���^��Ԃ�)

	virtual void Initialize() = 0;

	virtual void Draw() = 0;                      // �`�揈��

};