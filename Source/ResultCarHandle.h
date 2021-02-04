#pragma once
#include "Actor.h"
#include <string>

class ResultCarHandle : public Actor
{

public:

	ResultCarHandle(class Actor* in_owner, const std::string& in_meshPath);
	~ResultCarHandle();
	void UpdateActor(float in_deltaTime) override;


	class MeshComponent* GetMeshComponent() { return m_meshComp; }     // メッシュへのポインタゲッター

private:


	class Actor* m_owner;

};

