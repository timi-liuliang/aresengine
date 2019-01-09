#include <Physics/Spiral3.h>

namespace Ares
{
	// 计算当前位置
	void Spiral3::CalculatePos( Vector3& oResult, const Vector3& centerPos, const Vector3& centerDir, float radius, float angle)
	{
		Quat rotation;
		rotation.FromArc( Vector3::ZAxis, centerDir);

		Vector3 radiusDir( std::sin( angle) * radius, std::cos( angle) * radius, 0.f);
		radiusDir = rotation.Rotate( radiusDir);

		oResult = centerPos + radiusDir;
	}
}