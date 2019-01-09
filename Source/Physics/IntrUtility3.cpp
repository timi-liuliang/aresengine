#include <Core/AresMath.h>
#include <Physics/IntrUtility3.h>

namespace Ares
{
	// get triangle3 projection
	void IntrAxis::GetProjection( const Vector3& rkAxis, const Triangle3& rkTriangle, float& fMin, float& fMax)
	{
		float afDot[3] = 
		{
			Vector3Dot( rkAxis, rkTriangle.m_v[0]),
			Vector3Dot( rkAxis, rkTriangle.m_v[1]),
			Vector3Dot( rkAxis, rkTriangle.m_v[2])
		};

		fMin = afDot[0];
		fMax = fMin;

		if( afDot[1] < fMin)
			fMin = afDot[1];
		else if( afDot[1] > fMax)
			fMax = afDot[1];

		if( afDot[2] < fMin)
			fMin = afDot[2];
		else if( afDot[2] > fMax)
			fMax = afDot[2];
	}

	// projections
	void IntrAxis::GetProjection( const Vector3& rkAxis, const Box3& rkBox, float& fMin, float& fMax)
	{
		float fOrigin = Vector3Dot( rkAxis, rkBox.m_center);

		float fMaximumExtent =
			abs( rkBox.m_extent[0] * Vector3Dot( rkAxis, rkBox.m_axis[0])) +
			abs( rkBox.m_extent[1] * Vector3Dot( rkAxis, rkBox.m_axis[1])) +
			abs( rkBox.m_extent[2] * Vector3Dot( rkAxis, rkBox.m_axis[2]));

		fMin = fOrigin - fMaximumExtent;
		fMax = fOrigin + fMaximumExtent;
	}

	//-------------------------------------------------------------------------
	// Miscellaneous support
	// The input and output polygons are stored in akP. The size of akp is
	// assumed to be large enough to store the clipped convex polygon vertices.
	// For now the maximum array size is 8 to support the current intersection
	// algorithms.
	void ClipConvexPolygonAgainstPlane( const Vector3& rkNormal, float fConstant, int& rQuantity, Vector3* akp)
	{
		// The input vertices are assumed to be in counterclockwise order. The ordering is an
		// invariant of this function. The size of array akp is assumed to be large enough to 
		// store the clipped polygon vertices

		// test on which side of line are the vertices
		int iPositive = 0, iNegative = 0, iPIndex = -1;
		int iQuantity = rQuantity;

		float afTest[8];
		for ( int i=0; i<rQuantity; i++)
		{
			// an epsilon is used here because it is possible for the dot product
			// and fC to be exactly equal to each other( int theory), but differ
			// slightly because of floating point problems. Thus, add a little to
			// the test number to push actually equal numbers over the edge towards
			// the positive

			// TODO: This should probably be a relative tolerance. Multiplying by the
			// constant is probably not the best way to do this
			afTest[i] = Vector3Dot( rkNormal, akp[i]) - fConstant + abs( fConstant * FLT_TOLERANCE);

			if( afTest[i] >= 0.f)
			{
				iPositive++;
				if( iPIndex < 0)
				{
					iPIndex = i;
				}
			}
			else
			{
				iNegative++;
			}
		}

		if( rQuantity == 2)
		{
			// Lines are a little different. in that clipping the segment cannot create
			// a new segment, as clipping a apolygon would
			if( iPositive > 0)
			{
				if( iNegative > 0)
				{
					int iClip;

					if( iPIndex == 0)
					{
						// vertex0 positive, vertex1 is clipped
						iClip = 1;
					}
					else	
					{
						// IPIndex == 1
						// vertex1 positive, vertex0 clipped
						iClip = 0;
					}

					float fT = afTest[ iPIndex] / ( afTest[iPIndex] - afTest[iClip]);
					akp[iClip] = akp[iPIndex] + fT * ( akp[iClip] - akp[iPIndex]);
				}

				// otherwise both positive, no clipping
			}
			else
			{
				// Assert: The entrie line is clipped, but we should not get here
				rQuantity = 0;
			}
		}
		else
		{
			if( iPositive > 0)
			{
				if( iNegative > 0)
				{
					// plane transversely intersects polygon
					Vector3 akCV[8];
					int iCQuantity = 0, iCur, iPrv;
					float fT;

					if( iPIndex > 0)
					{
						// first clip vertex on line
						iCur = iPIndex;
						iPrv = iCur - 1;
						fT   = afTest[iCur] / ( afTest[iCur] - afTest[iPrv]);
						akCV[iCQuantity++] = akp[iCur] + fT * (akp[iPrv] - akp[iCur]);

						// vertices on positive side of line
						while ( iCur < iQuantity && afTest[iCur] >= 0.f)
						{
							akCV[iCQuantity++] = akp[iCur++];
						}

						// last clip vertex on line
						if( iCur < iQuantity)
						{
							iPrv = iCur - 1;
						}
						else
						{
							iCur = 0;
							iPrv = iQuantity - 1;
						}

						fT = afTest[iCur] / (afTest[iCur]-afTest[iPrv]);
						akCV[iCQuantity++] = akp[iCur] + fT * (akp[iPrv] - akp[iCur]);
					}
					else
					{
						// IPIndex is 0
						// vertices on positive side of line
						iCur = 0;
						while( iCur < iQuantity && afTest[iCur] >= 0.f)
						{
							akCV[iCQuantity++] = akp[iCur++];
						}

						// last clip vertex on line
						iPrv = iCur - 1;
						fT   = afTest[iCur] / (afTest[iCur] - afTest[iPrv]);
						akCV[iCQuantity++] = akp[iCur] + fT * (akp[iPrv] - akp[iCur]);

						// skip vertices on negative side
						while( iCur < iQuantity && afTest[iCur] < 0.f)
						{
							iCur++;
						}

						// first clip vertex on line
						if( iCur < iQuantity)
						{
							iPrv = iCur - 1;
							fT = afTest[iCur] / (afTest[iCur] - afTest[iPrv]);
							akCV[iCQuantity++] = akp[iCur] + fT * ( akp[iPrv] - akp[iCur]);

							// vertices on positive side of line
							while( iCur < iQuantity && afTest[iCur] >= 0.f)
							{
								akCV[iCQuantity++] = akp[iCur++];
							}
						}
						else
						{
							// iCur = 0;
							iPrv = iQuantity - 1;
							fT = afTest[0] / ( afTest[0]-afTest[iPrv]);
							akCV[iCQuantity++] = akp[0] + fT*( akp[iPrv] - akp[0]);
						}
					}

					iQuantity = iCQuantity;
					memcpy( akp, akCV, iCQuantity * sizeof( Vector3));
				}
					
				//else polygon fully on positive side of plane, nothing to do
				rQuantity = iQuantity;
			}
			else
			{
				// polygon does not intersect positive side of plane, clip all this should
				// not ever happen if called by the findintersect routines after an intersection
				// has been determined
				rQuantity = 0;
			}
		}
	}
}