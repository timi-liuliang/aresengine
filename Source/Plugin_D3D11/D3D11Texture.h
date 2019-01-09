#pragma once

#include <vector>
#include "D3D11Typedefs.h"
#include "D3D11RenderDevice.h"
#include <Core/Graphics/AresTexture.h>

using namespace std;

namespace Ares
{
	//----------------------------------------
	// D3D11Texture 2012-3-3   帝林 
	//----------------------------------------
	class D3D11Texture : public Texture
	{
	public:
		D3D11Texture( TextureType type, UINT sampleCount, UINT sampleQuality, UINT accessHint);

		// destructor
		virtual ~D3D11Texture();

		// 获取宽
		virtual UINT GetWidth( UINT levle) const;

		// 获取高
		virtual UINT GetHeight( UINT levle) const;

		// 获取深度
		virtual UINT GetDepth( UINT level) const;

		// 检索ShaderResourceView
		virtual const ID3D11ShaderResourceViewPtr& RetriveD3DShaderResourceView( UINT mostDetailedMip=0, UINT mipLevels=1);

		// 检索RenderTargetView
		virtual const ID3D11RenderTargetViewPtr& RetriveD3DRenderTargetView( UINT firstArrayIdx, UINT arraySize, UINT level);

		// 获取D3D资源
		virtual ID3D11ResourcePtr GetD3DResource() const { return ID3D11ResourcePtr(); }

	protected:
		// 获取D3DFlags
		void GetD3DFlags( D3D11_USAGE& usage, UINT& bindFlags, UINT& cpuAccessFlags, UINT& miscFlags);

		// RetriveD3DSRV
		const ID3D11ShaderResourceViewPtr& RetriveD3DSRV( const D3D11_SHADER_RESOURCE_VIEW_DESC& desc);

		// RetriveD3DDSV
		const ID3D11DepthStencilViewPtr& RetriveD3DDSV( const D3D11_DEPTH_STENCIL_VIEW_DESC& desc);

		// 拷贝纹理
		bool CopyToSubTexture( Texture& target, UINT dstSubRes, UINT dstXOffset, UINT dstYOffset, UINT dstZOffset, UINT dstWidth, UINT dstHeight, UINT dstDepth, UINT srcSubRes, UINT srcXOffset, UINT srcYOffset, UINT srcZOffset, UINT srcWidth, UINT srcHeight, UINT srcDepth);

	private:
		// Map2D texture
		virtual void Map2D( UINT arrayIndex, UINT level, TextureMapAccess access, UINT xOffset, UINT yOffset, UINT width, UINT height, void*&data, UINT& rowPitch);

		// Unmaap2D
		virtual void Unmap2D( UINT arrayIndex, UINT level);

	protected:
		// 辅助结构体
		struct RTVDSVCreation
		{
			UINT m_firstArrayIdx;
			UINT m_arraySize;
			UINT m_level;

			union ForTex3DOrCube
			{
				struct ForTex3D
				{
					UINT	m_firstSlice;
					UINT	m_numSlices;
				} m_for3D;

				struct ForTexCube
				{
					Texture::CubeFaces m_face;
				};

			} m_for3DOrCube;

			// 构造函数
			RTVDSVCreation()
			{
				memset( this, 0, sizeof(RTVDSVCreation));
			}
		};
		typedef vector<pair<RTVDSVCreation, ID3D11RenderTargetViewPtr>> RenderTargetViews;
		//typedef vector<pair<RTVDSVCreation, ID3D11DepthStencilViewPtr>> DetpthStencilViews;

		typedef vector<pair<D3D11_SHADER_RESOURCE_VIEW_DESC, ID3D11ShaderResourceViewPtr>> D3DSRViews;
		typedef vector<pair<D3D11_DEPTH_STENCIL_VIEW_DESC,   ID3D11DepthStencilViewPtr>>   D3DDSViews;

	protected:
		ID3D11DevicePtr			m_d3dDevice;	
		ID3D11DeviceContextPtr	m_d3dImmCtx;

		RenderTargetViews		m_d3dRtViews;
		D3DSRViews				m_d3dSRViews;
		D3DDSViews				m_d3dDsViews;
	};
	typedef boost::shared_ptr<D3D11Texture> D3D11TexturePtr;

	//---------------------------------------
	// D3D11Texture2D 2012-3-3  帝林
	//---------------------------------------
	class D3D11Texture2D : public D3D11Texture
	{
	public:
		D3D11Texture2D( ID3D11Texture2DPtr d3d11Tex, UINT sampleCount, UINT smapleQuality, UINT accessHint);
		D3D11Texture2D( UINT width, UINT height, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT smapleQuality, UINT accessHint, const ElementInitData* initData);
	
		// returns the width of the texture
		virtual UINT GetWidth( UINT levle) const;

		// returns the height of the texture
		virtual UINT GetHeight( UINT levle) const;

		// Copies (and maybe scales to fit) the contents of this texture to another texture
		virtual void CopyToTexure( Texture& target);

		// copy texture
		virtual void CopyToSubTexture2D( Texture& target,UINT dstArrayIdx, UINT dstLevel, UINT dstXOffset, UINT dstYOffset, UINT dstWidth, UINT dstHeight, UINT srcArrayIdx, UINT srcLevel, UINT srcXOffset, UINT srcYOffset, UINT srcWidth, UINT srcHeight);

		// 获取D3D资源
		ID3D11ResourcePtr GetD3DResource() const { return m_d3d11Texture2D; }

		// 获取D3D纹理
		ID3D11Texture2DPtr GetD3DTexture() const { return m_d3d11Texture2D; }

		// 检索ShaderResourceView
		virtual const ID3D11ShaderResourceViewPtr& RetriveD3DShaderResourceView( UINT mostDetailedMip=0, UINT mipLevels=1, UINT firstArrayIndex=0, UINT arraySize=INVALID);

		// 检索RenderTargetView
		virtual const ID3D11RenderTargetViewPtr& RetriveD3DRenderTargetView( UINT firstArrayIdx, UINT arraySize, UINT level);

		// 检索DepthStencilView
		virtual const ID3D11DepthStencilViewPtr& RetriveD3DDepthStencilView( UINT firstArrayIndex, UINT arraySize, UINT level);

		// 生成Mipmaps
		virtual void GenerateMips();

	private:
		// Map2D texture
		virtual void Map2D( UINT arrayIndex, UINT level, TextureMapAccess access, UINT xOffset, UINT yOffset, UINT width, UINT height, void*&data, UINT& rowPitch);

		// Unmaap2D
		virtual void Unmap2D( UINT arrayIndex, UINT level);

		// 更新参数
		void UpdateParams();

	private:
		D3D11_TEXTURE2D_DESC	m_desc;
		ID3D11Texture2DPtr	    m_d3d11Texture2D;

		vector<UINT>			m_widthes;
		vector<UINT>			m_heights;
	};

	//---------------------------------------
	// D3D11Texture3D 2012-5-10  帝林
	//---------------------------------------
	class D3D11Texture3D : public D3D11Texture
	{

	};
}