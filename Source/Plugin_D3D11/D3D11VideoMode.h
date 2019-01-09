#pragma once

#include <d3d11.h>
#include <Core/AresPlatform.h>

namespace Ares
{
	//--------------------------------------------
	// 显示模式	2012-02-11 帝林
	//--------------------------------------------
	class D3D11VideoMode
	{
	public:
		// 构造函数
		D3D11VideoMode();

		// 构造函数
		D3D11VideoMode( UINT width, UINT height, DXGI_FORMAT format);

	public:
		// 获取宽度
		UINT GetWidth() const { return m_width; }

		// 获取高度 
		UINT GetHeight() const { return m_height; }

		// 获取格式
		DXGI_FORMAT GetFormat() const { return m_format; }

	private:
		UINT		m_width;		// 宽
		UINT		m_height;		// 高
		DXGI_FORMAT m_format;		// 格式
	};

	// 重载操作符 "<"
	BOOL operator < ( const D3D11VideoMode& first, const D3D11VideoMode& second);

	// 重载操作符 "=="
	BOOL operator == ( const D3D11VideoMode& first, const D3D11VideoMode& second);
}