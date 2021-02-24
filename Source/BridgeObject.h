//---------------------------------------------+
// ���I�u�W�F�N�g
//---------------------------------------------+
#pragma once
#include "Actor.h"
#include "Math.h"
#include <vector>
#include <string>

class BridgeObject : public Actor
{
public:


	BridgeObject(int in_num, const Vector3& in_startPos, bool in_bright = true);
	~BridgeObject();



private:

	
	int m_bridgeNum;                                                // ���̐�

	std::vector<std::vector<class LevelObject*>> m_bridges;         // ���I�u�W�F�N�g�z��

	std::vector<class PointLight*> m_pLights;

	// �����f���̃t�@�C���p�X�Q
	static const std::string BRIDGE_ROAD;
	static const std::string BRIDGE_FRAME;
	static const std::string BRIDGE_HANDRAIL_A;
	static const std::string BRIDGE_HANDRAIL_B;
	static const std::string BRIDGE_CURB_A;
	static const std::string BRIDGE_CURB_B;
	static const std::string BRIDGE_LAMPS;
};