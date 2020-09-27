//-----------------------------------------------------------------------+
// メッシュクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"

#include <SDL_log.h>
#include "Math.h"





Mesh::Mesh()
	:m_vertexArray(nullptr)
	,m_radius(0.0f)
	,m_specValue(100.0f)
{
}

Mesh::~Mesh()
{
}


void Mesh::Delete()
{
	delete m_vertexArray;
	m_vertexArray = nullptr;
}

Texture * Mesh::GetTexture(size_t in_index)
{

	if (in_index < m_textures.size())
	{
		return m_textures[in_index];
	}
	else
	{
		return nullptr;
	}

}
