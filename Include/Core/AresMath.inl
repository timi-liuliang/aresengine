#pragma once

namespace Ares
{
	// Vector4 to Vector3
	FORCEINLINE Vector3::Vector3( const Vector4& v)
		: x( v.x), y( v.y), z( v.z)
	{ }

	// 单位化
	FORCEINLINE UBOOL Vector3::Normalize( FLOAT tolerance)
	{
		const FLOAT squareSum = x*x + y*y + z*z;
		if( squareSum > tolerance)
		{
			const FLOAT scale = appInvSqrt( squareSum);
			x *= scale; y *= scale; z *= scale;

			return TRUE;
		}

		x = FLT_SMALL; y=0.f; z=0.f;
		//A_ASSERT( FALSE);

		return FALSE;
	}

	// Constructors
	FORCEINLINE Matrix44::Matrix44()
	{

	}

	// Transform a location
	FORCEINLINE Vector3 Matrix44::TransformVector3( const Vector3& v) const
	{
		return TransformVector4( Vector4( v.x, v.y, v.z, 1.f));
	}

	// Homogeneous transform
	FORCEINLINE Vector4 Matrix44::TransformVector4( const Vector4& v) const
	{
		Vector4 result;

	#if ASM_X86
	#ifdef _MSC_VER
		__asm
		{
			mov		eax,[v]
			mov		ecx,[this]

			movups	xmm4,[ecx]			// M[0][0]
			movups	xmm5,[ecx+16]		// M[1][0]
			movups	xmm6,[ecx+32]		// M[2][0]
			movups	xmm7,[ecx+48]		// M[3][0]

			movss	xmm0,[eax]FVector.X
				shufps	xmm0,xmm0,0
				mulps	xmm0,xmm4

				movss	xmm1,[eax]FVector.Y
				shufps	xmm1,xmm1,0
				mulps	xmm1,xmm5

				movss	xmm2,[eax]FVector.Z
				shufps	xmm2,xmm2,0
				mulps	xmm2,xmm6

				addps	xmm0,xmm1

				movss	xmm3,[eax]FVector4.W
				shufps	xmm3,xmm3,0
				mulps	xmm3,xmm7

				// stall
				lea		eax,[Result]

			addps	xmm2,xmm3

				// stall

				addps	xmm0,xmm2

				movups	[eax],xmm0
		}
	#else
		#error Please implement for your compiler.
	#endif

	#else
		result.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0];
		result.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1];
		result.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2];
		result.w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3];
	#endif

		return result;
	}

	// From Matrix
	FORCEINLINE void Quat::FromMatrix( const Matrix44& mat)
	{
		// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
		// article "Quaternion Calculus and Fast Animation"
		const int next[3] = { 1, 2, 0 };

		float trace = mat[0][0] + mat[1][1] + mat[2][2];
		float root;

		if( trace>0.f)
		{
			// |w|>1/2, may as well choose w>1/2
			root = appSqrt( trace + 1.f);
			w	 = 0.5f * root;
			root = 0.5f / root;
			x	 = (mat[1][2] - mat[2][1]) * root;
			y	 = (mat[2][0] - mat[0][2]) * root;
			z	 = (mat[0][1] - mat[1][0]) * root;
		}
		else
		{
			// |w| <= 1/2
			int i=0;
			if( mat[1][1] > mat[0][0])
				i = 1;

			if( mat[2][2] > mat[i][i])
				i = 2;

			int j = next[i];
			int k = next[j];

			root = appSqrt( mat[i][i] - mat[j][j] - mat[k][k] + 1.f);
		
			FLOAT qt[4];
			qt[i] = 0.5f * root;
			root  = 0.5f / root;
			qt[3] = ( mat[j][k] - mat[k][j]) * root;
			qt[j] = ( mat[i][j] + mat[j][i]) * root;
			qt[k] = ( mat[i][k] + mat[k][i]) * root;

			x = qt[0];
			y = qt[1];
			z = qt[2];
			w = qt[3];
		}
	}

	// From Euler angles
	FORCEINLINE void Quat::FromEuler( const Vector3& euler)
	{
		Vector3 halfAngle = euler * 0.5f;
		float	sin1	  = sin( halfAngle.x);
		float	sin2	  = sin( halfAngle.y);
		float	sin3	  = sin( halfAngle.z);
		float   cos1	  = cos( halfAngle.x);
		float   cos2	  = cos( halfAngle.y);
		float   cos3	  = cos( halfAngle.z);

		w = sin1*cos2*cos3 - sin1*sin2*sin3;
		x = sin1*sin2*cos3 + cos1*cos2*sin3;
		y = cos1*sin2*cos3 - sin1*cos2*sin3;
		z = sin1*cos2*cos3 + cos1*sin2*sin3;
	}

	// From Axis angle
	FORCEINLINE void Quat::FromAxisAngle( const Vector3& axis, FLOAT angle)
	{
		float halfA    = 0.5f * angle;
		float sinValue = sin( halfA);
		float cosValue = cos( halfA);

		x = sinValue * axis.x;
		y = sinValue * axis.y;
		z = sinValue * axis.z;
		w = cosValue;
	}

	// From Arc
	FORCEINLINE void Quat::FromArc( Vector3 begin, Vector3 end)
	{
		begin.Normalize();
		end.Normalize();

		Vector3 c = Vector3Cross( end, begin);		// 叉积
		float	d = Vector3Dot( begin, end);		// 点积
		float   s = appInvSqrt( ( 1.f+d)*2.f);

		x = c.x * s;
		y = c.y * s;
		z = c.z * s;
		w = 0.5f/ s;
	}

	// Transform to Matrix
	FORCEINLINE void Quat::ToMatrix( Matrix44& mat) const
	{
		mat = Matrix44::Identity;

		FLOAT twoX  =  2.f * x;
		FLOAT twoY  =  2.f * y;
		FLOAT twoZ  =  2.f * z;
		FLOAT twoWX = twoX * w;
		FLOAT twoWY = twoY * w;
		FLOAT twoWZ = twoZ * w;
		FLOAT twoXX = twoX * x;
		FLOAT twoXY = twoY * x;
		FLOAT twoXZ = twoZ * x;
		FLOAT twoYY = twoY * y;
		FLOAT twoYZ = twoZ * y;
		FLOAT twoZZ = twoZ * z;

		mat[0][0] = 1.f - (twoYY + twoZZ);
		mat[0][1] = twoXY - twoWZ;
		mat[0][2] = twoXZ + twoWY;
		mat[1][0] = twoXY + twoWZ;
		mat[1][1] = 1.f - (twoXX + twoZZ);
		mat[1][2] = twoYZ - twoWX;
		mat[2][0] = twoXZ - twoWY;
		mat[2][1] = twoYZ + twoWX;
		mat[2][2] = 1.f - (twoXX + twoYY);
	}

	// Transform to Euler
	//FORCEINLINE void Quat::ToEuler( Vector3& euler)
	//{
	//	euler.x = std::atan2f( 2*(w*z+x*y), 1-2*(z*z+x*x));
	//	euler.y = std::asinf(  2*(w*x-y*z));
	//	euler.z = std::atan2f( 2*(w*y+z*x), 1-2*(x*x+y*y));
	//}

	// 重载运算符 "*="
	//FORCEINLINE Quat Quat::operator*=(const Quat& Q)
	//{
	//	// 40% faster version of the Quaternion multiplication.
	//	const FLOAT T0 = (z - y) * (Q.y - Q.z);
	//	const FLOAT T1 = (w + x) * (Q.w + Q.x);
	//	const FLOAT T2 = (w - x) * (Q.y + Q.z);
	//	const FLOAT T3 = (y + z) * (Q.w - Q.x);
	//	const FLOAT T4 = (z - x) * (Q.x - Q.y);
	//	const FLOAT T5 = (z + x) * (Q.x + Q.y);
	//	const FLOAT T6 = (w + y) * (Q.w - Q.z);
	//	const FLOAT T7 = (w - y) * (Q.w + Q.z);
	//	const FLOAT T8 = T5 + T6 + T7;
	//	const FLOAT T9 = 0.5f * (T4 + T8);

	//	x = T1 + T9 - T8;
	//	y = T2 + T9 - T7;
	//	z = T3 + T9 - T6;
	//	w = T0 + T9 - T5;

	//	return *this;
	//}

	// 旋转顶点
	FORCEINLINE Vector3 Quat::Rotate( const Vector3& v) const
	{
		//Quat qv( v.x, v.y, v.z, 0.f);
		//qv = ~*this * qv * *this;

		//return Vector3( qv.x, qv.y, qv.z);

		// Nvidia SDK implementation
		///Vector3 uv, uuv;
		//Vector3 qvec( x, y, z);
		//uv  = Vector3Cross( qvec, v);
		//uuv = Vector3Cross( qvec, uv);
		//uv *= ( 2.f * w);
		//uuv*= 2.f;

		//return v+uv+uuv;

		// Unreal implementation
		Vector3 qv( x, y, z);
		Vector3 ov = 2.f * w * Vector3Cross( qv, v);
		ov += ( w*w - Vector3Dot( qv, qv)) * v;
		ov += ( 2.f * Vector3Dot( qv, v))  * qv;

		return ov;
	}

	// 单位化
	FORCEINLINE void Quat::Normalize()
	{
		float invMag = 1.f /sqrtf( x*x + y*y + z*z + w*w);
		if( invMag>0)
			(*this) = (*this) * invMag;
	}

	// 运算符重载 "*"
	FORCEINLINE Quat operator * ( const Quat& a, const Quat& b)
	{
		// NOTE:  Multiplication is not generally commutative, so in most
		// cases p*q != q*p.
		return Quat
		(
			a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
			a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
			a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
			a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
		);
	}

	// 四元数点积
	FORCEINLINE float QuatDot( const Quat& a, const Quat& b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
	}

	// 四元数线性插值
	FORCEINLINE Quat QuatLerp( const Quat& from, const Quat& to, float percentage)
	{
		Quat r = from + ( to - from) * percentage;
		r.Normalize();

		return r;
	}

	// 四元数插值
	FORCEINLINE Quat QuatSlerp( const Quat& from, const Quat& to, float percentage)
	{
		Quat  q3;
		float dot = QuatDot( from, to);

		// dot = cos(theta)
		// if(dot<0), q1 and q2 are more than 90 degrees apart, so we can invert one to
		// reduce spinning
		if( dot<0)
		{
			dot = -dot;
			q3   = to * -1.f;
		}
		else
		{
			q3 = to;
		}

		if( dot < 0.95f)
		{
			float angle   = acos( dot);
			float sina    = sin( angle);
			float sinat   = sin( angle*percentage);
			float sinaomt = sin( angle*(1.f-percentage));

			return ( from * sinaomt + q3*sinat) * ( 1.f/sina);
		}
		else
		{
			return QuatLerp( from, q3, percentage);
		}
	}

	// GetTranspose
	FORCEINLINE Matrix44 Matrix44::GetTranspose() const
	{
		Matrix44 result;

		result.m[0][0] = m[0][0];
		result.m[0][1] = m[1][0];
		result.m[0][2] = m[2][0];
		result.m[0][3] = m[3][0];

		result.m[1][0] = m[0][1];
		result.m[1][1] = m[1][1];
		result.m[1][2] = m[2][1];
		result.m[1][3] = m[3][1];

		result.m[2][0] = m[0][2];
		result.m[2][1] = m[1][2];
		result.m[2][2] = m[2][2];
		result.m[2][3] = m[3][2];

		result.m[3][0] = m[0][3];
		result.m[3][1] = m[1][3];
		result.m[3][2] = m[2][3];
		result.m[3][3] = m[3][3];

		return result;
	}

	// BOOL isIdentity
	FORCEINLINE BOOL Matrix44::IsIdentity() const
	{
		return  m[0][0] == 1.0f && m[0][1] == 0.0f && m[0][2] == 0.0f && m[0][3] == 0.0f &&
				m[1][0] == 0.0f && m[1][1] == 1.0f && m[1][2] == 0.0f && m[1][3] == 0.0f &&
				m[2][0] == 0.0f && m[2][1] == 0.0f && m[2][2] == 1.0f && m[2][3] == 0.0f &&
				m[3][0] == 0.0f && m[3][1] == 0.0f && m[3][2] == 0.0f && m[3][3] == 1.0f;
	}

	// Set Scaling
	FORCEINLINE void Matrix44Scaling( Matrix44& result, FLOAT scale)
	{
		result.m[0][0] = scale; result.m[0][1] =	 0;  result.m[0][2] =	  0;  result.m[0][3] = 0;
		result.m[1][0] =	 0; result.m[1][1] = scale;  result.m[1][2] =	  0;  result.m[1][3] = 0;
		result.m[2][0] =	 0; result.m[2][1] =	 0;  result.m[2][2] = scale;  result.m[2][3] = 0;
		result.m[3][0] =	 0; result.m[3][1] =     0;  result.m[3][2] =	  0;  result.m[3][3] = 1;
	}

	FORCEINLINE void Matrix44Scaling( Matrix44& result, const Vector3& scale)
	{
		result.m[0][0] = scale.x; result.m[0][1] =	     0;  result.m[0][2] =	    0;  result.m[0][3] = 0;
		result.m[1][0] =	   0; result.m[1][1] = scale.y;  result.m[1][2] =	    0;  result.m[1][3] = 0;
		result.m[2][0] =	   0; result.m[2][1] =	     0;  result.m[2][2] = scale.z;  result.m[2][3] = 0;
		result.m[3][0] =	   0; result.m[3][1] =       0;  result.m[3][2] =	    0;  result.m[3][3] = 1;
	}

	FORCEINLINE void Matrix44Scaling( Matrix44& result, FLOAT sx, FLOAT sy, FLOAT sz)
	{
		result.m[0][0] = sx; result.m[0][1] =  0;  result.m[0][2] =  0;  result.m[0][3] = 0;
		result.m[1][0] =  0; result.m[1][1] = sy;  result.m[1][2] =  0;  result.m[1][3] = 0;
		result.m[2][0] =  0; result.m[2][1] =  0;  result.m[2][2] = sz;  result.m[2][3] = 0;
		result.m[3][0] =  0; result.m[3][1] =  0;  result.m[3][2] =  0;  result.m[3][3] = 1;
	}

	// Set Translation
	FORCEINLINE void Matrix44Translation( Matrix44& result, FLOAT xOffset, FLOAT yOffset, FLOAT zOffset)
	{
		result.m[0][0] =  1; result.m[0][1] =  0;  result.m[0][2] =  0;  result.m[0][3] = 0;
		result.m[1][0] =  0; result.m[1][1] =  1;  result.m[1][2] =  0;  result.m[1][3] = 0;
		result.m[2][0] =  0; result.m[2][1] =  0;  result.m[2][2] =  1;	 result.m[2][3] = 0;
		result.m[3][0] =  xOffset; 
		result.m[3][1] =  yOffset;  
		result.m[3][2] =  zOffset;  
		result.m[3][3] =  1;
	}

	// Set Translation
	FORCEINLINE void Matrix44Translation( Matrix44& result, const Vector3& offset)
	{
		Matrix44Translation( result, offset.x, offset.y, offset.z);
	}

	// 重载"[]" 
	FORCEINLINE Vector4& Matrix44::operator[]( INT i)
	{
		A_ASSERT( i>=0);
		A_ASSERT( i<4);

		return (Vector4&)(*m[i]);
	}

	// 重载"[]" 
	FORCEINLINE const Vector4& Matrix44::operator[]( INT i) const
	{
		A_ASSERT( i>=0);
		A_ASSERT( i<4);

		return (const Vector4&)(*m[i]);
	}

	// 重载运算符"+"
	FORCEINLINE Matrix44 Matrix44::operator + ( const Matrix44& mat) const
	{
		return Matrix44(_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
						_21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
						_31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
						_41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44);
	}

	// 重载运算符"+="
	FORCEINLINE Matrix44 Matrix44::operator += ( const Matrix44& mat)
	{
		_11 += mat._11, _12 += mat._12, _13 += mat._13, _14 += mat._14;
		_21 += mat._21, _22 += mat._22, _23 += mat._23, _24 += mat._24;
		_31 += mat._31, _32 += mat._32, _33 += mat._33, _34 += mat._34;
		_41 += mat._41, _42 += mat._42, _43 += mat._43, _44 += mat._44;

		return *this;
	}

	// 重载运算符"-"
	FORCEINLINE Matrix44 Matrix44::operator - ( const Matrix44& mat) const
	{
		return Matrix44(_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
						_21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
						_31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
						_41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44);
	}

	// 重载运算符 "="
	FORCEINLINE Matrix44 Matrix44::operator * ( FLOAT f) const
	{
		return Matrix44(_11 * f, _12 * f, _13 * f, _14 * f,
						_21 * f, _22 * f, _23 * f, _24 * f,
						_31 * f, _32 * f, _33 * f, _34 * f,
						_41 * f, _42 * f, _43 * f, _44 * f);
	}

	// Matrix44ViewLH
	FORCEINLINE void Matrix44ViewLH( Matrix44& result, const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis, const Vector3& origin)
	{
		result.SetIdentity();

		result._11 = xAxis.x; result._12 = yAxis.x; result._13 = zAxis.x;
		result._21 = xAxis.y; result._22 = yAxis.y; result._23 = zAxis.y;
		result._31 = xAxis.z; result._32 = yAxis.z; result._33 = zAxis.z;
		result._41 = -Vector3Dot( xAxis, origin);
		result._42 = -Vector3Dot( yAxis, origin); 
		result._43 = -Vector3Dot( zAxis, origin);
	}
}