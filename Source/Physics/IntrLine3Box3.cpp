#include <Physics/IntrLine3Box3.h>

namespace Ares
{
	// 构造函数 
	IntrLine3Box3::IntrLine3Box3( const Line3& line, const Box3& box)
		: m_line( line)
		, m_box( box)
	{}

	// 相交测试
	bool IntrLine3Box3::Test()
	{
		float AWdU[3], AWxDdU[3], RHS;

		Vector3 diff = m_line.m_orig - m_box.m_center;
		Vector3 WxD  = Vector3Cross( m_line.m_dir, diff);

		AWdU[1]   = std::abs( Vector3Dot( m_line.m_dir, m_box.m_axis[1])); 
		AWdU[2]   = std::abs( Vector3Dot( m_line.m_dir, m_box.m_axis[2])); 
		AWxDdU[0] = std::abs( Vector3Dot( WxD, m_box.m_axis[0])); 
		RHS = m_box.m_extent[1]*AWdU[2] + m_box.m_extent[2]*AWdU[1];
		if (AWxDdU[0] > RHS)
			return false;

		AWdU[0]   = std::abs( Vector3Dot( m_line.m_dir, m_box.m_axis[0]));
		AWxDdU[1] = std::abs( Vector3Dot( WxD, m_box.m_axis[1]));
		RHS = m_box.m_extent[0]*AWdU[2] + m_box.m_extent[2]*AWdU[0];
		if (AWxDdU[1] > RHS)
			return false;

		AWxDdU[2] = std::abs( Vector3Dot( WxD, m_box.m_axis[2]));
		RHS = m_box.m_extent[0]*AWdU[1] + m_box.m_extent[1]*AWdU[0];
		if (AWxDdU[2] > RHS)
			return false;

		return true;
	}
}