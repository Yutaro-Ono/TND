#pragma once
#include "Actor.h"
#include <string>

class LampObject : public Actor
{
	
public:

	LampObject(const Vector3& in_pos);
	LampObject(const Vector3& in_pos, class Mesh* in_mesh);
	~LampObject();





private:

	class PointLight* m_headLight;
	class PointLight* m_groundLight;


};