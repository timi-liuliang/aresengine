#pragma once

#include "iskincodes.h"
#include "Phyexp.h"
#include "iparamb2.h"
#include "iskin.h"
#include <d3dx9math.h>
#include <string>
#include <IGame.h>
#include <Core/AresMath.h>
#include <Engine/RenderSystem/M1/M1Loader.h>

using namespace std;

namespace M1_IGame
{
	// 从材质的Diffuse slot中获取数据
	void  GetIndicesFromTexMap( int& i1, int& i2, Mtl* _mtl);

	// 从全局路径中拆分出纹理名,可更改后缀名
	string SplitNameExt( const char* fullPath, const char* aimExt);

	// 查找PhysiqueModifier
	Modifier*  FindPhysiqueModifier( INode* pNode);

	// 查找MaxSkinModifier
	Modifier* FindMaxSkinModifier( INode* nodePtr);

	// 更换坐标系
	D3DXMATRIX MAXtoARES( Matrix3& mat);

	//// 文件拷贝
	//bool CopyFile(char* source, char* target);

	// 辅助函数
	void ChangeExt( char* dst, const char* source, const char* ext);

	// 辅助函数
	void NameExt( char* dst, const char* source);

	// 转到到Ares空间
	Ares::Transform GMatrixToAresTransform( const GMatrix& mat);

	// 转换
	void Point3ToSM1Vertex( Ares::SM1Vertex& oV, const Point3& iV);
}