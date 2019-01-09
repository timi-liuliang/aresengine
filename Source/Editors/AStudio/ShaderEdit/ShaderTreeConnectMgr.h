#pragma once

#include "ShaderTreeControl.h"
#include <Engine/RenderSystem/SuperMaterialShaderTree.h>

using namespace ST;

namespace Ares
{
	//-------------------------------
	// 链接管理器 2013-3-22 帝林
	//-------------------------------
	struct ShaderTreeConnectMgr
	{
		vector<ST::QBezierCurve*>	m_curves;
		ST::QBezierCurve*			m_curveEdit;

	public:
		// 构造函数
		ShaderTreeConnectMgr() : m_curveEdit( NULL) {}

		// 新建链接
		void CreateConnect();

		// 完成链接
		void FinishConnect( QExpressionInputOutput* end);

		// 删除链接
		void DeleteEditConnect();

		// 获取编辑中链接
		QBezierCurve* GetEditConnect() { return m_curveEdit; }

		// 获取链接
		QBezierCurve* EditConnect( ST::QExpressionInputOutput* end);

		// 当输入输出移动
		void OnMove( QExpressionInputOutput* ioput);

		// 重置
		void Reset();

		// 添加链接
		void AddConnection( ST::QBezierCurve* curve);

		// 移除无用链接
		void RemoveUselessConnection( MaterialShaderTree* shaderTree);

		// 获取链接
		ST::QBezierCurve* GetConnection( const string& expression, const string& input);
	};
}