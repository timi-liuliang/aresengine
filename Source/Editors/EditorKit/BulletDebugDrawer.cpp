#include "BulletDebugDrawer.h"

namespace Ares
{
	// 构造函数
	BulletDebugDrawer::BulletDebugDrawer( Scene* scene, btDynamicsWorld* dynamicsWorld)
		: m_numActive( 0)
	{
		m_scene		   = scene;
		m_dynamicsWorld= dynamicsWorld;
	}

	// 析构函数
	BulletDebugDrawer::~BulletDebugDrawer()
	{

	}

	// 更新
	void BulletDebugDrawer::FrameMove( float timeStep)
	{
		if( !m_dynamicsWorld)
			return;

		// 使所有几何体不可见
		m_numActive = 0;
		foreach_( VisualShape* shape, m_visualShapes)
			shape->SetVisible( false);

		Matrix44	m;
		btMatrix3x3 rot; rot.setIdentity();
		int	numObjects = m_dynamicsWorld->getNumCollisionObjects();
		btVector3	wireColor( 1.f, 0.f, 0.f);
		for( int i=0; i<numObjects; i++)
		{
			btCollisionObject* colObj = m_dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody*	   body		  = btRigidBody::upcast( colObj);
			if( body && body->getMotionState())
			{
				btDefaultMotionState* motionState = (btDefaultMotionState*)body->getMotionState();
				motionState->m_graphicsWorldTrans.getOpenGLMatrix( (btScalar*)(&m));
				rot = motionState->m_graphicsWorldTrans.getBasis();
			}
			else
			{
				colObj->getWorldTransform().getOpenGLMatrix( (btScalar*)(&m));
				rot = colObj->getWorldTransform().getBasis();
			}

			// 颜色
			btVector3 wireColor( 1.f, 1.f, 0.5f);
			if( i&1) wireColor=btVector3(0.f,0.0f,1.f);
			if( colObj->getActivationState()==1)
			{
				if (i & 1)
				{
					wireColor += btVector3 (1.f,0.f,0.f);
				}
				else
				{			
					wireColor += btVector3 (.5f,0.f,0.f);
				}
			}
			if( colObj->getActivationState()==2) //ISLAND_SLEEPING
			{
				if(i&1)
				{
					wireColor += btVector3 (0.f,1.f, 0.f);
				}
				else
				{
					wireColor += btVector3 (0.f,0.5f,0.f);
				}
			}

			// 轴向包围盒
			btVector3 aabbMin,aabbMax;
			m_dynamicsWorld->getBroadphase()->getBroadphaseAabb(aabbMin,aabbMax);

			aabbMin-=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
			aabbMax+=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);

			Draw( m, colObj->getCollisionShape(), wireColor, aabbMin, aabbMax);
		}
	}

	// 获取VisualShape
	VisualShape* BulletDebugDrawer::GetVisualShape()
	{
		if( (int)m_visualShapes.size()<=m_numActive)
		{
			VisualShape* visualShape = m_scene->GetRenderSystem().GetVisualShapeMgr().CreateVisualShape();
			m_visualShapes.push_back( visualShape);
		}

		m_numActive++;
		return m_visualShapes[m_numActive-1];
	}

	// 渲染
	void BulletDebugDrawer::Draw( Matrix44& m, const btCollisionShape* shape, const btVector3& color, const btVector3& worldBoundsMin, const btVector3& worldBoundsMax)
	{	
		if( shape)
		{
			VisualShape* visualShape = GetVisualShape();
			if( visualShape)
			{
				visualShape->SetVisible( true);

				int shapeType = shape->getShapeType();
				switch( shapeType)
				{
					//case ST_Capsule:
					//	{
					//		Capsule3* capsule  = dynamic_cast<Capsule3*>( shape);
					//		Capsule3  capsuleT = *capsule;
					//		capsuleT.DoTransform( collObject->GetTransform());

					//		VisualShape*   visualShape = GetVisualShape( shape);
					//		VisualCapsule* visualCapsule = dynamic_cast<VisualCapsule*>( visualShape);
					//		if( visualCapsule)
					//			visualCapsule->SetShape( capsuleT);
					//	}
					//	break;

					//case ST_Rect3:
					//	{
					//		int a = 10;
					//	}
					//	break;

				case BOX_SHAPE_PROXYTYPE:
					{
						const btBoxShape* boxShape = static_cast<const btBoxShape*>( shape);
						btVector3 halfExtent	   = boxShape->getHalfExtentsWithMargin();

						const int numIndices = 36;
						const int numTris	 = 12;

						// 索引数据
						int indices[numIndices] = 
						{	
							0,1,2,		// ok
							3,2,1,		// ok
							10,8,11,	// ok
							11,8,9,		// ok
							15,13,14,	// ok
							14,13,12,	// ok
							19,17,16,	// ok		
							19,16,18,	// ok
							5,4,7,		// ok
							7,4,6,		// ok
							23,20,21,	// 
							23,22,20	//
						};

						// 顶点数据
						Vector3 positions[24] = 
						{	
							Vector3(halfExtent[0],halfExtent[1],halfExtent[2]),
							Vector3(-halfExtent[0],halfExtent[1],halfExtent[2]),
							Vector3(halfExtent[0],-halfExtent[1],halfExtent[2]),	
							Vector3(-halfExtent[0],-halfExtent[1],halfExtent[2]),	

							Vector3(halfExtent[0],halfExtent[1],-halfExtent[2]),
							Vector3(-halfExtent[0],halfExtent[1],-halfExtent[2]),	
							Vector3(halfExtent[0],-halfExtent[1],-halfExtent[2]),	
							Vector3(-halfExtent[0],-halfExtent[1],-halfExtent[2]),

							Vector3(halfExtent[0],halfExtent[1],halfExtent[2]),
							Vector3(halfExtent[0],-halfExtent[1],halfExtent[2]),
							Vector3(halfExtent[0],halfExtent[1],-halfExtent[2]),
							Vector3(halfExtent[0],-halfExtent[1],-halfExtent[2]),

							Vector3(halfExtent[0],halfExtent[1],halfExtent[2]),
							Vector3(-halfExtent[0],halfExtent[1],halfExtent[2]),
							Vector3(halfExtent[0],halfExtent[1],-halfExtent[2]),
							Vector3(-halfExtent[0],halfExtent[1],-halfExtent[2]),

							Vector3(-halfExtent[0],halfExtent[1],halfExtent[2]),
							Vector3(-halfExtent[0],-halfExtent[1],halfExtent[2]),
							Vector3(-halfExtent[0],halfExtent[1],-halfExtent[2]),
							Vector3(-halfExtent[0],-halfExtent[1],-halfExtent[2]),

							Vector3(halfExtent[0],-halfExtent[1],halfExtent[2]),	
							Vector3(-halfExtent[0],-halfExtent[1],halfExtent[2]),
							Vector3(halfExtent[0],-halfExtent[1],-halfExtent[2]),	
							Vector3(-halfExtent[0],-halfExtent[1],-halfExtent[2])
						};

						// 法线数据
						vector<Vector3>	normals( 24);
						TBNComputeNormal( normals.data(), positions, 24, indices, numTris);

						visualShape->Set( RenderLayout::TT_TriangleList, positions, sizeof(Vector3)*24, indices, sizeof(int)*numIndices, EF_R32UI, normals.data());
						visualShape->SetColor( ColorRGB( color.x(), color.y(), color.z()));
						visualShape->GetTransform()->SetMatrix( m);
					}
					break;
				}

			}	
		}
	}
}