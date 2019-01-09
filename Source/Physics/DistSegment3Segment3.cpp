#include <Physics/DistSegment3Segment3.h>

namespace Ares
{
	// construction
	DistSegment3Segment3::DistSegment3Segment3( const Segment3& segment0, const Segment3& segment1)
		: m_segment0( segment0)
		, m_segment1( segment1)
	{
	}

	// static distance queries
	float DistSegment3Segment3::GetSquared()
	{
		Vector3 diff = m_segment0.m_orig - m_segment1.m_orig;
		float a01= Vector3Dot( -m_segment0.m_dir, m_segment1.m_dir);
		float b0 = Vector3Dot( diff, m_segment0.m_dir);
		float b1 = Vector3Dot( -diff, m_segment1.m_dir);
		float c  = diff.LengthSq();
		float det= std::abs( 1.f - a01*a01);
		float s0, s1, sqrDist, extDet0, extDet1, tmpS0, tmpS1;

		if (det >= ZERO_TOLERANCE)
		{
			// Segments are not parallel.
			s0 = a01*b1 - b0;
			s1 = a01*b0 - b1;
			extDet0 = m_segment0.m_extent*det;
			extDet1 = m_segment1.m_extent*det;

			if (s0 >= -extDet0)
			{
				if (s0 <= extDet0)
				{
					if (s1 >= -extDet1)
					{
						if (s1 <= extDet1)  // region 0 (interior)
						{
							// Minimum at interior points of segments.
							float invDet = ((float)1)/det;
							s0 *= invDet;
							s1 *= invDet;
							sqrDist = s0*(s0 + a01*s1 + ((float)2)*b0) + s1*(a01*s0 + s1 + ((float)2)*b1) + c;
						}
						else  // region 3 (side)
						{
							s1 = m_segment1.m_extent;
							tmpS0 = -(a01*s1 + b0);
							if (tmpS0 < -m_segment0.m_extent)
							{
								s0 = -m_segment0.m_extent;
								sqrDist = s0*(s0 - ((float)2)*tmpS0) +
									s1*(s1 + ((float)2)*b1) + c;
							}
							else if (tmpS0 <= m_segment0.m_extent)
							{
								s0 = tmpS0;
								sqrDist = -s0*s0 + s1*(s1 + ((float)2)*b1) + c;
							}
							else
							{
								s0 = m_segment0.m_extent;
								sqrDist = s0*(s0 - ((float)2)*tmpS0) +
									s1*(s1 + ((float)2)*b1) + c;
							}
						}
					}
					else  // region 7 (side)
					{
						s1 = -m_segment1.m_extent;
						tmpS0 = -(a01*s1 + b0);
						if (tmpS0 < -m_segment0.m_extent)
						{
							s0 = -m_segment0.m_extent;
							sqrDist = s0*(s0 - ((float)2)*tmpS0) +
								s1*(s1 + ((float)2)*b1) + c;
						}
						else if (tmpS0 <= m_segment0.m_extent)
						{
							s0 = tmpS0;
							sqrDist = -s0*s0 + s1*(s1 + ((float)2)*b1) + c;
						}
						else
						{
							s0 = m_segment0.m_extent;
							sqrDist = s0*(s0 - ((float)2)*tmpS0) +
								s1*(s1 + ((float)2)*b1) + c;
						}
					}
				}
				else
				{
					if (s1 >= -extDet1)
					{
						if (s1 <= extDet1)  // region 1 (side)
						{
							s0 = m_segment0.m_extent;
							tmpS1 = -(a01*s0 + b1);
							if (tmpS1 < -m_segment1.m_extent)
							{
								s1 = -m_segment1.m_extent;
								sqrDist = s1*(s1 - ((float)2)*tmpS1) +
									s0*(s0 + ((float)2)*b0) + c;
							}
							else if (tmpS1 <= m_segment1.m_extent)
							{
								s1 = tmpS1;
								sqrDist = -s1*s1 + s0*(s0 + ((float)2)*b0) + c;
							}
							else
							{
								s1 = m_segment1.m_extent;
								sqrDist = s1*(s1 - ((float)2)*tmpS1) +
									s0*(s0 + ((float)2)*b0) + c;
							}
						}
						else  // region 2 (corner)
						{
							s1 = m_segment1.m_extent;
							tmpS0 = -(a01*s1 + b0);
							if (tmpS0 < -m_segment0.m_extent)
							{
								s0 = -m_segment0.m_extent;
								sqrDist = s0*(s0 - ((float)2)*tmpS0) +
									s1*(s1 + ((float)2)*b1) + c;
							}
							else if (tmpS0 <= m_segment0.m_extent)
							{
								s0 = tmpS0;
								sqrDist = -s0*s0 + s1*(s1 + ((float)2)*b1) + c;
							}
							else
							{
								s0 = m_segment0.m_extent;
								tmpS1 = -(a01*s0 + b1);
								if (tmpS1 < -m_segment1.m_extent)
								{
									s1 = -m_segment1.m_extent;
									sqrDist = s1*(s1 - ((float)2)*tmpS1) +
										s0*(s0 + ((float)2)*b0) + c;
								}
								else if (tmpS1 <= m_segment1.m_extent)
								{
									s1 = tmpS1;
									sqrDist = -s1*s1 + s0*(s0 + ((float)2)*b0) + c;
								}
								else
								{
									s1 = m_segment1.m_extent;
									sqrDist = s1*(s1 - ((float)2)*tmpS1) +
										s0*(s0 + ((float)2)*b0) + c;
								}
							}
						}
					}
					else  // region 8 (corner)
					{
						s1 = -m_segment1.m_extent;
						tmpS0 = -(a01*s1 + b0);
						if (tmpS0 < -m_segment0.m_extent)
						{
							s0 = -m_segment0.m_extent;
							sqrDist = s0*(s0 - ((float)2)*tmpS0) +
								s1*(s1 + ((float)2)*b1) + c;
						}
						else if (tmpS0 <= m_segment0.m_extent)
						{
							s0 = tmpS0;
							sqrDist = -s0*s0 + s1*(s1 + ((float)2)*b1) + c;
						}
						else
						{
							s0 = m_segment0.m_extent;
							tmpS1 = -(a01*s0 + b1);
							if (tmpS1 > m_segment1.m_extent)
							{
								s1 = m_segment1.m_extent;
								sqrDist = s1*(s1 - ((float)2)*tmpS1) +
									s0*(s0 + ((float)2)*b0) + c;
							}
							else if (tmpS1 >= -m_segment1.m_extent)
							{
								s1 = tmpS1;
								sqrDist = -s1*s1 + s0*(s0 + ((float)2)*b0) + c;
							}
							else
							{
								s1 = -m_segment1.m_extent;
								sqrDist = s1*(s1 - ((float)2)*tmpS1) +
									s0*(s0 + ((float)2)*b0) + c;
							}
						}
					}
				}
			}
			else 
			{
				if (s1 >= -extDet1)
				{
					if (s1 <= extDet1)  // region 5 (side)
					{
						s0 = -m_segment0.m_extent;
						tmpS1 = -(a01*s0 + b1);
						if (tmpS1 < -m_segment1.m_extent)
						{
							s1 = -m_segment1.m_extent;
							sqrDist = s1*(s1 - ((float)2)*tmpS1) +
								s0*(s0 + ((float)2)*b0) + c;
						}
						else if (tmpS1 <= m_segment1.m_extent)
						{
							s1 = tmpS1;
							sqrDist = -s1*s1 + s0*(s0 + ((float)2)*b0) + c;
						}
						else
						{
							s1 = m_segment1.m_extent;
							sqrDist = s1*(s1 - ((float)2)*tmpS1) +
								s0*(s0 + ((float)2)*b0) + c;
						}
					}
					else  // region 4 (corner)
					{
						s1 = m_segment1.m_extent;
						tmpS0 = -(a01*s1 + b0);
						if (tmpS0 > m_segment0.m_extent)
						{
							s0 = m_segment0.m_extent;
							sqrDist = s0*(s0 - ((float)2)*tmpS0) +
								s1*(s1 + ((float)2)*b1) + c;
						}
						else if (tmpS0 >= -m_segment0.m_extent)
						{
							s0 = tmpS0;
							sqrDist = -s0*s0 + s1*(s1 + ((float)2)*b1) + c;
						}
						else
						{
							s0 = -m_segment0.m_extent;
							tmpS1 = -(a01*s0 + b1);
							if (tmpS1 < -m_segment1.m_extent)
							{
								s1 = -m_segment1.m_extent;
								sqrDist = s1*(s1 - ((float)2)*tmpS1) +
									s0*(s0 + ((float)2)*b0) + c;
							}
							else if (tmpS1 <= m_segment1.m_extent)
							{
								s1 = tmpS1;
								sqrDist = -s1*s1 + s0*(s0 + ((float)2)*b0) + c;
							}
							else
							{
								s1 = m_segment1.m_extent;
								sqrDist = s1*(s1 - ((float)2)*tmpS1) +
									s0*(s0 + ((float)2)*b0) + c;
							}
						}
					}
				}
				else   // region 6 (corner)
				{
					s1 = -m_segment1.m_extent;
					tmpS0 = -(a01*s1 + b0);
					if (tmpS0 > m_segment0.m_extent)
					{
						s0 = m_segment0.m_extent;
						sqrDist = s0*(s0 - ((float)2)*tmpS0) +
							s1*(s1 + ((float)2)*b1) + c;
					}
					else if (tmpS0 >= -m_segment0.m_extent)
					{
						s0 = tmpS0;
						sqrDist = -s0*s0 + s1*(s1 + ((float)2)*b1) + c;
					}
					else
					{
						s0 = -m_segment0.m_extent;
						tmpS1 = -(a01*s0 + b1);
						if (tmpS1 < -m_segment1.m_extent)
						{
							s1 = -m_segment1.m_extent;
							sqrDist = s1*(s1 - ((float)2)*tmpS1) +
								s0*(s0 + ((float)2)*b0) + c;
						}
						else if (tmpS1 <= m_segment1.m_extent)
						{
							s1 = tmpS1;
							sqrDist = -s1*s1 + s0*(s0 + ((float)2)*b0) + c;
						}
						else
						{
							s1 = m_segment1.m_extent;
							sqrDist = s1*(s1 - ((float)2)*tmpS1) +
								s0*(s0 + ((float)2)*b0) + c;
						}
					}
				}
			}
		}
		else
		{
			// The segments are parallel.  The average b0 term is designed to
			// ensure symmetry of the function.  That is, dist(seg0,seg1) and
			// dist(seg1,seg0) should produce the same number.
			float e0pe1 = m_segment0.m_extent + m_segment1.m_extent;
			float sign = (a01 > (float)0 ? (float)-1 : (float)1);
			float b0Avr = ((float)0.5)*(b0 - sign*b1);
			float lambda = -b0Avr;
			if (lambda < -e0pe1)
			{
				lambda = -e0pe1;
			}
			else if (lambda > e0pe1)
			{
				lambda = e0pe1;
			}

			s1 = -sign*lambda*m_segment1.m_extent/e0pe1;
			s0 = lambda + sign*s1;
			sqrDist = lambda*(lambda + ((float)2)*b0Avr) + c;
		}

		m_closePointSegment0 = m_segment0.m_orig + s0*m_segment0.m_dir;
		m_closePointSegment1 = m_segment1.m_orig + s1*m_segment1.m_dir;
		m_segment0Param = s0;
		m_segment1Param = s1;

		// Account for numerical round-off errors.
		if (sqrDist < (float)0)
		{
			sqrDist = (float)0;
		}
		return sqrDist;
	}
}