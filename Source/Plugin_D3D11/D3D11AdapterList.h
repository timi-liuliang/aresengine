#pragma once

#include <vector>
#include <d3d11.h>
#include "D3D11Adapter.h"

using namespace std;

namespace Ares
{
	//-----------------------------------------
	// D3D11适配器列表 2012-02-10 帝林
	//-----------------------------------------
	class D3D11AdapterList
	{
	public:
		// 构造函数
		D3D11AdapterList();

		// 根据IDXGIFactory1枚举所有的适配器
		void Enumerate( IDXGIFactory1* dxgiFactory1);

		// 根据索引获取适配器
		D3D11Adapter* GetAdapter( UINT idx) const;

		// 获取Adapter数量
		UINT GetAdapterNum() const { return m_adapters.size(); }

		// 获取当前Adapter索引
		UINT GetCurrentAdapterIdx() const { return m_curAdapterIdx; }

		// 设置当前适配器
		void SetCurrentAdapterIdx( UINT adapterIdx) { m_curAdapterIdx = adapterIdx; }

	private:
		UINT					m_curAdapterIdx;// 当前适配器索引
		vector<D3D11Adapter*>	m_adapters;		// 适配器列表
	};
}