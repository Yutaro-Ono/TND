#pragma once
#include "Actor.h"


class LampObject : public Actor
{
	
public:

	LampObject(const Vector3& in_pos);
	~LampObject();





private:

	class PointLight* m_headLight;
	class PointLight* m_groundLight;


};