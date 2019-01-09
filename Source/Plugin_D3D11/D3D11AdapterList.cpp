#include "D3D11AdapterList.h"

namespace Ares
{
	// 构造函数
	D3D11AdapterList::D3D11AdapterList()
		: m_curAdapterIdx( 0)
	{

	}

	// 根据IDXGIFactory1枚举所有的适配器
	void D3D11AdapterList::Enumerate( IDXGIFactory1* dxgiFactory1)
	{
		// 枚举系统中的适配器
		IDXGIAdapter1* dxgiAdapter = NULL;
		for ( UINT i=0; dxgiFactory1->EnumAdapters1( i, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			if( dxgiAdapter)
			{
				DXGI_ADAPTER_DESC1 adapterDesc;
				dxgiAdapter->GetDesc1( &adapterDesc);

				if( 0 == wcscmp( adapterDesc.Description, L"NVIDIA_PerfHUD"))
				{
					m_curAdapterIdx = i;
				}

				D3D11Adapter* adapter = new_ D3D11Adapter( i, dxgiAdapter);
				adapter->Enumerate();

				m_adapters.push_back( adapter);
			}
		}

		// 如果没有找到兼容的设备则抛出错误
		if( m_adapters.empty())
		{
			//THR();
		}
	}

	// 根据索引获取适配器
	D3D11Adapter* D3D11AdapterList::GetAdapter( UINT idx) const
	{
		A_ASSERT( idx < m_adapters.size());

		return m_adapters[idx];
	}
}