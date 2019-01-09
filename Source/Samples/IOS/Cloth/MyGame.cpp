	#include "MyGame.h"
#include <Engine/AresRoot.h>
#include <Physics/Spiral3.h>
#include <Engine/RenderSystem/AresVisualShape.h>

using namespace Ares;

// 新建项目
AresGame* NewGame(void* layer, int width, int height)
{
    RenderSettings tSettings(layer);
    tSettings.m_width = width;
    tSettings.m_height= height;
    
	return new_ MyGame( tSettings);
}

// 构造函数
MyGame::MyGame( RenderSettings& tSettings)
	: AresGame()
{
    ResPaths paths;
    paths.push_back( "Resources\\data\\shader\\");
    paths.push_back( "Resources\\data\\shader\\official\\");
    paths.push_back( "Resources\\data\\texture\\");
    
    m_cameraLogic = new_ CameraLogic();
    m_cameraLogic->SetProjParams( PI/4.f, tSettings.m_width,tSettings.m_height, 1.f, 150.0f, tSettings.m_api);
    
    new_ Root( paths);
    if( m_sceneSystem = ARoot->BuildSceneSystem( m_cameraLogic, tSettings))
    {
        // 螺旋曲线
        int   numSplit = 10000;
        float radius = 0.f;
        float angle  = 0.f;
        Vector3 startPoint = Vector3::Zero;
        Vector3 endPoint( 0.f, 0.f, 0.f);
        Vector3 lineDir = endPoint - startPoint;
        Vector3 splitLen = (lineDir)/numSplit;
        vector<Vector3> lineList( numSplit);
        for( int i=0; i<numSplit; i++)
        {
        	radius += 10.f / float(numSplit);
        	angle  += 2*PI/200.f;
        
        	Spiral3::CalculatePos( lineList[i], startPoint+splitLen*i, lineDir, radius, angle);
        }
        
        VisualShape* spiral = m_sceneSystem->GetRenderSystem().GetVisualShapeMgr().CreateVisualShape();
        spiral->Set( RenderLayout::TT_LineStrip, lineList.data(), sizeof(Vector3)*lineList.size(), NULL, 0);
        spiral->SetColor( ColorRGB::Blue);
        spiral->SetVisible( true);
        
        Rect3 ScreenSpace;
        Matrix44 viewProj = m_sceneSystem->GetCameraSystem().GetViewProj();
        for( size_t i=0; i<lineList.size(); i++)
        {
            Vector4 result = viewProj.TransformVector4( Vector4( lineList[i].x, lineList[i].y, lineList[i].z, 1.f));
            result /= result.w;
            
            Vector3  v3 = result;
            ScreenSpace.UnionPoint( v3);
        }
        
        
        //VisualCapsule* capsule = m_sceneSystem->GetRenderSystem().GetVisualShapeMgr().CreateCapsule();
        //Capsule3 tCapsule( Segment3( -Vector3::XAxis, Vector3::XAxis), 1.f);
        //capsule->SetShape( tCapsule);
        //capsule->SetColor( ColorRGB::White);
        //capsule->SetVisible( true);
        
        int a = 10;
    }
}

// 析构函数
MyGame::~MyGame()
{
}

// 更新
void MyGame::FrameMove( float elaspedTime)
{
    ARoot->FrameMove( elaspedTime);
}

// 渲染
void MyGame::Render()
{
    ARoot->Render();
}
