#ifndef ARES_GAME_H
#define ARES_GAME_H

//-------------------------------------------
// Aresengine Game Framework 2013-7-15 帝林
//-------------------------------------------
class AresGame
{
public:
    AresGame() {}
    virtual ~AresGame() {}
    
    // 更新
    virtual void FrameMove( float elaspedTime)=0;
    
    // 渲染
    virtual void Render()=0;
};

// 新建项目
AresGame* NewGame( void* layer, int width, int height);

#endif