//----------------------------------------------------------------------------------------+
//
// GBuffer(���W�E�@���E�A���x�h���X�y�L����)�֕`�挋�ʂ��o�͂���
//
//----------------------------------------------------------------------------------------+

#pragma once



class GBuffer
{

private:






public:


	unsigned int m_gBuffer;      // G-Buffer (3�v�f�����t���[���o�b�t�@)

	// G-Buffer�Ɋ֘A�t����e����
	unsigned int m_gPos;             // 3D��ԍ��W
	unsigned int m_gNormal;          // �@���x�N�g��
	unsigned int m_gAlbedoSpec;      // �A���x�h(RGB)���X�y�L����(A)

	unsigned int m_gRBO;             // G-Buffer�ɕR�t����`��o�b�t�@

};