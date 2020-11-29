#include "LightBase.h"
#include "GameMain.h"
#include "Renderer.h"

// コンストラクタ
LightBase::LightBase(LIGHT_TYPE in_type)
	:m_type(in_type)
	,m_direction(Vector3(1.0, 0.0, 0.0))

{
	
}

LightBase::~LightBase()
{
}
