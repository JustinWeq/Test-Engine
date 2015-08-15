#include "Quaternion.h"

namespace JR_Quaternion
{
	//length-- returns the length of the quaternion
	//quaternion- the quaternion to calculate the length for
	float length(Quaternion quaternion)
	{
		return sqrtf(quaternion.x* quaternion.x + quaternion.y*quaternion.w +
			quaternion.z * quaternion.z + quaternion.w*quaternion.w);
	}

	//normalize--returns the passed in quaternion normalized
	//quaternion- the quaternion to calulate the normal for
	Quaternion normalize(Quaternion quaternion)
	{
		float Length = length(quaternion);

		quaternion.x /= Length;
		quaternion.y /= Length;
		quaternion.z /= Length;
		quaternion.z /= Length;

		return quaternion;
	}

	//conjugate--conjugates and returns the passed in quaternion
	//quaternion- the quaternion to calculate the conjugate for
	Quaternion conjugate(Quaternion quaternion)
	{
		quaternion.x = -quaternion.x;
		quaternion.y = -quaternion.y;
		quaternion.z = -quaternion.z;
		return quaternion;
	}

	//multiply-- multiplys the two passed in quaternions
	//a- the first qaternion
	//d- the second quaternion
	Quaternion multiply(Quaternion a, Quaternion b)
	{
		Quaternion c;
		c.x = a.w*b.x + a.x*b.x + a.y*b.z - a.z*b.y;
		c.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
		c.z = a.w*b.z + a.z*b.y - a.y*b.x + a.z*b.w;
		c.w = a.w*b.w - a.x*b.x - a.y* b.y - a.z*b.z;

		return c;
	}
}