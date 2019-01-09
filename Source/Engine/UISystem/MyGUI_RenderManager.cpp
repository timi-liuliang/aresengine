/*!
	@file
	@author		Albert Semenov
	@date		04/2009
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Engine/UISystem/MyGUI_Precompiled.h"
#include "Engine/UISystem/MyGUI_RenderManager.h"
#include "Engine/UISystem/AresUISystem.h"
#include "Engine/UISystem/MyGUI_LayerManager.h"
#include "Engine/UISystem/MyGUI_Texture.h"
#include "Engine/UISystem/MyGUI_VertexBuffer.h"

namespace MyGUI
{
	// Create vertex buffer.
	// This method should create vertex buffer with triangles list type,
	// each vertex have position, colour, texture coordinates.
	IVertexBuffer* RenderManager::createVertexBuffer()
	{ 
		return new_ MyGUIBuffer; 
	}

	// Create empty texture instance 
	ITexture* RenderManager::createTexture(const std::string& _name)
	{
		TextureMap::iterator it = m_textures.find( _name);
		if( it!=m_textures.end())
			return it->second;

		// ÐÂ½¨
		MyGUITexture* texture = new MyGUITexture( _name);
		m_textures[_name] = texture;

		return texture;
	}

	/** Get texture by name */
	ITexture* RenderManager::getTexture(const std::string& _name) 
	{
		TextureMap::iterator it = m_textures.find( _name);
		if( it!=m_textures.end())
			return it->second;

		return NULL; 
	}

	bool RenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		if( _format==PixelFormat::L8A8)
			return false;

		return true;
	}

#if MYGUI_DEBUG_MODE == 1
	bool RenderManager::checkTexture(ITexture* _texture)
	{
		return true;
	}
#endif

	void RenderManager::onResizeView(const IntSize& _viewSize)
	{
		LayerManager* layers = LayerManager::GetSingletonPtr();
		if (layers != nullptr)
			layers->resizeView(_viewSize);
	}
} // namespace MyGUI
