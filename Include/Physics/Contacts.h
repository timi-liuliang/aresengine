#pragma once

#include <Core/AresMath.h>

namespace Ares
{
	//----------------------------------------
	// 碰撞接触 2012-11-19	帝林 
	//----------------------------------------
	class CollisionObject;
	struct Contact
	{
		Vector3				m_point;			// the position of the contact in world coordinates
		Vector3				m_normal;			// the direction of the contact in world coordinates
		float				m_depth;			// the depth of penetration at the contact point

		CollisionObject*	m_body0;			// 碰撞对象0
		CollisionObject*	m_body1;			// 碰撞对象1

		Contact() 
			: m_body0( NULL) , m_body1( NULL)
		{}
	};

	//------------------------------------
	// 碰撞结果	2012-11-19 帝林
	//------------------------------------
	class ContactResult
	{
	public:		
		// 运算符重载"[]" const
		Contact& operator[]( INT i)
		{
			return m_contacts[i];
		}

		// 添加
		void AddContact( CollisionObject* objectA, CollisionObject* objectB, const Vector3& normal, const Vector3& point0, const Vector3& point1)
		{
			Contact contact;
			contact.m_body0  = objectA;
			contact.m_body1	 = objectB;
			contact.m_normal = normal;
			contact.m_point  = 0.5f * ( point0+point1);
			contact.m_depth  = 0.5f * ( point0-point1).Length();

			m_contacts.push_back( contact);
		}

		// 获取大小
		size_t GetSize() { return m_contacts.size(); }

	private:
		std::vector<Contact> m_contacts;		// 待优化
	};
}