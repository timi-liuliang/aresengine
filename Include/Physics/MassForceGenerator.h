#ifndef MASS_FORCE_GENERATOR_H
#define MASS_FORCE_GENERATOR_H

namespace Ares
{
	// Declaration
	class MassPoint;

	// MassForceGenerator : A force generator can be asked to add a force to one or more mass point
	struct MassForce
	{
		virtual void UpdateForce( MassPoint& mp, float duration)=0;
	};

	// MassSpring: A force generator that applies a spring force
	struct MassSpring : public MassForce
	{

	};
}

#endif