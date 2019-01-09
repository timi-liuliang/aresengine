#include "Application.h"
#include <Core/AresCore.h>
#include <Engine/AresRoot.h>

Application* G_Application=NULL;

// 构造函数
Application::Application()
{
	G_Application = this;

	Init();

	m_scLogic = NULL;
	m_moLogic = NULL;
}

// 析构函数
Application::~Application()
{
	SAFE_DELETE( m_moLogic);
	SAFE_DELETE( m_scLogic);
}

// 初始化
bool Application::Init()
{
	// 新建Root
	ResPaths paths;
	paths.push_back("data\\shader\\");
	paths.push_back("data\\shader\\official\\");
	paths.push_back("data\\texture\\");
	paths.push_back("data\\model\\");
	paths.push_back("data\\model\\scene");
	paths.push_back("data\\fx\\texture\\");
	new_ Root( D3D11, paths);

	return true;
}

// 初始化场景编辑器逻辑
void Application::InitSCLogic( HWND hwnd)
{
	m_scLogic = new_ SCLogic( hwnd);
}

// 初始化模型编辑器逻辑
void Application::InitMOLogic( HWND hwnd)
{
	m_moLogic = new_ MOLogic( hwnd);
}

// 程序开始
void Application::Exec()
{
	static DWORD lastTime   = timeGetTime();

	// 计算delta Time
	DWORD curTime     = timeGetTime();
	DWORD elapsedTime = curTime - lastTime;

	// logic update
	FrameMove( elapsedTime * 0.001f);

	// Call the main render function
	Render( elapsedTime * 0.001f);

	lastTime          = curTime;
}

// 每帧更新
void Application::FrameMove(float fTime)
{
	if( m_scLogic && !m_scLogic->IsPaused())
		m_scLogic->FrameMove( fTime);

	if( m_moLogic && !m_moLogic->IsPaused())
		m_moLogic->FrameMove( fTime);

	// 场景
	ARoot->FrameMove( fTime);
}

// 应用程序渲染
void Application::Render(float fTime)
{
	// 渲染场景
	ARoot->Render();
}