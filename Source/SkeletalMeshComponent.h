#pragma once
#include "MeshComponent.h"
#include "MatrixPalette.h"

class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(class Actor* in_owner);
	// ���b�V���R���|�[�l���g�̕`��
	virtual void Draw(class Shader* in_shader) override;

	// �V���h�E�p�̕`��
	virtual void DrawShadow(class Shader* in_shader) override;

	void Update(float in_deltaTime) override;

	// �X�P���g���̃Z�b�g
	void SetSkeleton(const class Skeleton* sk) { m_skeleton = sk; }

	// �A�j���[�V�����̍Đ��B�A�j���[�V�����̎c�蒷����Ԃ�
	float PlayAnimation(const class Animation* anim, float playRate = 1.0f);
	// ���ݍĐ����̃A�j���[�V�����͍Đ������H true : �Đ��� false : �Đ��I��
	bool IsPlaying();
	float GetPlayTime() { return m_animTime; }

protected:
	void ComputeMatrixPalette();                                           // �s��p���b�g�̌v�Z

	MatrixPalette m_palette;                                                // �s��p���b�g
	const class Skeleton* m_skeleton;                                       // �X�P���g��
	const class Animation* m_animation;                                     // �A�j���[�V����
	float m_animPlayRate;                                                   // �A�j���[�V�����̍Đ����x
	float m_animTime;                                                       // �A�j���[�V��������

};
