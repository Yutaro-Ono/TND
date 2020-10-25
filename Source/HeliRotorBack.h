#pragma once
#include "Helicopter.h"
#include <string>

class HeliRotorBack : public Actor
{

public:

	HeliRotorBack(class Helicopter* in_heli, const std::string& in_filePath);
	~HeliRotorBack();

	void UpdateActor(float in_deltaTime) override;



private:


	class Helicopter* m_owner;

};