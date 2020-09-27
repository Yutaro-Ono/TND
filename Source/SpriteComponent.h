//-----------------------------------------------------------------------+
// SpriteComponentƒNƒ‰ƒX
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include "SDL.h"


class SpriteComponent : public Component
{


public:

	SpriteComponent(class Actor* in_owner, int in_drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(class Shader* in_shader);
	virtual void SetTexture(class Texture* in_texture);

	int GetDrawOrder() const { return m_drawOrder; }
	int GetTexHeight() const { return m_textureHeight; }
	int GetTexWidth() const { return m_textureWidth; }	

	void SetVisible(bool in_visible) { m_visible = in_visible; }
	bool GetVisible() const { return m_visible; }

	class Texture* GetTexture() { return m_texture; }


protected:

	class Texture* m_texture;

	int m_drawOrder;
	int m_textureWidth;
	int m_textureHeight;

	bool m_visible;


};