#pragma once

#include "AresPlatform.h"

namespace Ares
{
	//--------------------------------------------
	// 辅助文件加载 2012/2/4  帝林
	//--------------------------------------------
	class IFileIO
	{
	public:
		// 构造函数
		IFileIO();

		// 析构函数
		virtual ~IFileIO();

	public:
		// 加载xml格式
		virtual BOOL LoadXml( const char* fileName)=0;

		// 保存xml格式
		virtual BOOL SaveXml( const char* fileName) { return FALSE; }

		// 加载二进制格式
		virtual BOOL LoadBinary( const char* fileName);

		// 保存为二进制格式
		virtual BOOL SaveBinary( const char* fileName);

	public:
		// 清除数据
		virtual void Clear();

	protected:
		UINT	m_dataSize;	// 数据大小
		BYTE*	m_data;		// 数据
	};

	//
	// INLINE
	//

	// 构造函数
	INLINE IFileIO::IFileIO()
		: m_data(NULL),
		  m_dataSize( 0)
	{}

	// 析构函数
	INLINE IFileIO::~IFileIO()
	{
		m_dataSize = 0;
		SAFE_DELETE_ARRAY( m_data);
	}

	// 清除数据
	INLINE void IFileIO::Clear()
	{
		m_dataSize = 0;
		SAFE_DELETE_ARRAY( m_data);
	}

	// 加载二进制格式
	INLINE BOOL IFileIO::LoadBinary( const char* fileName)
	{
		if( !fileName)
			return false;

		FILE* fileHandle = fopen( fileName, "rb");
		if( fileHandle)
		{
			// 加载数据大小
			fread( &m_dataSize, sizeof( UINT), 1, fileHandle);

			// 加载数据实体
			m_data = new_ BYTE[m_dataSize];
			fread( m_data, m_dataSize, 1, fileHandle);

			fflush( fileHandle);
			fclose( fileHandle);

			return TRUE;
		}

		return FALSE;
	}

	// 保存为二进制格式
	INLINE BOOL IFileIO::SaveBinary( const char* fileName)
	{
		if( !fileName)
			return false;

		FILE* fileHandle = fopen( fileName, "wb");
		if( fileHandle)
		{
			// 保存数据大小
			fwrite( &m_dataSize, sizeof( UINT), 1, fileHandle);

			// 保存数据实体
			fwrite( m_data, m_dataSize, 1, fileHandle);

			fflush( fileHandle);
			fclose( fileHandle);

			return TRUE;
		}

		return FALSE;
	}
}