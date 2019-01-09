#include "IGame_AnimationManager.h"
#include "Init.h"
#include "AeConfig.h"
#include <BipedApi.h>
#include <bipexp.h>
#include <sstream>
#include <decomp.h>
#include <Core/AresMath.h>
#include "Function.h"

using namespace Ares;
using namespace std;

extern Interface*   g_pI;

namespace M1_IGame
{
	extern IGameScene *     g_pIGame;
	extern SExportConfig    g_config;	// 导出配置

	// 获取关键帧时间
	static TimeValue GetKeyTime(Control* control, int i);

	// 构造函数
	CAnimationManager::CAnimationManager()
	{
		// 初始化场景
		InitScene();
	}

	// 添加动画
	bool CAnimationManager::AddAnimation( const string& aniName, int start, int end, CM1Animation& m1Animation)
	{
		Interval interval;
		interval.SetStart( start * g_pIGame->GetSceneTicks());
		interval.SetEnd( end * g_pIGame->GetSceneTicks());

		// 获取所有的max结点
		m_nodes.clear();
		FindAllNodes( m_nodes);

		// 获取骨骼结点
		m_boneNodes.clear();
		for ( size_t i=0; i<m1Animation.m_header.m_iNumBones; i++)
		{
			SM1Bone& bone = m1Animation.m_pRootBone[i];
			
			IGameNode* pGameNode = GetIGameNodeByName( bone.m_cName, m_nodes);
			if( pGameNode)
				m_boneNodes.push_back( pGameNode);
			else
			{
				ostringstream oss;
				oss << "无法添加动画,骨骼\"" << bone.m_cName << "\"不存在...";

				::MessageBoxA( NULL, oss.str().c_str(), "AresX Warn", MB_OK);

				m_boneNodes.push_back( NULL);
			}
		}

		// 获取要想导出骨骼主结点
		GetExportBones();

		// 新加数据
		SM1AnimationSet animationSet;

		strcpy( animationSet.m_header.m_cName, aniName.c_str());
		animationSet.m_header.m_fLenght = TicksToSec(interval.Duration()-1);		// 动画时长
		animationSet.m_header.m_iNumBoneUses = m_boneNodes.size();
		animationSet.m_boneUsed	   = new bool[m_boneNodes.size()];
		animationSet.m_pAnimations = new SM1Animation[m_boneNodes.size()];

		// 骨骼
		for ( size_t i=0; i<m_boneNodes.size(); i++)
		{
			SM1Animation& boneAnimation = animationSet.m_pAnimations[i];
			bool&		  boneUsed		= animationSet.m_boneUsed[i];

			if( m_boneNodes[i] && IsNeedExport( m_boneNodes[i]))
			{
				// 骨骼索引
				boneUsed = true;
				boneAnimation.m_header.m_iBoneIndex = i;

				Control* pControl = m_boneNodes[i]->GetMaxNode()->GetTMController();
				if( g_config.m_keyFrame)
				{
					//SampleQuaternion( boneAnimation, m_boneNodes[i], interval);
					vector<TimeValue> keyTimes;
					GetBoneAnimationSetTime(keyTimes, interval, m_boneNodes[i]);
					if( keyTimes.size() > 0)
						BuildKeys( boneAnimation, m_boneNodes[i], keyTimes, interval);
					else
						SampleKeys( boneAnimation, m_boneNodes[i], interval);
				}
				else
				{
					SampleKeys( boneAnimation, m_boneNodes[i], interval);
				}
			}
			else
			{
				// 标记无此骨骼数据
				boneUsed = false;
				boneAnimation.m_header.m_iBoneIndex = -1;
			}
		}

		// 附加animationSet
		int aniSetNum = m1Animation.m_header.m_iNumAnimationSet;
		m1Animation.m_header.m_iNumAnimationSet = aniSetNum+1;
		SM1AnimationSet* tAnimations = m1Animation.m_pAnimationSets;
		m1Animation.m_pAnimationSets = new SM1AnimationSet[ aniSetNum+1];

		// 前期复制
		if( tAnimations)
			memcpy( m1Animation.m_pAnimationSets, tAnimations, sizeof( SM1AnimationSet) * aniSetNum);
		
		m1Animation.m_pAnimationSets[aniSetNum] = animationSet;

		// 释放内存
		for ( int i=0; i<aniSetNum; i++)
		{	
			tAnimations[i].m_pAnimations = NULL;
			tAnimations[i].m_boneUsed	 = NULL;
		}

	
		if( aniSetNum)
			delete[] tAnimations;

		animationSet.m_pAnimations = NULL;
		animationSet.m_boneUsed	   = NULL;

		return true;
	}

	// 删除动画
	bool CAnimationManager::DelAnimation( int aniIndex, CM1Animation& m1Animation)
	{
		int oldAniSetNum = m1Animation.m_header.m_iNumAnimationSet;

		// 合法性检测
		if( oldAniSetNum < aniIndex || aniIndex < 0)
			return false;

		if( m1Animation.m_pAnimationSets[aniIndex].m_pAnimations)
			delete[] m1Animation.m_pAnimationSets[aniIndex].m_pAnimations;
		if( m1Animation.m_pAnimationSets[aniIndex].m_boneUsed)
			delete[] m1Animation.m_pAnimationSets[aniIndex].m_boneUsed;

		for( int j=aniIndex; j<oldAniSetNum-1; j++)
		{
			m1Animation.m_pAnimationSets[j] = m1Animation.m_pAnimationSets[j+1];
		}

		// 防止2次删除
		m1Animation.m_pAnimationSets[oldAniSetNum-1].m_pAnimations = NULL;
		m1Animation.m_pAnimationSets[oldAniSetNum-1].m_boneUsed	   = NULL;

		m1Animation.m_header.m_iNumAnimationSet--;

		return true;
	}

	// 修改动画名字
	bool CAnimationManager::EditAnimation( const string& newName, int index,CM1Animation& m1Animation)
	{
		return true;
	}

	// 交换动画顺序
	bool CAnimationManager::ExchangeAnimation( int index0, int index1, CM1Animation& m1Animation)
	{
		return true;
	}

	// 添加子结点
	void CAnimationManager::FindChildNodes( IGameNode* parent, vector<IGameNode*>& out)
	{
		if( !parent)
			return;

		for ( int i=0; i<parent->GetChildCount(); i++)
		{
			IGameNode* tChild = parent->GetNodeChild( i);
			if( tChild)
			{
				out.push_back( tChild);

				FindChildNodes( tChild, out);
			}
		}
	}

	// 获取所有接点
	void CAnimationManager::FindAllNodes( vector<IGameNode*>& out)
	{
		out.clear();

		for( int i=0; i<g_pIGame->GetTopLevelNodeCount(); i++)
		{
			IGameNode* topNode = g_pIGame->GetTopLevelNode( i);
			out.push_back( topNode);

			FindChildNodes( topNode, out);
		}
	}

	// 获取结点
	IGameNode* CAnimationManager::GetIGameNodeByName( const char* name, vector<IGameNode*>& nodes)
	{
		for ( size_t i=0; i<nodes.size(); i++)
		{
			IGameNode* pNode = nodes[i];
			if( !pNode)
				return NULL;

			if( strcmp( pNode->GetName(), name)==0)
				return pNode;
		}

		return NULL;
	}

	// 构建TransKeys
	void CAnimationManager::BuildKeys( SM1Animation& boneAnimation, IGameNode* pGameNode, vector<TimeValue>& keyTimes,Interval interval)
	{	
		boneAnimation.m_header.m_iNumRotationKeys = keyTimes.size();
		boneAnimation.m_header.m_iNumTransKeys	  = keyTimes.size();

		boneAnimation.m_pRotationKeys = new SM1AnimationQuaternionKey[keyTimes.size()];
		boneAnimation.m_pTransKeys    = new SM1AnimationVectorKey[keyTimes.size()];

		for( size_t i=0; i<keyTimes.size(); i++)
		{
			TimeValue t		  = keyTimes[i];
			TimeValue tn	  = t - interval.Start();
			Transform  tfL	  = GMatrixToAresTransform( pGameNode->GetWorldTM( t));
			IGameNode* parent = pGameNode->GetNodeParent();
			if ( parent && IsBone( parent))
			{
				Transform tfP = GMatrixToAresTransform( parent->GetWorldTM( t));
				
				tfL = ~tfP * tfL;
			}

			// 时间
			boneAnimation.m_pRotationKeys[i].m_fTime = TicksToSec( tn);
			boneAnimation.m_pTransKeys[i].m_fTime    = TicksToSec( tn);
			
			memcpy( boneAnimation.m_pRotationKeys[i].m_quatKey, &tfL.m_rotation, sizeof( Ares::Quat));
			memcpy( boneAnimation.m_pTransKeys[i].m_vecKey, &tfL.m_origin, sizeof( Ares::Vector3)); 
		}
	}

	// 采样动画
	void CAnimationManager::SampleKeys( SM1Animation& boneAnimation, IGameNode* pGameNode, Interval interval)
	{
		int    delta   = g_pIGame->GetSceneTicks() * g_config.m_sampledensity;
		int    numKeys = ( interval.Duration()) / delta + 1;

		if((interval.Duration()-1) % delta)
			numKeys += 1;

		if( numKeys > 0)
		{
			boneAnimation.m_header.m_iNumMatKeys    = numKeys;

			boneAnimation.m_pMatKeys = new SM1AnimationMatrixKey[numKeys];
		}

		vector<AngAxis> angAxis;
		angAxis.clear();
		for ( int i=0; i<numKeys; i++)
		{
			TimeValue tn  = i  * delta;

			// 防止过界
			if( tn >= interval.Duration())
				tn = interval.Duration()-1;

			TimeValue t   = tn + interval.Start();

			GMatrix  matT = pGameNode->GetWorldTM( t);

			IGameNode* parent = pGameNode->GetNodeParent();
			if ( parent && IsBone( parent))
			{
				GMatrix matParent = parent->GetWorldTM( t);
				matParent = matParent.Inverse();

				matT *=  matParent;
			}

			// 时间
			boneAnimation.m_pMatKeys[i].m_fTime		 = TicksToSec( tn);

			// data
			for ( int j=0; j<4; j++)
				for ( int k=0; k<4; k++)
				{
					boneAnimation.m_pMatKeys[i].m_matKey[j][k] = matT[j][k];
				}

			//boneAnimation.m_pMatKeys[i].m_matKey[3][0] *= g_config.m_scale.x;
			//boneAnimation.m_pMatKeys[i].m_matKey[3][1] *= g_config.m_scale.y;
			//boneAnimation.m_pMatKeys[i].m_matKey[3][2] *= g_config.m_scale.z;
		}
	}

	struct M1Key : public IKey
	{
		float val[1024];
	};

	// 获取关键帧时间
	static TimeValue GetKeyTime(Control* control, int i)
	{
		if( !control)
			return 0;

		static M1Key key;
		IKeyControl* keyControl = GetKeyControlInterface( control);
		if( keyControl)
		{
			keyControl->GetKey( i, &key);

			TimeValue time = key.time;

			return time;
		}

		return -1;
	}

	// 获取关键帧时间点
	void CAnimationManager::GetBoneAnimationSetTime( vector<TimeValue>& keyTimes, Interval interval, IGameNode* gameNode)
	{
		Control* control = gameNode->GetMaxNode()->GetTMController();

		Control* tControl;
		IBipMaster* pBip = GetBipMasterInterface( control);
		if( pBip)
		{
			Point3 pos = pBip->GetBipedPos( 0, gameNode->GetMaxNode());
			//Quat   quat= pBip->GetBipedRot( 0, gameNode->GetMaxNode());

			// 水平控制器
			tControl = pBip->GetHorizontalControl();
			AddKeyTime( keyTimes, tControl, interval);

			// 垂直控制器
			tControl = pBip->GetVerticalControl();
			AddKeyTime( keyTimes, tControl, interval);

			tControl = pBip->GetTurnControl();
			AddKeyTime( keyTimes, tControl, interval);
		}

		// 非bip
		tControl = control->GetPositionController();
		AddKeyTime( keyTimes, tControl, interval);

		tControl = control->GetRotationController();
		AddKeyTime( keyTimes, tControl, interval);

		tControl = control->GetScaleController();
		AddKeyTime( keyTimes, tControl, interval);

		// 开始与结尾关键点
		AddKeyTime( keyTimes, interval.End());
		AddKeyTime( keyTimes, interval.Start());

		IGameNode* parentNode = gameNode->GetNodeParent();
		if( parentNode && IsBone( parentNode))
			GetBoneAnimationSetTime( keyTimes, interval, parentNode);
	}

	// 添加控制器关键时间点
	void CAnimationManager::AddKeyTime( vector<TimeValue>& keyTimes, Control* contrl, Interval interval)
	{
		if( !contrl)
			return;

		BOOL	bResult;
		for(TimeValue keyTime = interval.Start(); keyTime<=interval.End(); keyTime += g_pIGame->GetSceneTicks())
		{
			bResult =contrl->IsKeyAtTime( keyTime, KEYAT_POSITION|KEYAT_ROTATION|KEYAT_SCALE);
			if(bResult)
				AddKeyTime( keyTimes, keyTime);
		}
	}

	// 添加关键时间点
	void CAnimationManager::AddKeyTime( vector<TimeValue>& keyTimes, TimeValue keyTime)
	{
		vector<TimeValue>::iterator it;
		vector<TimeValue>::iterator	it_below = keyTimes.begin();
		for ( it=keyTimes.begin(); it!=keyTimes.end(); it++)
		{
			if( *it == keyTime)
				return;
			else if( *it < keyTime)
				it_below++;
		}

		keyTimes.insert( it_below, keyTime);
	}

	// 判断某结点是否是骨骼
	bool CAnimationManager::IsBone( IGameNode* gameNode)
	{
		for ( size_t i=0; i<m_boneNodes.size(); i++)
		{
			if( gameNode == m_boneNodes[i])
				return true;
		}

		return false;
	}

	// 判断骨骼是否导出
	bool CAnimationManager::IsNeedExport( IGameNode* gameNode)
	{
		// 空结点无需导出
		if( !gameNode)
			return false;

		// 非骨骼结点无需导出
		if( !IsBone( gameNode))
			return false;

		// 全部导出
		if( !m_exportNodes.size())
			return true;

		// 是否为需要导出的骨骼
		IGameNode* parentNode = gameNode;
		while( parentNode)
		{
			for( size_t i=0; i<m_exportNodes.size(); i++)
			{
				if( parentNode == m_exportNodes[i])
				{
					return true;
				}
			}

			parentNode = parentNode->GetNodeParent();
		}
		
		return false;
	}

	// 解包字符串(int)
	static void UnPackString(vector<string>& out, const string& source, char separator)
	{
		std::basic_string <char>::size_type last_pos = 0;
		std::basic_string <char>::size_type pos = 0;
		std::string range_elem;
		while( pos < source.size())
		{
			range_elem.clear();
			pos = source.find( separator, pos);
			for ( size_t i = last_pos;  i < pos; ++i )
			{
				range_elem += source[i];
			}
			last_pos = ++pos;

			out.push_back( range_elem);
		}
	}

	// 获取要导出的骨骼父结点
	void CAnimationManager::GetExportBones()
	{
		m_exportNodes.clear();

		for( int i=0; i<g_pI->GetNumProperties(PROPSET_USERDEFINED); i++)
		{
			const PROPSPEC*	   propSpec= g_pI->GetPropertySpec(PROPSET_USERDEFINED, i);
			const PROPVARIANT* propVar = g_pI->GetPropertyVariant(PROPSET_USERDEFINED, i);

			if(propVar->vt != VT_LPWSTR && propVar->vt != VT_LPSTR)
				continue;

			char	key[1024];
			char	value[1024];
			_tcscpy(key, TSTR(propSpec->lpwstr));
			_tcscpy(value, propVar->vt == VT_LPWSTR ? TSTR(propVar->pwszVal) : TSTR(propVar->pszVal));

			if(!_strnicmp( key,"export_bones", 12))
			{
				vector<string> boneNames;
				UnPackString( boneNames, value, ';');

				for( size_t i=0; i<boneNames.size(); i++)
				{
					IGameNode* boneNode = GetIGameNodeByName( boneNames[i].c_str(), m_boneNodes);
					if( boneNode)
						m_exportNodes.push_back( boneNode);
				}
			}
		}
	}
}