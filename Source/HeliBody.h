#pragma once
#include "Helicopter.h"
#include <string>

class HeliBody : public Actor
{

public:

	HeliBody(class Helicopter* in_heli, const std::string& in_filePath);
	~HeliBody();

	void UpdateActor(float in_deltaTime) override;



private:


	class Helicopter* m_owner;


};