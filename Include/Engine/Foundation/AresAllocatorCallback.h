#pragma once

namespace Ares
{
	//----------------------------------------------------------------------
	// IAllocatorCallback 2011-10-25 ╣шаж
	// Brief abstract bass class for an application defined memory allocator
	// note the sdk state should not be modified from within any allocation/
	// free function.
	class IAllocatorCallback
	{
	public:
		// brief destructor
		virtual ~IAllocatorCallback() {}

		// allocates size bytes of memory, which must be 16-byte aligned
		// this method should never return NULL, If you run out of memory
		// the you should terminate the app or take some other appropriate
		// action
		virtual void* Allocate( size_t size, const char* typeName, const char* fileName, int line)=0;

		// reallocate
		virtual void  Reallocate( void* ptr, size_t size)=0;

		// brief frees memory previously allocated by allocate
		// this function should be thread safe as it can be called in the context of the user thread
		// and physics processing threads
		virtual void Deallocate( void* ptr)=0;
	};
}
