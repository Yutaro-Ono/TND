//---------------------------------------------+
// 橋オブジェクト
//---------------------------------------------+
#include "BridgeObject.h"
#include "LevelObject.h"
#include "Mesh.h"
#include "MeshComponent.h"

BridgeObject::BridgeObject(int in_num, const Vector3& in_startPos)
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
		m_bridges[i][6]->SetPosition(in_startPos + Vector3(0.0f, 2000.0f, -15.0f));
		m_bridges[i][6]->GetMeshComponent()->SetIntensityVal(3.5f);
	}

}

BridgeObject::~BridgeObject()
{
}
