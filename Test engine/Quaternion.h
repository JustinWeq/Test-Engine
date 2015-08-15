#pragma once
#include <math.h>
namespace JR_Quaternion
{
	//Quaternion--a simple structs the contains quaternion informations
	struct Quaternion
	{
	public:
		//x- the x position of the quaternion
		float x;
		//y- the y position of the quaternion
		float y;
		//z- the z position of the quaternion
		float z;
		//w- the width of the quaternion
		float w;
	};

	//length-- returns the length of the quaternion
	//quaternion- the quaternion to calculate the length for
	float length(Quaternion quaternion);

	//normalize--returns the passed in quaternion normalized
	//quaternion- the quaternion to calulate the normal for
	Quaternion normalize(Quaternion quaternion);

	//conjugate--conjugates and returns the passed in quaternion
	//quaternion- the quaternion to calculate the conjugate for
	Quaternion conjugate(Quaternion quaternion);

	//multiply-- multiplys the two passed in quaternions
	//a- the first qaternion
	//d- the second quaternion
	Quaternion multiply(Quaternion a, Quaternion b);
}