#include <Engine/FxSystem/AresFxObject.h>
#include <Engine/FxSystem/AresFxEntity.h>

namespace Ares
{
	// 获取世界转换
	Transform FxObject::GetWorldTransform()
	{
		Transform result;
		if( m_entity)
			result = m_entity->GetTransform();

		return result * (*GetTransform());
	}
}