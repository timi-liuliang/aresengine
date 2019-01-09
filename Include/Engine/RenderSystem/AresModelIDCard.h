#pragma once

#include <cstdio>
#include <string.h>
#include <Physics/Shapes.h>
#include <Core/Graphics/AresElementFormat.h>

using namespace std;

namespace Ares
{
#pragma pack( 1)
	//--------------------------------
	// m1 animation header
	struct SM1AnimationsHeader
	{
		unsigned int   m_iNumBones;			// Number of bones
		unsigned int   m_iNumAnimationSet;	// Number of animationsets

		// constructor
		SM1AnimationsHeader()
		{
			m_iNumBones		   = 0;
			m_iNumAnimationSet = 0;
		}
	};

	//--------------------------------------
	// SM1Bone
	// A single bone in the M1 file
	struct SM1Bone
	{
		char	m_cName[68];
		int     m_iSiblingIndex;
		int     m_iFirstChildIndex;
		float   m_boneOffset[4][4];

		SM1Bone()
		{
			m_iSiblingIndex    = -1;
			m_iFirstChildIndex = -1;
		}
	};


	//----------------------------------------------
	//	关键帧容器(结构体存储关键帧数据)
	//----------------------------------------------
	struct SM1AnimationVectorKey	   // 缩放平移
	{
		float  m_fTime; 
		float  m_vecKey[3];
	};

	struct SM1AnimationQuaternionKey  // 旋转 
	{
		float  m_fTime;
		float  m_quatKey[4];
	};

	struct SM1AnimationMatrixKey	   // 矩阵
	{
		float  m_fTime;
		float  m_matKey[4][4];
	};

	//---------------------------------------
	// SM1AnimationHeader
	// Animation header
	struct SM1AnimationHeader
	{
		int				m_iBoneIndex;		// 骨骼索引
		unsigned int	m_iNumTransKeys;		
		unsigned int	m_iNumScaleKeys;
		unsigned int    m_iNumRotationKeys;
		unsigned int    m_iNumMatKeys;

		// constructor
		SM1AnimationHeader()
		{
			m_iBoneIndex		= -1;
			m_iNumTransKeys		= 0;			
			m_iNumScaleKeys		= 0;			
			m_iNumRotationKeys	= 0;			
			m_iNumMatKeys		= 0;
		}
	};

	//---------------------------------------
	// SM1Animation
	// A Animation container
	struct SM1Animation
	{
		SM1AnimationHeader         m_header;
		SM1AnimationVectorKey	   *m_pTransKeys;
		SM1AnimationVectorKey     *m_pScaleKeys;
		SM1AnimationQuaternionKey *m_pRotationKeys;
		SM1AnimationMatrixKey     *m_pMatKeys;		

		// constructor
		SM1Animation()
		{
			m_pTransKeys		= 0;
			m_pScaleKeys		= 0;
			m_pRotationKeys		= 0;
			m_pMatKeys			= 0;		
		}

		// destructor
		~SM1Animation()
		{
			if ( m_pTransKeys)
			{
				delete[] m_pTransKeys;
			}

			if ( m_pScaleKeys)
			{
				delete[] m_pScaleKeys;
			}

			if ( m_pRotationKeys)
			{
				delete[] m_pRotationKeys;
			}

			if ( m_pMatKeys)
			{
				delete[] m_pMatKeys;
			}

			m_pTransKeys	= 0;
			m_pScaleKeys	= 0;
			m_pRotationKeys = 0;
			m_pMatKeys		= 0;
		}
	};

	//---------------------------------------
	// SM1AnimationSetHeader
	struct SM1AnimationSetHeader
	{
		char	m_cName[68];
		float   m_fLenght;
		unsigned int m_iNumBoneUses;	// 此动画使用的骨骼数量

		// constructor
		SM1AnimationSetHeader()
		{
			m_fLenght      = 0.f;
			m_iNumBoneUses = 0;
		}
	};

	//---------------------------------------
	// SM1AnimationSet
	// A single animation
	struct SM1AnimationSet
	{
		SM1AnimationSetHeader  m_header;
		SM1Animation*		   m_pAnimations;
		bool*				   m_boneUsed;			// 骨骼是否使用

		// Constructor
		SM1AnimationSet()
		{
			m_boneUsed	   = 0;
			m_pAnimations  = 0;
		}

		// Destructor
		~SM1AnimationSet()
		{
			if ( m_pAnimations)
				delete[] m_pAnimations;

			if( m_boneUsed)
				delete[] m_boneUsed;

			m_pAnimations = NULL;
			m_boneUsed	  = NULL;
		}
	};

#pragma pack()

	//----------------------------------------
	// CM1Animation 2011-06-17 帝林
	class CM1Animation
	{
	public:
		// 构造函数
		CM1Animation();

		// 析构函数
		~CM1Animation();

		// Load an M1 animations
		bool Load( const char* szFilename, char* error=0);

		// Save an M1 animations
		bool Save( const char* szFilename);

	public:
		SM1AnimationsHeader m_header;			// Animations Info	
		SM1Bone*			m_pRootBone;		// Bones
		SM1AnimationSet*	m_pAnimationSets;	// AnimationSet
	};

	//------------------------------------
	// collmesh   2011-09-02 帝林
	class IDCardCollMesh
	{
		vector<Vector3> m_verts;		// 顶点
		vector<Int3>	m_faces;		// 三角形

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_verts;
			ar & m_faces;
		}
	};
}