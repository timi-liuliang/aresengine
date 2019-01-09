#pragma once

#include <WTypes.h>
#include <Core/Graphics/AresRenderWindow.h>
#include "SCLogic.h"
#include "ModelEdit/MOLogic.h"

using namespace Ares;

//--------------------------------------------
// DL应用程序  2010-05-07 18:23    帝林
//--------------------------------------------
class Application
{
public:
	Application();
	~Application();

	// 初始化
	virtual bool Init();

	// 初始化场景编辑器逻辑
	void InitSCLogic( HWND hwnd);

	// 初始化模型编辑器逻辑
	void InitMOLogic( HWND hwnd);

	// 程序开始
	void  Exec();

	// 每帧更新
	virtual void FrameMove(float fTime);

	// 渲染
	virtual void Render(float fTime);

private:
	SCLogic*		m_scLogic;
	MOLogic*		m_moLogic;
};

extern Application* G_Application;