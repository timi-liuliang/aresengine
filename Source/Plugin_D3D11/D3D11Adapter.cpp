#include <vector>
#include <algorithm>
#include "D3D11Adapter.h"

namespace Ares
{
	// 构造函数
	D3D11Adapter::D3D11Adapter()
		: m_idx( 0)
		, m_adapter( NULL)
	{

	}

	// 构造函数
	D3D11Adapter::D3D11Adapter( UINT idx, IDXGIAdapter1* adapter)
		: m_idx( idx),
		  m_adapter( adapter)
	{
		m_adapter->GetDesc1( &m_adapterDesc);
	}

	// 枚举显示模式
	void D3D11Adapter::Enumerate()
	{
		std::vector<DXGI_FORMAT> formats;
		formats.push_back( DXGI_FORMAT_R8G8B8A8_UNORM);
		formats.push_back( DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
		formats.push_back( DXGI_FORMAT_B8G8R8A8_UNORM);
		formats.push_back( DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);
		formats.push_back( DXGI_FORMAT_R10G10B10A2_UNORM);

		UINT i=0;
		IDXGIOutput* output = NULL;
		while( m_adapter->EnumOutputs( i, &output) != DXGI_ERROR_NOT_FOUND)
		{
			if( output)
			{
				for( UINT j=0; j<formats.size(); j++)
				{
					UINT num;
					output->GetDisplayModeList( formats[j], DXGI_ENUM_MODES_SCALING, &num, 0);
					if( num > 0)
					{
						std::vector<DXGI_MODE_DESC> modeDescs( num);
						output->GetDisplayModeList( formats[j], DXGI_ENUM_MODES_SCALING, &num, &modeDescs[0]);

						for ( UINT k=0; k<modeDescs.size(); k++)
						{
							D3D11VideoMode videoMode( modeDescs[k].Width, modeDescs[k].Height, modeDescs[k].Format);

							// 如果是新模式, 加入模式列表
							if( std::find( m_modes.begin(), m_modes.end(), videoMode) == m_modes.end())
							{
								m_modes.push_back( videoMode);
							}
						}
					}
				}

				output->Release();
			}

			++i;
		}

		std::sort( m_modes.begin(), m_modes.end());
	}

	// 获取描述信息
	const wstring D3D11Adapter::GetDescription() const
	{
		return m_adapterDesc.Description;
	}
}