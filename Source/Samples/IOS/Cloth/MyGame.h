#ifndef AGFRAMEWORK_H
#define AGFRAMEWORK_H

#include "AresGame.h"
#include "CameraLogic.h"
#include <Core/Graphics/AresRenderSettings.h>
#include <Engine/SceneSystem/AresSceneSystem.h>

using namespace Ares;

//-------------------------------------------
// Aresengine Game Framework 2013-7-15 帝林
//-------------------------------------------
class MyGame : public AresGame
{
public:
    MyGame( RenderSettings& tSettings);
    ~MyGame();
    
    // 更新
    void FrameMove( float elaspedTime);
    
    // 渲染
    void Render();
    
private:
    CameraLogic*    m_cameraLogic;
    SceneSystem*    m_sceneSystem;
};

#endif
