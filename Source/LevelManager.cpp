//---------------------------------------------------------------------------+
// LevelManager�N���X.  �}�b�v�̃u���b�N�z�u�A�I�u�W�F�N�g�z�u�����N���X
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//---------------------------------------------------------------------------+
#include "LevelManager.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "LevelBlock.h"
#include "RapidJsonHelper.h"
#include "FlameStatue.h"
#include "AngelStatue.h"
#include "Skydome.h"

#include <vector>
#include <string>
#include <sstream>


// �R���X�g���N�^
LevelManager::LevelManager(int in_stageNum)
	:m_camera(nullptr)
{
	// �}�b�v�̔z�u��ۑ�����json�ւ̃t�@�C���p�X�𐶐�
	std::stringstream ssMap;
	ssMap << "Data/Strings/map_" << in_stageNum << ".json";
	std::string mapPath = ssMap.str();

	// �X�e�[�W���b�V���ǂݍ���
	// m_blockMeshes.push_back(&emptySpace);
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Block_Wall/mapBlock1.gpmesh"));
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Block_Wall/mapBlock1.gpmesh"));
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Block_Floor/BLOCK_FLOOR.gpmesh"));
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Pillar/PILLAR.gpmesh"));
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Rock/Object_Rock.gpmesh"));

	// �I�u�W�F�N�g�p�̔z��Ƀ��b�V�����i�[
	m_objectMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Pillar/PILLAR.gpmesh"));
	m_objectMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Rock/Object_Rock.gpmesh"));
	m_objectMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Angel_Statue/Statue_Ruins.gpmesh"));
	m_objectMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Skydome/Skydome_Sunny.gpmesh"));



	//-----------------------------------------------------------------------------------------+
	// �X�e�[�W�f�[�^�ǂݍ���
	//-----------------------------------------------------------------------------------------+
	// �n��
	std::vector<std::vector<int>> groundData;
	if (!ReadTiledJson(groundData, mapPath.c_str(), "layer_0"))
	{
		printf("<Level> Map Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}
	// �I�u�W�F�N�g
	std::vector<std::vector<int>> objData;
	if (!ReadTiledJson(objData, mapPath.c_str(), "layer_1"))
	{
		printf("<Level> Map Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}


	LevelBlock* block;
	const float blockSize = 200.0f;

	int sizeX, sizeY;
	sizeX = groundData[0].size();
	sizeY = groundData.size();
	float offsetY = sizeY * blockSize;

	// �}�b�v�u���b�N��o�^(�n�`)
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			if (groundData[iy][ix] == 0)
			{
				continue;
			}

			if (groundData[iy][ix] < 5)
			{
				block = new LevelBlock();
				block->SetMesh(m_blockMeshes[groundData[iy][ix]]);
				block->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, -100.0f));
			}

		}
	}

	// �}�b�v�u���b�N��o�^(�I�u�W�F�N�g)
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			if (objData[iy][ix] == 0)
			{
				continue;
			}

			// ���I�u�W�F�N�g
			if (objData[iy][ix] == 3)
			{
				block = new LevelBlock();
				block->SetMesh(m_blockMeshes[objData[iy][ix]]);
				block->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, 0.0f));

				continue;
			}

			// ��I�u�W�F�N�g
			if (objData[iy][ix] == 4)
			{
				block = new LevelBlock();
				block->SetMesh(m_blockMeshes[objData[iy][ix]]);
				block->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, -50.0f));

				continue;
			}

			// �ǃu���b�N
			if (objData[iy][ix] < 5)
			{
				block = new LevelBlock();
				block->SetMesh(m_blockMeshes[objData[iy][ix]]);
				block->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, 100.0f));
			}
		}
	}

}


// �f�X�g���N�^
LevelManager::~LevelManager()
{
	// �n�`�u���b�N�̃N���A
	m_blockMeshes.clear();
	// �I�u�W�F�N�g�̃N���A
	m_objectMeshes.clear();

}


// ���x���̃��[�h���� ��������
bool LevelManager::LoadLevel(const char * in_json, int in_layerNum)
{
	return true;
}


// �r���[�����W�̃Z�b�^�[
void LevelManager::SetViewRange(float in_width, float in_height)
{
	m_leftUpPos.x = -in_width * 0.5f;
	m_rightDownPos.x = in_width * 0.5f;
	m_leftUpPos.y = -in_height * 0.5f;
	m_rightDownPos.y = in_height * 0.5f;
}


// �X�V����
void LevelManager::Update()
{
	// �����Ȃ�
}


// �Ƃ����Δz�u�p��json���擾����
bool LevelManager::ReadTiledJson(std::vector<std::vector<int>>& mapData, const char * in_fileName, const char * in_layerName)
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
	mapData.resize(height);
	for (auto &mapIter : mapData)
	{
		mapIter.resize(width);
	}

	// ���[�U�[�z��i2�����j�Ƀf�[�^���R�s�[����
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			mapData[y][x] = rapidArrayData[y * width + x].GetInt();
		}
	}

	return true;
}


// json����layer�𒲂ׂ�
int LevelManager::FindLayerIndex(rapidjson::GenericArray<false, rapidjson::Value>& layer, std::string & layerName)
{
	int  layerSize = layer.Size();

	int i;
	std::string layerNameString(layerName);
	for (i = 0; i < layerSize; i++)
	{
		std::string currentLayerName = layer[i]["name"].GetString();
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
