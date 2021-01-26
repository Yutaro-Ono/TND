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
#include "LevelTerrain.h"
#include "BridgeObject.h"
#include "ClientActor.h"
#include "PatrolPoint.h"
#include "RapidJsonHelper.h"
#include "GameWorld.h"
#include "PointLight.h"
#include "LampObject.h"
#include "LoadScreen.h"
#include <vector>
#include <string>
#include <sstream>


const int LevelManager::MAP_INDEX_GROUND = 16;


// �R���X�g���N�^
LevelManager::LevelManager(GameWorld* in_world, int in_stageNum)
	:m_camera(nullptr)
{
	// �}�b�v�̔z�u��ۑ�����json�ւ̃t�@�C���p�X�𐶐�
	std::stringstream ssMap;
	ssMap << "Data/Strings/map_" << in_stageNum << ".json";
	std::string mapPath = ssMap.str();

	// �X�e�[�W���b�V���ǂݍ���
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Objects/GroundBase/Ground.OBJ"));
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Objects/Street/Street.OBJ"));
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Objects/Building/0/Build11.OBJ"));
	//m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Objects/Bridge/Bridge_Lamps/Bridge_Lamps.OBJ"));
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Objects/Lamp/SM_Lamp.OBJ"));
	//m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Body_Internal.OBJ"));
	//m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Objects/Sphere/Sphere.OBJ"));


	
	//-----------------------------------------------------------------------------------------+
    // �X�e�[�W�f�[�^�ǂݍ���
    //-----------------------------------------------------------------------------------------+
    // �n��
	std::vector<std::vector<int>> groundData;
	if (!ReadTiledJson(groundData, mapPath.c_str(), "layer_Ground"))
	{
		printf("<Level> Map Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}

	// ��
	std::vector<std::vector<int>> wallData;
	if (!ReadTiledJson(wallData, mapPath.c_str(), "layer_wall"))
	{
		printf("<Level> Street Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}


	// ����
	std::vector<std::vector<int>> buildingData;
	if (!ReadTiledJson(buildingData, mapPath.c_str(), "layer_Building"))
	{
		printf("<Level> Building Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}


	// �˗��l
	std::vector< std::vector<int>> clientData;
	if (!ReadTiledJson(clientData, mapPath.c_str(), "layer_human"))
	{
		printf("<Level> client Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}


	// �w������|�C���g
	std::vector<std::vector<int>> patrolPointData;
	if (!ReadTiledJson(patrolPointData, mapPath.c_str(), "layer_patrolPoint"))
	{
		printf("<Level> patrolPoint Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}


	// �|�C���g���C�g
	std::vector<std::vector<int>> pointLightData;
	if (!ReadTiledJson(pointLightData, mapPath.c_str(), "layer_pointLight"))
	{
		printf("<Level> pointLight Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}


	LevelBlock* block;
	LevelTerrain* terrain;
	const float blockSize = 200.0f;
	const float floorZoffset = -20.0f;

	int sizeX, sizeY;
	sizeX = groundData[0].size();
	sizeY = groundData.size();
	float offsetY = sizeY * blockSize;
	// �}�b�v�u���b�N��o�^(�n�`)
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			// �n�ʃI�u�W�F�N�g��Tiled���16�œo�^����Ă���
			if (groundData[iy][ix] == MAP_INDEX_GROUND)
			{
				terrain = new LevelTerrain(m_blockMeshes[0], LevelTerrain::TERRAIN_TYPE::TYPE_GLASS);
				terrain->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, floorZoffset));
			}

			// ���H�I�u�W�F�N�g��Tiled���1�`11�œo�^����Ă���
			if (groundData[iy][ix] >= 1 && groundData[iy][ix] <= 11)
			{
				terrain = new LevelTerrain(m_blockMeshes[1], LevelTerrain::TERRAIN_TYPE::TYPE_STREET);
				terrain->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, floorZoffset));
			}
		}
	}
	groundData.clear();

	// ���[�h����
	GAME_INSTANCE.GetLoadScreen()->AddGauge();

	// �}�b�v�u���b�N��o�^(��)
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			// �I�u�W�F�N�g��Tiled���18�œo�^����Ă���
			if (wallData[iy][ix] == 19)
			{
				block = new LevelBlock();
				block->SetMesh(m_blockMeshes[0]);
				block->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, floorZoffset + 25.0f));
				block->SetScale(1.1f);
				block->SetMeshVisible();
			}
		}
	}
	wallData.clear();

	// ���[�h����
	GAME_INSTANCE.GetLoadScreen()->AddGauge();

	// �}�b�v�u���b�N��o�^(����)
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			if (buildingData[iy][ix] == 19)
			{
				// �r���Ƃ��ăC���X�^���X�𐶐�
				block = new LevelBlock();
				block->SetMesh(m_blockMeshes[2]);
				block->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, floorZoffset + 5.0f));
				block->SetScale(5.0f);

				block->GetMeshComponent()->SetIntensityVal(5.0f);

			}
		}
	}
	buildingData.clear();

	// �}�b�v�ɓo�^(�˗��l)
	int count = 0;
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{

			if (clientData[iy][ix] == 33)
			{
				ClientActor* client = new ClientActor(Vector3(ix * blockSize, offsetY - iy * blockSize, -10.0f), count);
				count++;
				in_world->AddClientActor(client);

				// ���[�h����
				GAME_INSTANCE.GetLoadScreen()->AddGauge();
			}
		}
	}
	clientData.clear();


	// �}�b�v�ɓo�^ (�w���̏���|�C���g)
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			if (patrolPointData[iy][ix] >= 34)
			{
				PatrolPoint* patrol = new PatrolPoint(Vector3(ix * blockSize, offsetY - iy * blockSize, 1200.0f));

				in_world->AddPatrolPoint(patrol);

				// ���[�h����
				GAME_INSTANCE.GetLoadScreen()->AddGauge();

			}
		}
	}
	patrolPointData.clear();


	// �}�b�v�ɓo�^ (�|�C���g���C�g)
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			if (pointLightData[iy][ix] == 32)
			{
				//PointLight* pLight = new PointLight();
				//pLight->SetPosition(Vector3(ix* blockSize, offsetY - iy * blockSize, 0.0f));

				LampObject* lamp = new LampObject(Vector3(ix * blockSize, offsetY - iy * blockSize, -10.0f), m_blockMeshes[3]);

				// ���[�h����
				if (ix % 4 == 0)
				{
					GAME_INSTANCE.GetLoadScreen()->AddGauge();
				}

			}

		}
	}
	pointLightData.clear();

	//BridgeObject* bridge = new BridgeObject(1, Vector3::Zero);

	m_blockMeshes.clear();
	m_objectMeshes.clear();
}


// �f�X�g���N�^
LevelManager::~LevelManager()
{

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
