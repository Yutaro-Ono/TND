#pragma once
#include "Actor.h"
#include <string>


class HeliSpotLight : public Actor
{

public:

	HeliSpotLight(class Helicopter* in_heli, const std::string& in_filePath);
	~HeliSpotLight();

	void UpdateActor(float in_deltaTime);

private:

	class Helicopter* m_owner;


};