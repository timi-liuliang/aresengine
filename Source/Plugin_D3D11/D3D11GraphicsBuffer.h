#pragma once

#include "D3D11Typedefs.h"
#include <Core/Graphics/AresElementFormat.h>
#include <Core/Graphics/AresGraphicsBuffer.h>

namespace Ares
{
	//-------------------------------------
	// D3D11GraphicsBuffer 2012-4-14 帝林
	//-------------------------------------
	class D3D11GraphicsBuffer : public GraphicsBuffer
	{
	public:
		// 构造函数
		D3D11GraphicsBuffer( BufferUsage usage, UINT accessHint, UINT bindFlags, const ElementInitData* initData, ElementFormat format);

		// 析构函数
		~D3D11GraphicsBuffer() {}

		// 获取D3D11Buffer
		const ID3D11BufferPtr& GetD3D11Buffer() const { return m_d3d11Buffer; }

		// 重置大小
		virtual void Resize( UINT sizeInByte);

	protected:
		// 获取D3D标记
		void GetD3DFlags( D3D11_USAGE& usage, UINT& cpuAccessFlags, UINT& bindFlags, UINT& miscFlags);

	private:
		// Map
		virtual void* Map( BufferAccess bufAccess);

		// Unmap
		virtual void Unmap();

	private:
		ID3D11DevicePtr				 m_d3d11Device;
		ID3D11DeviceContextPtr		 m_d3d11ImmCtx;
		ID3D11BufferPtr				 m_d3d11Buffer;
		ID3D11ShaderResourceViewPtr	 m_d3d11SrView;			// Shader Resource View
		ID3D11UnorderedAccessViewPtr m_d3d11Uaview;			// Unordered Access View

		UINT						 m_bindFlags;
		UINT						 m_hwBufSize;
		ElementFormat				 m_formatAsShaderRes;	// format as shader resource
	};
}