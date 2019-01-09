#pragma once

#include <vector>
#include <d3d11.h>
#include <Core/AresPtr.h>
#include "D3D11VideoMode.h"
#include <boost/shared_ptr.hpp>

using namespace std;

namespace Ares
{
	//-------------------------------------------
	// D3D11适配器 2012-02-10 帝林
	// 1.用于查询设备能力
	//-------------------------------------------
	class D3D11Adapter
	{
	public:
		// 构造函数
		D3D11Adapter();

		// 构造函数
		D3D11Adapter( UINT idx, IDXGIAdapter1* adapter);

	public:
		// 枚举显示模式
		void Enumerate();

		// 获取描述信息
		const wstring GetDescription() const;

		// 获取d3d11适配器
		IDXGIAdapter1* GetAdapter() { return m_adapter; }

	private:
		UINT					m_idx;			// 这是第几块适配器
		IDXGIAdapter1*			m_adapter;		// 适配器
		DXGI_ADAPTER_DESC1		m_adapterDesc;	// 适配器描述
		vector<D3D11VideoMode>	m_modes;		// 分辨率等
	};
}