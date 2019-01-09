//#include <Engine/SceneSystem/SceneNode.h>
//#include <Engine/SceneSystem/AresEntity.h>
//#include <Physics/Shapes/Algorithm/Intersect.h>
//
//namespace Ares
//{
//	// 构造函数
//	SceneNode::SceneNode()
//	{
//		// 矩阵单位化
//		m_matWorld.SetIdentity();
//		m_matRotation.SetIdentity();
//
//		m_iEntityNum   = 0;
//		m_pEntityFirst = NULL;
//		m_pEntityLast  = NULL;
//
//		m_pSibling = NULL;
//		m_pChild   = NULL;
//
//		m_inheritance        = EM_INHERITANCE_ALL;
//
//		m_objectFlags        = 0;
//	}
//
//	// 析构函数
//	SceneNode::~SceneNode()
//	{
//		const s_ptr<SceneNode>& tParent = GetParent();
//		if( tParent)
//			tParent->DetachChild( this);
//
//		// 去除与四叉树绑定
//		//DetachFromQuadTree();
//	}
//
//	// 创建子结点
//	s_ptr<SceneNode> SceneNode::CreateChildSceneNode()
//	{
//		s_ptr<SceneNode> pSceneNode = s_ptr<SceneNode>( new_ SceneNode);
//
//		this->AttachChild( pSceneNode.get());
//
//		return pSceneNode;
//	}
//
//	// 添加兄弟结点
//	void SceneNode::AddSibling( SceneNode* pSceneNode)
//	{
//		if( pSceneNode)
//		{
//			SceneNode* tNode = (SceneNode*)pSceneNode;
//
//			SceneNode* pLastSibling = tNode;
//			SceneNode* pSibling     = (SceneNode*)tNode->GetSibling();
//			while( pSibling)
//			{
//				pLastSibling = pSibling;
//				pSibling     = (SceneNode*)pSibling->GetSibling();
//			}
//
//			pLastSibling->SetSibling( m_pSibling);
//
//			m_pSibling = tNode; 
//		}
//	}
//
//	// 设置父结点
//	void  SceneNode::SetParent( SceneNode* pParent) 
//	{ 
//		m_pParent = s_ptr<SceneNode>(pParent); 
//	}
//
//	// 连接子结点
//	bool SceneNode::AttachChild(SceneNode* pChild)
//	{
//		if (pChild)
//		{
//			// 有父结点,原父结点先删除此结点
//			if (pChild->GetParent())
//			{
//				return false;
//			}
//
//			SceneNode* tChild = (SceneNode*)pChild;
//			// 子结点与当前结点属于同一场景 
//			tChild->SetSibling(m_pChild);
//
//			m_pChild = pChild;
//			tChild->SetParent( this);
//		}
//
//		return true;
//	}
//
//	// 断开子结点
//	void SceneNode::DetachChild(SceneNode* pChild)
//	{
//		if (m_pChild && pChild)
//		{
//			SceneNode* tChild = (SceneNode*)pChild;
//
//			if (m_pChild == pChild)
//			{
//				m_pChild = tChild->GetSibling();
//
//				// 兄弟结点父结点设为NULL
//				tChild->SetSibling(NULL);		
//				tChild->SetParent(NULL);
//			}
//			
//			SceneNode* pTempNode = m_pChild;
//
//			// 目前的味一问题; 这里是线性搜索(！！！！！！改为类似于OGRE的Map！！！！！)
//			while ( pTempNode && pTempNode->GetSibling() != tChild)
//			{
//				pTempNode = (SceneNode*)pTempNode->GetSibling();
//			}
//
//			// 搜寻到子结点
//			if (pTempNode)
//			{
//				pTempNode->SetSibling(tChild->GetSibling());
//
//				tChild->SetSibling(NULL);
//				tChild->SetParent(NULL);
//			}
//		}
//	}
//
//	// 更新
//	void SceneNode::Update()
//	{
//		if ( !m_updateFlags.test( EM_IGNORE_ORIENTATION))
//		{
//			// has the orientation changed?
//			//if ( m_transform.IsLocalMatrixNew( /*m_localMatrix*/))
//			{
//				m_updateFlags.set( EM_NEW_LOCAL_MATRIX);
//			}
//		}
//
//		// derived classes update the local matrix then call this base function to complete the update
//		RecalcWorldMatrix();
//
//		RecalcWorldBounds();
//		//RefreshQuadTreeMembership();
//	}
//
//	// 递归更新
//	void SceneNode::RecursiveUpdate()
//	{
//		// update my self
//		Update();
//
//		// update my sibling
//		SceneNode* pSibling = GetSibling();
//		if ( pSibling)
//		{
//			pSibling->RecursiveUpdate();
//		}
//
//		// update my child
//		SceneNode* pChild = GetChild();
//		if ( pChild)
//		{
//			pChild->RecursiveUpdate();
//		}
//
//		// clear the flags from the previous update
//		PrepareForUpdate();
//	}
//
//	// 计算世界矩阵
//	void SceneNode::RecalcWorldMatrix()
//	{
//		bool parentMoved = m_pParent && m_pParent->IsWorldMatrixNew();
//		BOOL weMoved     = m_updateFlags.test( EM_NEW_LOCAL_MATRIX);
//
//		if ( weMoved || parentMoved)
//		{
//			// multiply our local matrix by our parent
//			if ( m_pParent)
//			{
//				if ( m_inheritance = EM_INHERITANCE_ALL)
//				{
//					m_matWorld = m_transform.GetMatrix() * m_pParent->GetWorldMatrix();
//
//					// 更新旋转矩阵
//					//m_matRotation = m_transform.m_matRotation * m_pParent->GetRotationMatrix();
//				}
//
//				//// 仅继承父结点的位置
//				//else if ( m_inheritance == EM_INHERITANCE_POSITION_ONLY)
//				//{
//				//	const Matrix44& pMat = m_pParent->GetWorldMatrix();
//
//				//	Matrix44 parentMatrix;
//				//	parentMatrix.SetIdentity();
//
//				//	parentMatrix._41 = pMat._41;
//				//	parentMatrix._42 = pMat._42;
//				//	parentMatrix._43 = pMat._43;
//
//				//	m_matWorld = m_transform.Matrix() * parentMatrix;
//				//}
//
//				//// 仅继承父结点缩放与旋转
//				//else if ( m_inheritance == EM_INHERITANCE_ROTSCALE_ONLY)
//				//{
//				//	Matrix44 parentMatrix( m_pParent->GetWorldMatrix());
//				//	parentMatrix._41 = 0.0f;
//				//	parentMatrix._42 = 0.0f;
//				//	parentMatrix._43 = 0.0f;
//
//				//	m_matWorld = m_transform.Matrix() * parentMatrix;
//
//				//	// 更新旋转矩阵
//				//	m_matRotation = m_transform.m_matRotation * m_pParent->GetRotationMatrix();
//				//}
//			}
//			else
//			{
//				m_matWorld = m_transform.GetMatrix();
//			}
//
//			// 设置位标记
//			m_updateFlags.set( EM_NEW_WORLD_MATRIX);
//
//			// and it's inverse & vectors are out of data
//			m_updateFlags.set( EM_REBUILD_INVERSE_WORLD_MATRIX);
//			m_updateFlags.set( EM_REBUILD_WORLD_VECTORS);
//		}
//	}
//
//	// 设置本地包围盒
//	void SceneNode::SetLocalBounds(Rect3& localBounds)
//	{
//		m_objectFlags.set(EM_NEW_LOCAL_BOUNDS);
//
//		m_localBounds = localBounds;
//	}
//
//	// 在原基础上更新包围盒 
//	void SceneNode::UnionLocalBounds( Rect3& unionBounds)
//	{
//		Rect3::Merge( m_localBounds, m_localBounds, unionBounds);
//	}
//
//	// 附加实体
//	void SceneNode::AttachObject(Entity* entity)
//	{
//		if ( entity)
//		{		
//			// 计算包围盒
//			Rect3 newEntityRect = entity->GetLocalBounds();
//
//			// 从后面添加实体
//			if ( m_pEntityLast)
//			{
//				m_pEntityLast->SetRearSearchLink( entity);
//				entity->SetForwardSearchLink( m_pEntityLast);
//				m_pEntityLast = entity;
//				m_iEntityNum++;
//
//				m_localBounds.UnionPoint( newEntityRect.GetMinPoint());
//				m_localBounds.UnionPoint( newEntityRect.GetMaxPoint());
//			}
//			else
//			{
//				m_localBounds = newEntityRect;
//
//				m_pEntityFirst = entity;
//				m_pEntityLast  = entity;
//				m_iEntityNum   = 1;
//			}
//		}
//
//		m_objectFlags.set( EM_NEW_LOCAL_BOUNDS);
//
//		// 立即更新世界包围盒
//		RecalcWorldBounds();
//	}
//
//	// 删除实体
//	void SceneNode::DetachObject( Entity* pEntity)
//	{
//		// 当前未处理多实体情况
//		m_iEntityNum--;
//
//		//if ( m_iEntityNum == 0)
//		//{
//		//	// 脱离四叉树搜索
//		//	DetachFromQuadTree();
//
//		//	// 脱离结点树
//		//	DetachParent();
//
//		//	// 删除自己
//		//	delete this;
//		//}	
//	}
//
//	// 准备更新
//	void SceneNode::PrepareForUpdate()
//	{
//		m_updateFlags.reset();
//		m_objectFlags.reset();
//	}
//
//	// 获取包围盒
//	const Rect3& SceneNode::GetWorldBounds() 
//	{ 
//		RecalcWorldBounds(); 
//		
//		return m_worldBounds; 
//	}
//
//	// 重新计算包围盒
//	void SceneNode::RecalcWorldBounds()
//	{
//		// nodes only get their bounds updated as nessesary
//		if ( m_objectFlags.test(EM_NEW_LOCAL_BOUNDS) || IsWorldMatrixNew())
//		{
//			m_worldBounds = m_localBounds;
//
//			m_worldBounds.Transform( &GetWorldMatrix());
//
//			m_objectFlags.set( EM_NEW_WORLD_BOUNDS);
//		}
//	}
//}