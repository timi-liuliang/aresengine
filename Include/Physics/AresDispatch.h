#pragma once

namespace Ares
{
	struct DispatcherInfo
	{
		float	m_timeStep;
		int		m_stepCount;
		int		m_dispatchFunc;
		float	m_timeOfImpact;
		bool	m_useContinuous;
		bool	m_enableSatConvex;
		bool	m_enableSPU;
		bool	m_useEpa;
		float	m_allowedCcdPenetration;
		bool	m_useConvexConservativeDistanceUtil;
		float	m_convexConservativeDistanceThreshold;

		enum DispatchFunc
		{
			Dispatch_Discrete = 1,
			Dispatch_continuous,
		};

		DispatcherInfo()
			: m_timeStep( 0.f)
			, m_stepCount( 0)
			, m_dispatchFunc( Dispatch_Discrete)
			, m_timeOfImpact( 1.f)
			, m_useContinuous( true)
			, m_enableSatConvex( false)
			, m_enableSPU( true)
			, m_useEpa( true)
			, m_allowedCcdPenetration( 0.04f)
			, m_useConvexConservativeDistanceUtil( false)
			, m_convexConservativeDistanceThreshold( 0.f)
		{}
	};

	//---------------------------------
	// Dispatcher 2013-9-12 ╣шаж
	// The dispatch interface class can be used
	// in combination with broadphase to dispatch
	// calculations for overlapping pairs. For
	// example for pairwise collision detection,
	// calculating contact points stored int
	// persistentManifold or user callbacks
	//---------------------------------
	class Dispatcher
	{
	public:
		virtual ~Dispatcher();


	};
}