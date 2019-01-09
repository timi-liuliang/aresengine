#pragma once

#include <Core/AresMath.h>
#include <Physics/Shapes.h>

namespace Ares
{
	// 3D矩形与平面 空间位置检测 EM_PLANE_FRONT = 0, EM_PLANE_BACK = 1, EM_PLANE_INTERSECT = 2,
	int PlaneClassify( const Rect3& rect, const Plane3& plane);

	// 射线与三角形碰撞
	bool Intersect( const Line3& line, const Triangle3& triangle, float& fDist);

	// 通过鼠标点击窗口获取Pick射线
	bool WindowPointToRay(	Vector3& rayOrig, Vector3& rayDir, LONG x, LONG y, UINT d3dBackBufferWidth, UINT d3dBackBufferHeight, const Matrix44& matView, const Matrix44& matProj);

	// global function
	bool WindowPointToRay( Vector3& rayOrig, Vector3& rayDir, LONG x, LONG y, UINT d3dBackBufferWidth, UINT d3dBackBufferHeight, const Matrix44& matView, const Matrix44& matProj, const Matrix44& matOffset);

	// 屏幕点转换到3D空间
	void ScreenPointTo3DSpace( Vector3& result, LONG x, LONG y, LONG screenWidth, LONG screenHeight, const Matrix44& transform);
}