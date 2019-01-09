#include <Physics/DistVector3Triangle3.h>

namespace Ares
{
	// constructor
	DistVector3Triangle3::DistVector3Triangle3( const Vector3& vector, const Triangle3& triangle)
		: m_vector( vector),
		  m_triangle( triangle)
	{
	}

	// static distance queries
	float DistVector3Triangle3::GetSquared()
	{
		Vector3 kDiff  = m_triangle.m_v[0] - m_vector;
		Vector3 kEdge0 = m_triangle.m_v[1] - m_triangle.m_v[0];
		Vector3 kEdge1 = m_triangle.m_v[2] - m_triangle.m_v[0];

		float fA00 = kEdge0.LengthSq();
		float fA01 = Vector3Dot( kEdge0, kEdge1);
		float fA11 = kEdge1.LengthSq();
		float fB0  = Vector3Dot( kDiff, kEdge0);
		float fB1  = Vector3Dot( kDiff, kEdge1);
		float fC   = kDiff.LengthSq();
		float fDet = abs( fA00*fA11 - fA01*fA01);
		float fS   = fA01*fB1 - fA11*fB0;
		float fT   = fA01*fB0 - fA00*fB1;

		float fSqrDistance;

		if (fS + fT <= fDet)
		{
			if (fS < 0.f)
			{
				// region 4
				if (fT < 0.f)  
				{
					if (fB0 <  0.f)
					{
						fT = 0.f;
						if (-fB0 >= fA00)
						{
							fS = 1.f;
							fSqrDistance = fA00+(2.f)*fB0+fC;
						}
						else
						{
							fS = -fB0/fA00;
							fSqrDistance = fB0*fS+fC;
						}
					}
					else
					{
						fS = 0.f;
						if (fB1 >= 0.f)
						{
							fT = 0.f;
							fSqrDistance = fC;
						}
						else if (-fB1 >= fA11)
						{
							fT = 1.f;
							fSqrDistance = fA11+(2.f)*fB1+fC;
						}
						else
						{
							fT = -fB1/fA11;
							fSqrDistance = fB1*fT+fC;
						}
					}
				}
				else  // region 3
				{
					fS = 0.f;
					if (fB1 >= 0.f)
					{
						fT = 0.f;
						fSqrDistance = fC;
					}
					else if (-fB1 >= fA11)
					{
						fT = 1.f;
						fSqrDistance = fA11+(2.f)*fB1+fC;
					}
					else
					{
						fT = -fB1/fA11;
						fSqrDistance = fB1*fT+fC;
					}
				}
			}
			else if (fT < 0.f)  // region 5
			{
				fT = 0.f;
				if (fB0 >= 0.f)
				{
					fS = 0.f;
					fSqrDistance = fC;
				}
				else if (-fB0 >= fA00)
				{
					fS = 1.f;
					fSqrDistance = fA00+(2.f)*fB0+fC;
				}
				else
				{
					fS = -fB0/fA00;
					fSqrDistance = fB0*fS+fC;
				}
			}
			else  // region 0
			{
				// minimum at interior point
				float fInvDet = 1.f/fDet;
				fS *= fInvDet;
				fT *= fInvDet;
				fSqrDistance = fS*(fA00*fS+fA01*fT+(2.f)*fB0) +
					fT*(fA01*fS+fA11*fT+(2.f)*fB1)+fC;
			}
		}
		else
		{
			float fTmp0, fTmp1, fNumer, fDenom;

			if (fS < 0.f)  // region 2
			{
				fTmp0 = fA01 + fB0;
				fTmp1 = fA11 + fB1;
				if (fTmp1 > fTmp0)
				{
					fNumer = fTmp1 - fTmp0;
					fDenom = fA00-2.0f*fA01+fA11;
					if (fNumer >= fDenom)
					{
						fS = 1.f;
						fT = 0.f;
						fSqrDistance = fA00+(2.f)*fB0+fC;
					}
					else
					{
						fS = fNumer/fDenom;
						fT = 1.f - fS;
						fSqrDistance = fS*(fA00*fS+fA01*fT+2.0f*fB0) +
							fT*(fA01*fS+fA11*fT+(2.f)*fB1)+fC;
					}
				}
				else
				{
					fS = 0.f;
					if (fTmp1 <= 0.f)
					{
						fT = 1.f;
						fSqrDistance = fA11+(2.f)*fB1+fC;
					}
					else if (fB1 >= 0.f)
					{
						fT = 0.f;
						fSqrDistance = fC;
					}
					else
					{
						fT = -fB1/fA11;
						fSqrDistance = fB1*fT+fC;
					}
				}
			}
			else if (fT < 0.f)  // region 6
			{
				fTmp0 = fA01 + fB1;
				fTmp1 = fA00 + fB0;
				if (fTmp1 > fTmp0)
				{
					fNumer = fTmp1 - fTmp0;
					fDenom = fA00-(2.f)*fA01+fA11;
					if (fNumer >= fDenom)
					{
						fT = 1.f;
						fS = 0.f;
						fSqrDistance = fA11+(2.f)*fB1+fC;
					}
					else
					{
						fT = fNumer/fDenom;
						fS = 1.f - fT;
						fSqrDistance = fS*(fA00*fS+fA01*fT+(2.f)*fB0) +
							fT*(fA01*fS+fA11*fT+(2.f)*fB1)+fC;
					}
				}
				else
				{
					fT = 0.f;
					if (fTmp1 <= 0.f)
					{
						fS = 1.f;
						fSqrDistance = fA00+(2.f)*fB0+fC;
					}
					else if (fB0 >= 0.f)
					{
						fS = 0.f;
						fSqrDistance = fC;
					}
					else
					{
						fS = -fB0/fA00;
						fSqrDistance = fB0*fS+fC;
					}
				}
			}
			else  // region 1
			{
				fNumer = fA11 + fB1 - fA01 - fB0;
				if (fNumer <= 0.f)
				{
					fS = 0.f;
					fT = 1.f;
					fSqrDistance = fA11+(2.f)*fB1+fC;
				}
				else
				{
					fDenom = fA00-2.0f*fA01+fA11;
					if (fNumer >= fDenom)
					{
						fS = 1.f;
						fT = 0.f;
						fSqrDistance = fA00+(2.f)*fB0+fC;
					}
					else
					{
						fS = fNumer/fDenom;
						fT = 1.f - fS;
						fSqrDistance = fS*(fA00*fS+fA01*fT+(2.f)*fB0) +
							fT*(fA01*fS+fA11*fT+(2.f)*fB1)+fC;
					}
				}
			}
		}

		// account for numerical round-off error
		if (fSqrDistance < 0.f)
		{
			fSqrDistance = 0.f;
		}

		m_closePointVector = m_vector;
		m_closePointTriangle = m_triangle.m_v[0] + fS*kEdge0 + fT*kEdge1;
		m_triangleBary[1] = fS;
		m_triangleBary[2] = fT;
		m_triangleBary[0] = 1.f - fS - fT;

		return fSqrDistance;
	}

	// static distance queries
	float DistVector3Triangle3::Get()
	{
		float fSqrDist = GetSquared();

		return sqrtf( fSqrDist);
	}
}