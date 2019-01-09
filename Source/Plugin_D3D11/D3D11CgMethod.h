//#pragma once
//
//#include <Core/Graphics/AresRenderMethod.h>
//#include "D3D11Typedefs.h"
//#include <Cg/cgD3D11.h>
//
//namespace Ares
//{
//	//--------------------------------------
//	// D3D11RenderEffect 2012-4-6 帝林
//	// CGeffect Encapsulation
//	//--------------------------------------
//	struct D3D11CgMethod : public RenderMethod
//	{
//	public:
//		// 初始化
//		static bool InitCg( ID3D11DevicePtr& d3d11Device);
//
//		// 获取CGcontext
//		static CGcontext GetContext() { return m_cgContext; } 
//
//	public:
//		// 构造函数
//		D3D11CgMethod();
//
//		// 析构函数
//		~D3D11CgMethod();
//
//		// 加载函数 *.cgfx
//		virtual bool Load(  const char* fileName);
//
//		// 从内存加载
//		virtual bool LoadFromMemory( void* code, size_t length);
//
//		// 获取IASignature
//		ID3D10Blob* GetIASignature() { return cgD3D11GetIASignatureByPass( m_cgPass); }
//
//		// 应用参数
//		virtual void SetParams(const RenderMaterial& mat);
//
//	private:
//		// 剖析参数
//		void ParseParameters();
//
//		// 剖析状态
//		void ParseStates();
//
//		// 检测错误
//		void CheckForCgError( string* error=NULL);
//
//	private:
//		static CGcontext	m_cgContext;			// 全局唯一
//
//		CGeffect			m_cgEffect;				// CgEffect
//		CGtechnique			m_cgTechnique;
//		CGpass				m_cgPass;
//		vector<CGparameter> m_cgParameters;			// CgParameters
//	};
//}