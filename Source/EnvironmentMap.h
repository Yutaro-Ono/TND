#pragma once
#include "MeshComponent.h"

class EnvironmentMap : public MeshComponent
{

public:

	EnvironmentMap(class Actor* in_owner);
	~EnvironmentMap();

	virtual void Draw(class Shader* in_shader) override;                             // メッシュコンポーネントの描画
	virtual void DrawShadow(class Shader* in_shader) override;                               // シャドウ生成用の描画

private:



};