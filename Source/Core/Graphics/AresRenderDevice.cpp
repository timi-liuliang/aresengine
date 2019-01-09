#include "Core/Graphics/AresRenderDevice.h"

namespace Ares
{
	// 构造函数
	RenderDevice::RenderDevice( RenderDeviceType api)
		: m_logSystem( NULL)
		, m_api( api)
	{

	}

	// 析构函数
	RenderDevice::~RenderDevice()
	{

	}

	// 设置日志系统
	void RenderDevice::SetLogSystem( LogSystem* logSystem) 
	{ 
		m_logSystem = logSystem; 
	}

	// 获取日志系统
	LogSystem& RenderDevice::GetLogSystem() 
	{ 
		A_ASSERT( m_logSystem);

		return *m_logSystem; 
	}

	// BindFrameBuffer
	void RenderDevice::BindFrameBuffer( FrameBufferPtr frameBuffer)
	{
		if( frameBuffer)
		{
			if( frameBuffer->IsDirty() || m_curFrameBuffer!=frameBuffer)
			{
				if( m_curFrameBuffer)
					m_curFrameBuffer->Unbind();

				m_curFrameBuffer = frameBuffer;
				m_curFrameBuffer->Bind();
			}
		}
	}

	// 解绑当前FrameBuffer
	void RenderDevice::ResetFrameBuffer()
	{
		if( m_curFrameBuffer)
		{
			m_curFrameBuffer->Unbind();
			m_curFrameBuffer = FrameBufferPtr();
		}
	}

	// 创建渲染窗口
	RenderWindow* RenderDevice::CreateRenderWindow( const RenderSettings& settings)
	{
		//m_renderSettings = settings;

		return NULL;
	}

	// 创建效果文件
	//RenderEffectPtr RenderFactory::CreateEffect( const char* fileName)
	//{
		//EffectPool::iterator it = m_effectPool.find( fileName);
		//if( it==m_effectPool.end())
		//{
		//	RenderEffectPtr effect = RenderEffectPtr( new RenderEffect());
		//	if( effect->Load( fileName))
		//		m_effectPool[fileName] = effect;

		//	return effect;
		//}

		//return it->second;
	//}

	// 获取当前FrameBuffer
	FrameBufferPtr RenderDevice::GetCurFrameBuffer() const
	{
		return m_curFrameBuffer;
	}

	// 获取设备能力
	const RenderDeviceCaps& RenderDevice::GetDeviceCaps() const
	{
		return m_deviceCaps;
	}

	//// 获取渲染设置
	//const RenderSettings& RenderDevice::GetRenderSettings() const
	//{
	//	return m_renderSettings;
	//}

	//// 应用设置
	//void RenderDevice::SetRenderSettings( const RenderSettings& settings)
	//{
	//	// 窗口大小改变
	//	if( settings.m_width!=m_renderSettings.m_width || settings.m_height!=m_renderSettings.m_height)
	//	{
	//		GetRenderWindow()->Resize( settings.m_width, settings.m_height);
	//	}

	//	m_renderSettings = settings;
	//}
}