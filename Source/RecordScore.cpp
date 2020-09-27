//-----------------------------------------------------------------------+
// �X�R�A�̋L�^�N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "RecordScore.h"
#include "RapidJsonHelper.h"

#include <SDL.h>
#include <fstream>
#include <iostream>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>


const int RecordScore::RANK_LIMIT = 10;          // ���ʂ̍ő�l(��{�F10�ʂ܂�)


// �R���X�g���N�^
RecordScore::RecordScore(int in_score)
	:m_score(in_score)
	,m_rank(11)
{

	// ���ʂ��ƂɃX�R�A�����i�[
	for (int i = 0; i < RANK_LIMIT; i++)
	{
		// �z��̊m��
		m_ranking.push_back(0);

		// ���ʔz��Ƀf�[�^���i�[
		m_ranking[i] = LoadRankingScore("Data/Strings/path/path.json", (i + 1));
	}
	
	// �v���C���[�X�R�A���z��ɒǉ�
	m_ranking.push_back(m_score);

	// �X�R�A�̏�����Sort
	std::sort(m_ranking.begin(), m_ranking.end(), std::greater<int>());

	// �p�X��̃t�@�C���Ƀ����L���O���Z�[�u
	SaveRecord("Data/Strings/path/path.json", 0, m_ranking[0]);

}


// �f�X�g���N�^
RecordScore::~RecordScore()
{
	// �����Ȃ�
}


// �����L���O���̃��[�h
int RecordScore::LoadRankingScore(const std::string& in_fileName, int in_rankNum)
{
	// Json�t�@�C�����h�L�������g�Ƀ��[�h
	rapidjson::Document doc;
	if (!OpenJsonFile(doc, in_fileName.c_str()))
	{
		SDL_Log("Failed to Load Score %s", in_fileName.c_str());
		return false;
	}

	std::string rankID;
	// ID��ݒ�
	rankID = std::to_string(in_rankNum);

	// �X�R�A����Ԃ�
	return OutputInt(doc, rankID.c_str());
}


// �����L���O���̕��בւ�(����s�v)
void RecordScore::SortRecord(int& in_score)
{
}


// �L�^�̃Z�[�u
void RecordScore::SaveRecord(const std::string& in_fileName, const int& in_rank, const int& in_score)
{
	// �L�^����ID(����)�̍��ڂ��쐬
	std::string rankID = std::to_string(in_rank);

	// �h�L�������g�쐬
	rapidjson::Document doc;
	doc.SetObject();


	for (int i = 0; i < RANK_LIMIT; i++)
	{
		if (m_ranking[i] == m_score)
		{
			m_rank = (i + 1);
		}


		if (i == 0)
		{
			// �X�R�A���w�肵�����ʂɏ�������
			AddInt(doc.GetAllocator(), doc, "1", m_ranking[i]);
		}
		if (i == 1)
		{
			// �X�R�A���w�肵�����ʂɏ�������
			AddInt(doc.GetAllocator(), doc, "2", m_ranking[i]);
		}
		if (i == 2)
		{
			// �X�R�A���w�肵�����ʂɏ�������
			AddInt(doc.GetAllocator(), doc, "3", m_ranking[i]);
		}
		if (i == 3)
		{
			// �X�R�A���w�肵�����ʂɏ�������
			AddInt(doc.GetAllocator(), doc, "4", m_ranking[i]);
		}
		if (i == 4)
		{
			// �X�R�A���w�肵�����ʂɏ�������
			AddInt(doc.GetAllocator(), doc, "5", m_ranking[i]);
		}
		if (i == 5)
		{
			// �X�R�A���w�肵�����ʂɏ�������
			AddInt(doc.GetAllocator(), doc, "6", m_ranking[i]);
		}
		if (i == 6)
		{
			// �X�R�A���w�肵�����ʂɏ�������
			AddInt(doc.GetAllocator(), doc, "7", m_ranking[i]);
		}
		if (i == 7)
		{
			// �X�R�A���w�肵�����ʂɏ�������
			AddInt(doc.GetAllocator(), doc, "8", m_ranking[i]);
		}
		if (i == 8)
		{
			// �X�R�A���w�肵�����ʂɏ�������
			AddInt(doc.GetAllocator(), doc, "9", m_ranking[i]);
		}
		if (i == 9)
		{
			// �X�R�A���w�肵�����ʂɏ�������
			AddInt(doc.GetAllocator(), doc, "10", m_ranking[i]);
		}

		// �X�R�A���w�肵�����ʂɏ�������
		// AddInt(doc.GetAllocator(), doc, str, m_ranking[i]);

	}

	// Json��string�o�b�t�@�ɏ�������
	rapidjson::StringBuffer buffer;

	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	const char* output = buffer.GetString();

	// �O���t�@�C���ɏ����o��
	std::ofstream outFile(in_fileName);

	if (outFile.is_open())
	{
		outFile << output;
	}
}

// �����L���O���̃Q�b�^�[
const int & RecordScore::GetRankingScore(const int & in_num) const
{
	return m_ranking[in_num];
}
