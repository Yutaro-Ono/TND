//-----------------------------------------------------------------------+
// �Ƃ����΂̊Ǘ��N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "FlameManager.h"
#include "FlameActor.h"
#include "Player.h"
#include "RapidJsonHelper.h"
#include <vector>
#include <string>
#include <sstream>
#include "stdio.h"

const int FlameManager::MAP_ALL_NUM = 4;                   // �S�Ă̂Ƃ����Δz�u�}�b�v
const float FlameManager::FLAME_SIZE = 120.0f;             // �Ƃ����΂̊�{�T�C�Y


// �R���X�g���N�^
FlameManager::FlameManager(class Player* in_target, int in_stageNum)
	:m_target(in_target)
	,m_getRank(0)
	,m_rank(0)
	,m_toNext(false)
{
	// �X�e�[�W�z�u�t�@�C���̃p�X�𐶐�
	std::stringstream ssMap;
	ssMap << "Data/Strings/map_" << in_stageNum << ".json";
	std::string mapPath;
	mapPath = ssMap.str();


	// �Ƃ����΂̔z�u�f�[�^���A�X�^�b�N���m��
	while (m_mapData.size() != MAP_ALL_NUM)
	{
		m_mapData.emplace_back();
	}

	//-------------------------------------------------------------------+
	// �X�e�[�W�f�[�^�ǂݍ��� (�e4�p�^�[��)
	//-------------------------------------------------------------------+
	for (int i = 0; i < MAP_ALL_NUM; i++)
	{
		if (i == 0)
		{
			if (!ReadTiledJson(m_mapData[i], mapPath.c_str(), "layerFlame_A"))
			{
				printf("<Flame> Map Load : Failed\n");
				GAME_INSTANCE.SetShutDown();
				return;
			}
		}
		if (i == 1)
		{
			if (!ReadTiledJson(m_mapData[i], mapPath.c_str(), "layerFlame_B"))
			{
				printf("<Flame> Map Load : Failed\n");
				GAME_INSTANCE.SetShutDown();
				return;
			}
		}
		if (i == 2)
		{
			if (!ReadTiledJson(m_mapData[i], mapPath.c_str(), "layerFlame_C"))
			{
				printf("<Flame> Map Load : Failed\n");
				GAME_INSTANCE.SetShutDown();
				return;
			}
		}
		if (i == 3)
		{
			if (!ReadTiledJson(m_mapData[i], mapPath.c_str(), "layerFlame_D"))
			{
				printf("<Flame> Map Load : Failed\n");
				GAME_INSTANCE.SetShutDown();
				return;
			}
		}

	}

	// �ݒu����ۂ̊Ԋu�p�T�C�Y
	const float setSize = 200.0f;
	int sizeX, sizeY;

	sizeX = m_mapData[0][0].size();
	sizeY = m_mapData[0].size();

	float offsetY = sizeY * setSize;

	// �Ƃ����΂̔z�u (�p�^�[��A)
	for (int layer = 0; layer < 1; layer++)
	{
		for (int iy = 0; iy < sizeY; iy++)
		{
			for (int ix = 0; ix < sizeX; ix++)
			{
				if (m_mapData[layer][iy][ix] == 0)
				{
					continue;
				}

				if (m_mapData[layer][iy][ix] >= 5)
				{
					m_flameActor.push_back(new FlameActor);
					m_flameActor.back()->SetFlamePosition(Vector3(ix * setSize, offsetY - iy * setSize, 50.0f));
					m_flameActor.back()->SetSphereCollider(m_flameActor.back()->GetPosition(), FLAME_SIZE);
				}
			}
		}
	}
}

// �f�X�g���N�^
FlameManager::~FlameManager()
{
	Delete();
}

// �}�b�v�̃��[�h
bool FlameManager::LoadMap(const char* in_mapData)
{
	return false;
}

// �������
void FlameManager::Delete()
{
	// �S�Ă̂Ƃ����΂��폜
	for (auto iter : m_flameActor)
	{
		iter->SetState(Actor::STATE_DEAD);
	}

}

// �X�V����
void FlameManager::UpdateActor(float in_deltaTime)
{
	// �����蔻��Ɛ������Ă��邩�ǂ������`�F�b�N
	CollisionCheck();
	CheckAlive();
}

// �Ƃ����΂��������Ă��邩�ǂ����̃`�F�b�N
void FlameManager::CheckAlive()
{

	// �Ƃ����΃X�^�b�N�̃T�C�Y��0(�S�Ă̂Ƃ����΂������Ă����Ȃ�)�A���̃p�^�[����z�u����
	if (m_flameActor.size() == 0)
	{
		// �����N�A�b�v
		m_rank++;

		// �S�ẴX�e�[�W�����؂�����ŏ�(�p�^�[��A)�ɖ߂�
		if (m_rank == MAP_ALL_NUM)
		{
			m_rank = 0;
		}

		m_toNext = true;
	}

	// �S�Ď���ł�����A���̃p�^�[����z�u
	if (m_toNext == true)
	{
		const float setSize = 200.0f;
		int sizeX, sizeY;

		sizeX = m_mapData[0][0].size();
		sizeY = m_mapData[0].size();

		float offsetY = sizeY * setSize;

		// �����Ă��Ȃ���Ύ��̂Ƃ����΂�z�u
		for (int iy = 0; iy < sizeY; iy++)
		{
			for (int ix = 0; ix < sizeX; ix++)
			{
				if (m_mapData[m_rank][iy][ix] == 0)
				{
					continue;
				}

				if (m_mapData[m_rank][iy][ix] >= 5)
				{
					m_flameActor.push_back(new FlameActor);
					m_flameActor.back()->SetFlamePosition(Vector3(ix * setSize, offsetY - iy * setSize, 50.0f));
					m_flameActor.back()->SetSphereCollider(m_flameActor.back()->GetPosition(), FLAME_SIZE);
				}
			}
		}

		m_toNext = false;
	}
}

// �����蔻��`�F�b�N
void FlameManager::CollisionCheck()
{
	// �Ƃ����ΑS�̂̓����蔻��`�F�b�N
	for (int i = 0; i < m_flameActor.size(); i++)
	{

		m_flameActor[i]->CollisionCheck(m_target);

		// ����ł��������
		if (m_flameActor[i]->GetState() == Actor::STATE_DEAD)
		{
			m_getRank++;
			m_flameActor.erase(m_flameActor.begin() + i);
		}
	}
}

// �Ƃ����΂̃��Z�b�g(�t���[���[�h���Q�[�����[�h�ڍs���̏���)
void FlameManager::ResetFlame()
{
	for (auto iter : m_flameActor)
	{
		iter->SetState(Actor::STATE_DEAD);
	}

	for (int i = 0; i < m_flameActor.size(); i++)
	{

		// ����ł��������
		if (m_flameActor[i]->GetState() == Actor::STATE_DEAD)
		{
			m_flameActor.erase(m_flameActor.begin() + i);
		}
	}

	const float setSize = 200.0f;
	int sizeX, sizeY;

	sizeX = m_mapData[0][0].size();
	sizeY = m_mapData[0].size();

	float offsetY = sizeY * setSize;

	// �����Ă��Ȃ���Ύ��̂Ƃ����΂�z�u
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			if (m_mapData[0][iy][ix] == 0)
			{
				continue;
			}

			if (m_mapData[0][iy][ix] >= 5)
			{
				m_flameActor.push_back(new FlameActor);
				m_flameActor.back()->SetFlamePosition(Vector3(ix * setSize, offsetY - iy * setSize, 50.0f));
				m_flameActor.back()->SetSphereCollider(m_flameActor.back()->GetPosition(), FLAME_SIZE);
			}
		}
	}
}


// Json�̓ǂݍ���
bool FlameManager::ReadTiledJson(std::vector<std::vector<int>>& in_mapData, const char * in_fileName, const char * in_layerName)
{
	//RapidJson�h�L�������g�Ƃ��ĊJ���邩�H
	rapidjson::Document doc;
	if (!OpenJsonFile(doc, in_fileName))
	{
		return false;
	}

	// �J����Docment��type �� map���H
	if (!IsExistMember(doc, "type", "map"))
	{
		return false;
	}

	// �J����Document��layer�͂��邩�H
	if (!IsExistArrayName(doc, "layers"))
	{
		return false;
	}
	// layer���擾
	auto layer = doc["layers"].GetArray();
	int  layerSize = layer.Size();

	// ���C���[������Y�����C���[�̓Y�����l�𒲂ׂ�
	std::string layerNameString(in_layerName);
	int layerIndex = FindLayerIndex(layer, layerNameString);
	if (layerIndex < 0)
	{
		return false;
	}

	// layer����data�͂���H    
	rapidjson::Value::ConstMemberIterator itr = layer[layerIndex].FindMember("data");
	if (itr == layer[layerIndex].MemberEnd())
	{
		printf("���C���[��:%s�Ƀ}�b�v�f�[�^������܂���\n", in_layerName);
		return false;
	}

	// ���C���[���rapidjson�z��쐬
	const rapidjson::Value& rapidArrayData = layer[layerIndex]["data"].GetArray();
	int width, height;
	width = layer[layerIndex]["width"].GetInt();
	height = layer[layerIndex]["height"].GetInt();

	// ���[�U�[�z��m�� �s�����A������̏��ԂɃT�C�Y�m��
	in_mapData.resize(height);
	for (auto &mapIter : in_mapData)
	{
		mapIter.resize(width);
	}

	// ���[�U�[�z��i2�����j�Ƀf�[�^���R�s�[����
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			in_mapData[y][x] = rapidArrayData[y * width + x].GetInt();
		}
	}

	return true;
}


int FlameManager::FindLayerIndex(rapidjson::GenericArray<false, rapidjson::Value>& in_layer, std::string & in_layerName)
{
	int  layerSize = in_layer.Size();

	int i;
	std::string layerNameString(in_layerName);
	for (i = 0; i < layerSize; i++)
	{
		std::string currentLayerName = in_layer[i]["name"].GetString();
		printf("%s\n", currentLayerName.c_str());
		if (layerNameString == currentLayerName)
		{
			break;
		}
	}
	if (i == layerSize)
	{
		return -1;
	}

	return i;
}
