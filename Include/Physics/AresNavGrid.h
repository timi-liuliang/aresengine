#pragma once

#include "AresNavMap.h"

namespace Ares
{
	//--------------------------------------
	// 导航网格 2011-01-20  帝林
	//--------------------------------------
	class CNavGrid : public INavMap
	{
	protected:
		// 构造函数
		CNavGrid(int iWidth, int iHeight);

		// 析构函数
		~CNavGrid();

		// 获取可通行周围接点
		void GetPassAbleNeighbor( int* childID, int& numChild,int fatherID);

		//// 根据ID计算两格间h
		//virtual float CalcH( int idFrom, int idTo, ARESVECTOR3& vEnd);

		//// 根据ID计算两格间g
		//virtual float CalcG( ARESVECTOR3& vStart, int idFrom, int idTo, int idFromFrom=-1);

	public:
		// 根据坐标获取ID
		int  Coord2ID( int row, int col) { return row*m_iWidth + col; }

		// 设置可否通行
		bool SetPassAble( int row, int col, bool bPass);

		// 设置是否通行
		bool SetPassAble( int ID, bool bPass);

		// 是否可通行
		bool IsPassAble( int ID);

		// 保存
		void  Save();

		// 加载
		void  Load();

	private:
		int    m_iWidth;		// 宽图
		int    m_iHeight;		// 高度
		bool*  m_pPassAble;  	// 通行列表
	};
}