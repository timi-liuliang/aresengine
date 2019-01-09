#pragma once

#include "Core/AresMath.h"

using namespace boost;

namespace Ares
{
	enum BufferUsage
	{
		BU_Static,
		BU_Dynamic,
	};

	enum BufferAccess
	{
		BA_ReadOnly,
		BA_WriteOnly,
		BA_ReadWrite,
	};

	//--------------------------------------
	// 图形Buffer 2012-4-1  帝林
	//--------------------------------------
	class CORE_EXPORT GraphicsBuffer
	{
	public:
		// GraphicsBuffer Modifier
		class Modifier : noncopyable
		{
			friend class GraphicsBuffer;
		public:
			// constructors
			Modifier( s_ptr<GraphicsBuffer>& buffer, BufferAccess bufAccess)
				: m_buffer( buffer)
			{
				m_data = m_buffer->Map( bufAccess);
			}

			// desctructor
			~Modifier()
			{
				m_buffer->Unmap();
			}

			// returns the pointer of the data
			template<typename T>
			T* GetPointer()
			{
				return static_cast<T*>(m_data);
			}

			// returns the pointer of the data
			template<typename T>
			const T* GetPointer() const
			{
				return static_cast<T*>(m_data);
			}

			// returns size of the data
			template<typename T>
			int GetSize() const
			{
				// 除0判断
				A_ASSERT( sizeof( T));

				return m_buffer->GetSize() / sizeof(T);
			}

			// set data
			template<typename T>
			void Set( int idx, const T& t)
			{
				// 大小断言
				A_ASSERT( m_buffer->GetSize() / sizeof(t) > idx);

				// change value
				T* buf = static_cast<T*>( m_data);
				buf[idx] = t;
			}

			// set data
			void Set( const void* src, size_t sizeInByte)
			{
				//ARES_ASSERT( sizeInByte < m_buffer.GetSize());
				A_ASSERT( m_data);

				std::memcpy( m_data, src, sizeInByte);
			}

		private:
			s_ptr<GraphicsBuffer> m_buffer;		// buffer
			void*				  m_data;		// data
		};

	public:
		// 构造函数
		explicit GraphicsBuffer( BufferUsage usage, UINT accessHint);

		// 析构函数
		virtual ~GraphicsBuffer();

		// 获取大小(Byte 字节)
		UINT GetSize() const { return m_sizeInByte; }

		// 获取Usage
		BufferUsage GetUsage() const { return m_usage; }

		// 获取AccessHint
		UINT GetAccessHint() { return m_accessHint; }

		// 重置大小
		virtual void Resize( UINT sizeInByte) { m_sizeInByte = sizeInByte; }

	private:
		// Map
		virtual void* Map( BufferAccess bufAccess)=0;

		// Unmap
		virtual void Unmap()=0;

	protected:
		BufferUsage		m_usage;
		UINT			m_accessHint;
		UINT			m_sizeInByte;
	};
}