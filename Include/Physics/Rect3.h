#pragma  once

#include <Core/AresMath.h>
#include "Physics/Shape.h"

namespace Ares
{
	//-------------------------------------------------------------
	// 2D区域 
	//-------------------------------------------------------------
	class Rect3;
	class Rect2
	{
	public:
		union
		{
			struct 
			{
				float x0, x1;
				float y0, y1;
			};
		};

		// 构造函数
		Rect2();
		Rect2( float _x0, float _x1, float _y0, float _y1);

		// 重载等号  "=" 
		void operator = (const Rect2& src);

		// 重载符号  "="
		void operator = (const Rect3& src);

		// 返回XSize
		float GetXSize() { return std::abs(x1 - x0); }

		// 返回YSize
		float GetYSize() { return std::abs(y1 - y0); }

		// 顶点是否在2D区域内
		bool IsPointIn( float x, float y);

		// 合并点
		void  UnionPoint( float x, float y);

		// 重置
		void Reset();
	};

	//-------------------------------------------------------------
	// 3D区域 AABB 2010-03-28 17:08   -- 未知   帝林
	//-------------------------------------------------------------
	class Rect3 : public Shape
	{
	public:
		static const Rect3 Infinity;			// 无穷大

	public:
		Vector3 m_min;
		Vector3	m_max;

		// 构造函数
		Rect3();
		Rect3(const Rect3& src);
		Rect3(const Vector3& v0, const Vector3& v1);

		// 重载等号  "="
		void operator = (const Rect2& src);

		// 重载等号  "="
		void operator = (const Rect3& src);

		// 重载符号  "+="
		const Rect3& operator += (const Vector3& vec);

		// 重载符号  "*="
		const Rect3& operator *= (const Vector3& vec);

		// 重载运算符 "!="
		UBOOL operator != ( const Rect3& r) const
		{
			return m_min!=r.m_min || m_max!=r.m_max;
		}

		// 重载运算符 "=="
		UBOOL operator == ( const Rect3& r) const
		{
			return m_min==r.m_min && m_max==r.m_max;
		}

		// 设置
		void Set( float _x0, float _x1, float _y0, float _y1, float _z0, float _z1);

		// 返回3D区域尺寸
		Vector3 GetSize();

		// 返回XSize
		float GetXSize() const { return std::abs(m_max.x - m_min.x); }

		// 返回YSize
		float GetYSize() const { return std::abs(m_max.y - m_min.y); }

		// 返回ZSize
		float GetZSize() const { return std::abs(m_max.z - m_min.z); }

		// 返回表面积
		float GetSurfaceArea() const;

		// 返回小位置
		Vector3& GetMinPoint() { return m_min; }

		// 返回大坐标
		Vector3& GetMaxPoint() { return m_max; }

		// 返回小位置
		const Vector3& GetMinPoint() const { return m_min; }

		// 返回大坐标
		const Vector3& GetMaxPoint() const { return m_max; }

		// 获取八个点
		void GetEightPoints( vector<Vector3>& points) const;

		// 顶点是否在2D区域内
		virtual bool IsPointIn( const Vector3& p) const;

		// AABB是否在区域内
		bool IsContain( const Rect3& aabb) const;

		// 合并X
		void UnionX( float x);

		// 合并Y
		void UnionY( float y);

		// 合并Z
		void UnionZ( float z);

		// 合并点
		void  UnionPoint(const Vector3& point);

		// 合并点
		void  UnionPoint( float x, float y, float z);

		// 构建AABB包围盒
		virtual void BuildAABB( Rect3& box, const Transform& trans) const;

		// 计算运动物体惯性
		virtual Vector3 CalculateLocalInertia( float mass) { A_ASSERT(false); return Vector3::Zero; }

		// 转换
		void  TransformByMatrix( const Matrix44& matrix);

		// 重置
		void Reset();

	public:
		// 合并,静态方法
		static void Merge( Rect3& result, const Rect3& inA, const Rect3& inB);

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<Shape>(*this);
			ar & m_min;
			ar & m_max;
		}
	};
}