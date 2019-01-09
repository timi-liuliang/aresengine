#include <io.h>
#include "Core/Graphics/EffectLoader.h"

namespace Ares
{
	// 错误消息提醒
	static void DebugPrintf( vector<string>& errors, const char* format, ...)
	{
		char    buf[2048];
		va_list args;

		va_start(args, format);
		_vsnprintf( buf, 2048, format, args); 
		va_end(args);

		errors.push_back( buf);
	}

	#define DPF DebugPrintf

	// 构造函数
	CgEffectLoader::CgEffectLoader( const char* fileName)
		: m_reflection( NULL)
	{
		FILE* fileHandle = fopen( fileName, "rt");
		if( fileHandle)
		{
			// 检测文件大小,一次性读进内存
			fseek( fileHandle, 0, SEEK_END);
			size_t iEnd = ftell( fileHandle);
			fseek( fileHandle, 0, SEEK_SET);
			size_t iStart = ftell( fileHandle);
			size_t fileSize = iEnd - iStart;

			// Allocate memory for whole file
			BYTE* effectBuffer = new_ BYTE[ fileSize];
			if ( !effectBuffer) 
				return;

			// load file into buffer
			size_t size = fread( effectBuffer, 1, fileSize, fileHandle);
			if ( size != fileSize) 
				DPF( m_errors, "Warning : size %d != fileSize %d", size, fileSize);

			m_data     = effectBuffer,
			m_dataSize = size;

			// close the file , we don't need it any more
			fflush( fileHandle);
			fclose( fileHandle);
		}

		DPF( m_errors, "Error : Open %s failed!", fileName);
	}

	// 构造函数
	CgEffectLoader::CgEffectLoader( const void* effectBuffer, size_t size)
		: m_dataSize( size),
		  m_reflection( NULL)
	{
		m_data =(BYTE*)effectBuffer;
	}

	// 析构函数
	CgEffectLoader::~CgEffectLoader()
	{
		SAFE_DELETE( m_reflection);
		SAFE_DELETE_ARRAY( m_data);
	}

	// 剖析
	bool CgEffectLoader::Parser()
	{
		// buffer is null
		if( !m_data)
		{
			DPF( m_errors, "Error : effectBuffer is NULL");
			return false;
		}

		m_reflection = new_ Cg::Heap;

		return true;
	}
}