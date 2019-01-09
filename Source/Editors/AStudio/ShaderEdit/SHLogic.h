#pragma once

#include <vector>
#include <string>
#include <map>
#include "ShaderTreeEdit.h"

using namespace std;

namespace Ares
{
	//---------------------------------------
	// 编辑器逻辑入口	2010-07-20 帝林	
	//---------------------------------------
	class SHLogic
	{
	public:
		// 构造函数
		SHLogic();

		// 更新
		void FrameMove( float fTime);

		// 获取帧率
		float GetFPS()  { return m_fFPS; }

		// 渲染
		void Render();
		
		// 更新帧率
		bool UpdateFPS( float fTime);

		// 设置添充方式
		void SetFillMode(WORD index);

		// 设置实体
		void SetModelEntity( const char* resPath);

		// 为当前材质更换模型
		void SetModel( const char* resPath);

		// 应用当前激活shader
		void ApplyShader();

		// 获取激活材质
		//RenderMaterial& GetActiveMaterial();

		// 激活shader
		void SetActiveShader( const char* shaderName, const char* modelName);

		// 保存当前激活shader的显示配置
		void SaveActiveShader();

	public:
		// 获取材质编辑器
		ShaderTreeEdit* GetShaderTreeEdit() { return m_shaderTreeEdit; }

	private:
		HWND					m_hWnd;
		float					m_fFPS;					// frames per second

		// 鼠标位置点
		POINT					m_lastPoint;
		bool					m_mouseLeftButtonDown;	// 左键是否按下
		//RenderMaterial			m_material;

		string					m_shaderName;			// shader名(当前激活)
		ShaderTreeEdit*			m_shaderTreeEdit;	// 超级材质编辑
	};

	extern  SHLogic*				g_pSHLogic;
	#define SHLogic_Main			g_pSHLogic
	#define SHLogic_Input			SHLogic_Main->GetInputListener()
	#define SHLogic_ShaderTreeEdit	SHLogic_Main->GetShaderTreeEdit()
}