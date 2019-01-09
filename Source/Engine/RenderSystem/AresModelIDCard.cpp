#include <cstdio>
#include <Engine/RenderSystem/AresModelIDCard.h>

using namespace std;

namespace Ares
{
	// 构造函数
	CM1Animation::CM1Animation()
	{
		m_pRootBone		 = 0;
		m_pAnimationSets = 0;
	}

	// 析构函数
	CM1Animation::~CM1Animation()
	{
		if ( m_pRootBone)
		{
			delete[]  m_pRootBone;
			m_pRootBone = 0;
		}

		if ( m_pAnimationSets)
		{
			delete[] m_pAnimationSets;
		}
	}

	// Load an M1 animations
	bool CM1Animation::Load( const char* szFilename, char* error/*=0*/)
	{
		FILE*  fileHandle = NULL;
		if ( !( fileHandle = fopen( szFilename, "rb"))) 
			return false;

		// 检测文件大小,一次性读进内存
		fseek( fileHandle, 0, SEEK_END);
		size_t iEnd = ftell( fileHandle);
		fseek( fileHandle, 0, SEEK_SET);
		size_t iStart = ftell( fileHandle);
		size_t fileSize = iEnd - iStart;

		// Allocate memory for whole file
		unsigned char* ucpBuffer = NULL;
		unsigned char* ucpPtr    = NULL;
		unsigned char* ucpTmpPtr = NULL;

		ucpBuffer = new unsigned char[ fileSize];
		ucpPtr    = ucpBuffer;

		if ( !ucpBuffer) return false;

		// load file into buffer
		size_t size = fread( ucpBuffer, 1, fileSize, fileHandle);
		if ( size != fileSize) return false;

		// close the file , we don't need it any more
		fflush( fileHandle);
		fclose( fileHandle);

		//---------------------------
		// Make sure header is valid
		memcpy( &m_header, ucpPtr, sizeof( SM1AnimationsHeader));
		ucpPtr += sizeof( SM1AnimationsHeader);

		// read bone data
		if ( m_header.m_iNumBones)
		{
			m_pRootBone = new SM1Bone[ m_header.m_iNumBones];
			memcpy( m_pRootBone, ucpPtr, sizeof( SM1Bone) * m_header.m_iNumBones);
			ucpPtr += sizeof( SM1Bone) * m_header.m_iNumBones;
		}

		if( m_header.m_iNumAnimationSet > 0)
			m_pAnimationSets = new SM1AnimationSet[m_header.m_iNumAnimationSet];
		// read animationset
		for ( size_t i=0; i<m_header.m_iNumAnimationSet; i++)
		{
			// read animationset header
			memcpy( &m_pAnimationSets[i].m_header, ucpPtr, sizeof( SM1AnimationSetHeader));
			ucpPtr += sizeof( SM1AnimationSetHeader);

			if( m_pAnimationSets[i].m_header.m_iNumBoneUses > 0)
			{
				m_pAnimationSets[i].m_boneUsed	  = new bool[m_pAnimationSets[i].m_header.m_iNumBoneUses];	
				m_pAnimationSets[i].m_pAnimations = new SM1Animation[m_pAnimationSets[i].m_header.m_iNumBoneUses];
			}

			// 骨骼是否使用
			memcpy( m_pAnimationSets[i].m_boneUsed, ucpPtr,sizeof( bool) * m_pAnimationSets[i].m_header.m_iNumBoneUses);
			ucpPtr += sizeof( bool) * m_pAnimationSets[i].m_header.m_iNumBoneUses;

			for ( unsigned int j=0; j<m_pAnimationSets[i].m_header.m_iNumBoneUses; j++)
			{
				// save animation header
				memcpy( &m_pAnimationSets[i].m_pAnimations[j].m_header, ucpPtr,sizeof( SM1AnimationHeader));
				ucpPtr += sizeof( SM1AnimationHeader);

				const SM1AnimationHeader& header =  m_pAnimationSets[i].m_pAnimations[j].m_header;

				// save translation
				if ( header.m_iNumTransKeys)
				{		
					m_pAnimationSets[i].m_pAnimations[j].m_pTransKeys = new SM1AnimationVectorKey[header.m_iNumTransKeys];
					memcpy( m_pAnimationSets[i].m_pAnimations[j].m_pTransKeys,ucpPtr, sizeof( SM1AnimationVectorKey) * header.m_iNumTransKeys);
					ucpPtr += sizeof( SM1AnimationVectorKey) * header.m_iNumTransKeys;
				}

				// save translation
				if ( header.m_iNumScaleKeys)
				{
					m_pAnimationSets[i].m_pAnimations[j].m_pScaleKeys = new SM1AnimationVectorKey[header.m_iNumScaleKeys];
					memcpy( m_pAnimationSets[i].m_pAnimations[j].m_pScaleKeys, ucpPtr,sizeof( SM1AnimationVectorKey) * header.m_iNumScaleKeys);
					ucpPtr += sizeof( SM1AnimationVectorKey) * header.m_iNumScaleKeys;
				}

				// save translation
				if ( header.m_iNumRotationKeys)
				{
					m_pAnimationSets[i].m_pAnimations[j].m_pRotationKeys = new SM1AnimationQuaternionKey[header.m_iNumRotationKeys];
					memcpy( m_pAnimationSets[i].m_pAnimations[j].m_pRotationKeys, ucpPtr, sizeof( SM1AnimationQuaternionKey) * header.m_iNumRotationKeys);
					ucpPtr += sizeof( SM1AnimationQuaternionKey) * header.m_iNumRotationKeys;
				}				

				// save translation
				if ( header.m_iNumMatKeys)
				{	
					m_pAnimationSets[i].m_pAnimations[j].m_pMatKeys = new SM1AnimationMatrixKey[header.m_iNumMatKeys];
					memcpy( m_pAnimationSets[i].m_pAnimations[j].m_pMatKeys, ucpPtr, sizeof( SM1AnimationMatrixKey) * header.m_iNumMatKeys);
					ucpPtr += sizeof( SM1AnimationMatrixKey) * header.m_iNumMatKeys;
				}

			}
		}

		// clean up
		if ( ucpBuffer)
			delete[] ucpBuffer;

		return true;
	}

	// Save an M1 animations
	bool CM1Animation::Save( const char* szFilename)
	{
		FILE* fileHandle = fopen( szFilename, "wb");

		if ( fileHandle)
		{
			// Save Header
			fwrite( &m_header, sizeof( SM1AnimationsHeader), 1, fileHandle);

			// Save bone 
			fwrite( m_pRootBone, sizeof( SM1Bone) * m_header.m_iNumBones, 1, fileHandle);

			// Save animationset
			for ( size_t i=0; i<m_header.m_iNumAnimationSet; i++)
			{
				// save animationset header
				fwrite( &m_pAnimationSets[i].m_header, sizeof( SM1AnimationSetHeader), 1, fileHandle);

				// save bone used data
				fwrite( m_pAnimationSets[i].m_boneUsed, sizeof(bool)*m_header.m_iNumBones, 1, fileHandle);

				for ( unsigned int j=0; j<m_pAnimationSets[i].m_header.m_iNumBoneUses; j++)
				{
					// save animation header
					fwrite( &m_pAnimationSets[i].m_pAnimations[j].m_header, sizeof( SM1AnimationHeader), 1, fileHandle);

					const SM1AnimationHeader& header =  m_pAnimationSets[i].m_pAnimations[j].m_header;

					// save translation
					fwrite( m_pAnimationSets[i].m_pAnimations[j].m_pTransKeys, sizeof( SM1AnimationVectorKey) * header.m_iNumTransKeys, 1, fileHandle);

					// save translation
					fwrite( m_pAnimationSets[i].m_pAnimations[j].m_pScaleKeys, sizeof( SM1AnimationVectorKey) * header.m_iNumScaleKeys, 1, fileHandle);

					// save translation
					fwrite( m_pAnimationSets[i].m_pAnimations[j].m_pRotationKeys, sizeof( SM1AnimationQuaternionKey) * header.m_iNumRotationKeys, 1, fileHandle);

					// save translation
					fwrite( m_pAnimationSets[i].m_pAnimations[j].m_pMatKeys, sizeof( SM1AnimationMatrixKey) * header.m_iNumMatKeys, 1, fileHandle);
				}
			}

			fflush( fileHandle);
			fclose( fileHandle);

			return true;
		}

		return false;
	}
}