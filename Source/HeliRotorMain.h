#pragma once
#include "Helicopter.h"
#include <string>

class HeliRotorMain : public Actor
{

public:

	HeliRotorMain(class Helicopter* in_heli, const std::string& in_filePath);
	~HeliRotorMain();

	void UpdateActor(float in_deltaTime) override;


private:


	class Helicopter* m_owner;

};