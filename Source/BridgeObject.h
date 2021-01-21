//---------------------------------------------+
// 橋オブジェクト
//---------------------------------------------+
#pragma once
#include "Math.h"
#include <vector>
#include <string>

class BridgeObject
{
public:


	BridgeObject(int in_num, const Vector3& in_startPos);
	~BridgeObject();



private:

	
	int m_bridgeNum;                                                // 橋の数

	std::vector<std::vector<class LevelObject*>> m_bridges;         // 橋オブジェクト配列


	// 橋モデルのファイルパス群
	static const std::string BRIDGE_ROAD;
	static const std::string BRIDGE_FRAME;
	static const std::string BRIDGE_HANDRAIL_A;
	static const std::string BRIDGE_HANDRAIL_B;
	static const std::string BRIDGE_CURB_A;
	static const std::string BRIDGE_CURB_B;
	static const std::string BRIDGE_LAMPS;
};