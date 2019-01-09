#pragma once

#include <string>
#include "MyGUI_ITexture.h"
#include <Core/Graphics/AresTexture.h>

using namespace Ares;

namespace MyGUI
{
	//--------------------------------
	// UI用纹理 2012-10-16 帝林
	//--------------------------------
	class MyGUITexture : public ITexture
	{
	public:
		// 构造函数 
		MyGUITexture( const std::string& name);

		// 析构函数
		~MyGUITexture();

		// 获取名称
		virtual const std::string& getName() const { return m_name; }

		// 手动创建
		virtual void createManual( int width, int height, TextureUsage usage, PixelFormat format);

		// 资源加载
		virtual void loadFromFile( const std::string& fileName);

		// 销毁
		virtual void destroy() { m_texture = TexturePtr(); }

		// 保存
		virtual void saveToFile( const std::string& fileName) {}

		// 获取宽
		virtual int getWidth() { return m_texture->GetWidth(0); }

		// 获取高
		virtual int getHeight() { return m_texture->GetHeight(0); }

		// 获取Texture
		virtual TexturePtr GetTexture() { return m_texture; }

		// 获取渲染目标
		virtual IRenderTarget* getRenderTarget();

	private:
		std::string		m_name;			// 名称
		TexturePtr		m_texture;		// 纹理
		IRenderTarget*	m_renderTarget;
	};
}