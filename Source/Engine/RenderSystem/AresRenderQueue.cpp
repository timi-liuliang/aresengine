#include <Engine/AresRoot.h>
#include <Engine/RenderSystem/AresRenderQueue.h>

namespace Ares
{
	// 构造函数
	RenderQueue::RenderQueue()
	{

	}

	// 析构函数
	RenderQueue::~RenderQueue()
	{

	}

	// add renderable
	void RenderQueue::AddRenderable(  RenderLayoutPtr& layout, RenderMaterial* material)
	{
		if( layout && material)
			m_renderables.push_back( make_pair( layout, material));
	}

	//the function is responsive to executing the sorted renderable group
	void RenderQueue::Render( bool sort/*=true*/, bool clear/*=true*/)
	{
		// is exec sort
		if( sort)
			Sort();

		// render
		for ( size_t i=0; i<m_renderables.size(); i++)
		{
			ARenderDevice->Render( m_renderables[i].first.get(), m_renderables[i].second);
		}

		// is clear
		//if( clear)
		//	Clear();
	}

	//// SRenderEntry soring functor 
	//typedef SRenderEntry* LPRenderEntry;
	//struct sort_less
	//{
	//	// 重载 "()" 运算符
	//	bool operator()(const LPRenderEntry& a, const LPRenderEntry& b) const
	//	{
	//		// 首先排序值 A
	//		if( a->sortValueA > b->sortValueA)
	//		{
	//			return false;
	//		}
	//		else if( a->sortValueA < b->sortValueA)
	//		{
	//			return true;
	//		}

	//		// 排序值 B
	//		if( a->sortValueB > b->sortValueB)
	//		{
	//			return false;
	//		}
	//		else if( a->sortValueB < b->sortValueB)
	//		{
	//			return true;
	//		}

	//		// 排序值　C
	//		if( a->sortValueC > b->sortValueC)
	//		{
	//			return false;
	//		}
	//		else if( a->sortValueC < b->sortValueC)
	//		{
	//			return true;
	//		}

	//		return false;
	//	}
	//};


	// 排序
	void RenderQueue::Sort()
	{
		// 使用指针进行排序(而非排序真实的对象)
		//QuickSort( m_pEntryList, m_iActiveEntries, sort_less());
	}
}