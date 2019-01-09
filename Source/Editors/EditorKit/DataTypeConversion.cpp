#include "DataTypeConversion.h"

namespace Ares
{
	namespace DTC
	{
		// Vector3 ת string
		string Vector3ToString( const Vector3& vec)
		{
			char result[512];

			sprintf_s( result, 512, "%.3f,%.3f,%.3f", vec.x, vec.y, vec.z);

			return result;
		}

		// string ת Vector3
		Vector3 StringToVector3( const string& str)
		{
			float x, y, z;

			sscanf_s( str.c_str(), "%f,%f,%f", &x, &y, &z);

			return Vector3( x, y, z);
		}

		// ColorRGB 转 string
		string ColorRGBToString( const ColorRGB& color)
		{
			char result[512];

			sprintf_s( result, 512, "%f,%f,%f", color.r, color.g, color.b);

			return result;
		}

		// string 转 ColorRGB
		ColorRGB StringToColorRGB( const string& str)
		{
			float r, g, b;

			sscanf_s( str.c_str(), "%f,%f,%f", &r, &g, &b);

			return ColorRGB( r, g, b);
		}
	}
}