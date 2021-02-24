//---------------------------------------------+
// 橋オブジェクト
//---------------------------------------------+
#include "BridgeObject.h"
#include "LevelObject.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "BoxCollider.h"
#include "PointLight.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="in_num"> 橋の数 </param>
/// <param name="in_startPos"> 最初の橋オブジェクトの設置座標 </param>
/// <param name="in_bright"> 橋のライトを光らせるか </param>
BridgeObject::BridgeObject(int in_num, const Vector3& in_startPos, bool in_bright)
	:m_bridgeNum(in_num)
{
	// 各種メッシュ情報の取得
	Mesh* road = RENDERER->GetMesh("Data/Meshes/TND/Objects/Bridge/Bridge_Road/Bridge_Road.OBJ");
	Mesh* frame = RENDERER->GetMesh("Data/Meshes/TND/Objects/Bridge/Bridge_Frame/Bridge_Frame.OBJ");
	Mesh* handrail1 = RENDERER->GetMesh("Data/Meshes/TND/Objects/Bridge/Bridge_HandRail_1/Bridge_HandRail_1.OBJ");
	Mesh* handrail2 = RENDERER->GetMesh("Data/Meshes/TND/Objects/Bridge/Bridge_HandRail_2/Bridge_HandRail_2.gpmesh");
	Mesh* curb1 = RENDERER->GetMesh("Data/Meshes/TND/Objects/Bridge/Bridge_Curb_1/Bridge_Curb_1.OBJ");
	Mesh* curb2 = RENDERER->GetMesh("Data/Meshes/TND/Objects/Bridge/Bridge_Curb_2/Bridge_Curb_2.OBJ");
	Mesh* lamp = RENDERER->GetMesh("Data/Meshes/TND/Objects/Bridge/Bridge_Lamps/Bridge_Lamps.OBJ");

	// 橋オブジェクト配列の確保
	m_bridges.resize(m_bridgeNum);

	// 指定された数ぶん橋オブジェクトを作成
	for (int i = 0; i < m_bridgeNum; i++)
	{
		m_bridges[i].push_back(new LevelObject(road));
		m_bridges[i][0]->SetPosition(in_startPos + Vector3(0.0f, 2000.0f, 0.0f));
		m_bridges[i].push_back(new LevelObject(frame));
		m_bridges[i][1]->SetPosition(in_startPos + Vector3(0.0f, 2000.0f, -3950.0f));
		m_bridges[i].push_back(new LevelObject(handrail1));
		m_bridges[i][2]->SetPosition(in_startPos + Vector3(0.0f, 2000.0f, -15.0f));
		m_bridges[i].push_back(new LevelObject(handrail2));
		m_bridges[i][3]->SetPosition(in_startPos + Vector3(0.0f, 2000.0f, -15.0f));
		m_bridges[i].push_back(new LevelObject(curb1));
		m_bridges[i][4]->SetPosition(in_startPos + Vector3(0.0f, 2000.0f, -15.0f));
		m_bridges[i].push_back(new LevelObject(curb2));
		m_bridges[i][5]->SetPosition(in_startPos + Vector3(0.0f, 2000.0f, -15.0f));
		m_bridges[i].push_back(new LevelObject(lamp));
		m_bridges[i][6]->SetPosition(in_startPos + Vector3(0.0f, 2000.0f, 85.0f));

		m_bridges[i][6]->GetMeshComponent()->SetIntensityVal(3.5f);
		if (!in_bright)
		{
			m_bridges[i][6]->GetMeshComponent()->SetIntensityVal(1.0f);
		}
	}

	// ポイントライト
	int lightNum = 8 * m_bridgeNum;      // ポイントライトの数
	for (int i = 0; i < lightNum; i++)
	{
		m_pLights.push_back(new PointLight(PointLight::VL_MEDIUM));
		m_pLights[i]->SetLuminance(6.9f);

		// 奇数偶数番で左右位置を変更
		if (i == 0)
		{
			m_pLights[i]->SetPosition(in_startPos + Vector3(0.0f, 2000.0f, 0.0f) + Vector3(-2775.0f, -850.0f, 85.0f));
		}
		else if(i == 1)
		{
			m_pLights[i]->SetPosition(in_startPos + Vector3(0.0f * (float)i, 2000.0f, 0.0f) + Vector3(-2775.0f, 850.0f, 85.0f));
		}
		else if (i == 2)
		{
			m_pLights[i]->SetPosition(in_startPos + Vector3(1700.0f * 1, 2000.0f, 0.0f) + Vector3(-2775.0f, -850.0f, 85.0f));
		}
		else if (i == 3)
		{
			m_pLights[i]->SetPosition(in_startPos + Vector3(1700.0f * 1, 2000.0f, 0.0f) + Vector3(-2775.0f, 850.0f, 85.0f));
		}
		else if (i == 4)
		{
			m_pLights[i]->SetPosition(in_startPos + Vector3(1925.0f * 2, 2000.0f, 0.0f) + Vector3(-2775.0f, -850.0f, 85.0f));
		}
		else if (i == 5)
		{
			m_pLights[i]->SetPosition(in_startPos + Vector3(1925.0f * 2, 2000.0f, 0.0f) + Vector3(-2775.0f, 850.0f, 85.0f));
		}
		else if (i == 6)
		{
			m_pLights[i]->SetPosition(in_startPos + Vector3(1850.0f * 3, 2000.0f, 0.0f) + Vector3(-2775.0f, -850.0f, 85.0f));
		}
		else if (i == 7)
		{
			m_pLights[i]->SetPosition(in_startPos + Vector3(1850.0f * 3, 2000.0f, 0.0f) + Vector3(-2775.0f, 850.0f, 85.0f));
		}
		
	}


	// 当たり判定
	// ボックスコライダーの生成・定義
	// 道
	BoxCollider* roadColl = new BoxCollider(this, PhysicsWorld::TYPE_BACK_GROUND);
	AABB roadBox;
	roadBox.SetMinVector(in_startPos + in_startPos + Vector3(0.0f, 2000.0f, 0.0f) + Vector3(-6515.0f, -850.0f, -100.0f));
	roadBox.SetMaxVector(in_startPos + in_startPos + Vector3(0.0f, 2000.0f, 0.0f) + Vector3(6515.0f, 850.0f, 0.0f));
	roadColl->SetObjectBox(roadBox);
	// ガードレール(左)
	BoxCollider* railCollL = new BoxCollider(this, PhysicsWorld::TYPE_BACK_GROUND);
	AABB railLBox;
	railLBox.SetMinVector(in_startPos + in_startPos + Vector3(0.0f, 2000.0f, 0.0f) + Vector3(-6515.0f, -850.0f, -100.0f));
	railLBox.SetMaxVector(in_startPos + in_startPos + Vector3(0.0f, 2000.0f, 0.0f) + Vector3(6515.0f, -775.0f, 30.0f));
	railCollL->SetObjectBox(railLBox);
	// ガードレール(右)
	BoxCollider* railCollR = new BoxCollider(this, PhysicsWorld::TYPE_BACK_GROUND);
	AABB railRBox;
	railRBox.SetMinVector(in_startPos + in_startPos + Vector3(0.0f, 2000.0f, 0.0f) + Vector3(-6515.0f, 775.0f, -100.0f));
	railRBox.SetMaxVector(in_startPos + in_startPos + Vector3(0.0f, 2000.0f, 0.0f) + Vector3(6515.0f, 850.0f, 30.0f));
	railCollR->SetObjectBox(railRBox);
}

BridgeObject::~BridgeObject()
{
}
