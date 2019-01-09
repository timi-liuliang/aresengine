#pragma once

#include <vector>

namespace Ares
{
	//------------------------------------------------
	// Pool 2011-9-19 帝林
	// A generic container to handle a pool of objects
	//------------------------------------------------
	template<class T>
	class CPool
	{
	public:
		typedef typename std::vector<T>::iterator				iterator;
		typedef typename std::vector<T>::const_iterator			const_iterator;
		typedef typename std::vector<T>::reverse_iterator		reverse_iterator;
		typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

		static const size_t DEFAULT_CAPACITY = 256;

		// Constructor
		CPool<T>(size_t capactity=DEFAULT_CAPACITY);

	public:
		// operator []
		T& operator[]( size_t index);

		// 重载[]运算符
		const T& operator[](size_t index) const;

	public:
		// Gets an iterator referring to the first active element in this pool
		iterator begin() { return m_container.begin(); }

		// Gets an iterator referring to the past-the-end active element in this pool
		iterator end() { return m_container.begin() + m_numActive; }

	public:
		// Adds an active element to this pool
		bool PushActive( T& element);

		// actives the first inactive element
		T* MakeActive();

		// inactive an active element
		void MakeInactive( size_t index);

		// Gets the number of active elements in this pool
		size_t GetNumActive() const { return m_numActive; }

		// Gets the number of inactive elements in this pool
		size_t GetNumInactive() const;

		// Gets the room left for new elements in this pool
		size_t GetNumEmpty() const { return m_container.capacity() - m_container.size(); }

		// swap elements
		void SwapElements( T& a, T& b) { std::swap( a, b); }

	private:
		std::vector<T>   m_container;
		size_t			 m_numActive;
		size_t			 m_maxTotal;
	};

	// constructor
	template<class T>
	CPool<T>::CPool( size_t capacity)
		: m_numActive( 0),
		  m_maxTotal( 0)
	{
		m_container.reserve( capacity);
	}

	template<class T>
	T& CPool<T>::operator[]( size_t index)
	{
		return m_container[index];
	}
	
	template<class T>
	const T& CPool<T>::operator[]( size_t index) const
	{
		return m_container[index];
	}

	// 使不活跃
	template<class T>
	void CPool<T>::MakeInactive( size_t index)
	{
		if( index >= m_numActive)
			return;

		SwapElements( m_container[index], m_container[m_numActive-1]);
		--m_numActive;
	}

	// Adds an active element to this pool
	template<class T>
	bool CPool<T>::PushActive( T& element)
	{
		if( m_container.size() == m_container.capacity())
			return false;

		m_container.push_back( element);
		SwapElements( m_container[m_numActive], m_container.back());
		++m_numActive;

		if( m_container.size() > m_maxTotal)
			m_maxTotal = m_container.size();
	
		return true;
	}

	// actives the first inactive element
	template<class T>
	T* CPool<T>::MakeActive()
	{
		if( GetNumInactive()==0)
			return NULL;

		++m_numActive;

		return &m_container[0] + m_numActive - 1;
	}

	// Gets the number of inactive elements in this pool
	template<class T>
	inline size_t CPool<T>::GetNumInactive() const
	{
		return m_container.size() - m_numActive;
	}
}