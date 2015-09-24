#include "Math.h"

namespace JR_Math
{
	bool quickHull(D3DXVECTOR3* inVertices, D3DXVECTOR3*& outVertices, int numOfVertices, int& numOfHullVertices)
	{
		float maxX = 0, minX = 0;
		float maxY = 0, minY = 0;
		float maxZ = 0, minZ = 0;

		D3DXVECTOR3 extremePoints[6];
		//find the six extreme points
		//loop through all points in the passed in list of points
		for (int i = 0;i < numOfVertices;i++)
		{
			//test max x
			if (inVertices[i].x > maxX)
			{
				//max x point found
				extremePoints[0] = inVertices[i];
				//continue searching for other extreme points
			}

			//test min x
			if (inVertices[i].x < minX)
			{
				//min x point found
				extremePoints[1] = inVertices[i];
				//continue searching for other extreme points
			}

			//test max y
			if (inVertices[i].y > maxY)
			{
				//max y point found
				extremePoints[2] = inVertices[i];
				//continue searching for other extreme points
			}

			//test min y
			if (inVertices[i].y < minY)
			{
				//min y point found
				extremePoints[3] = inVertices[i];
				//continue searching for other extreme points
			}

			//test max z
			if (inVertices[i].z > maxZ)
			{
				//max z point found
				extremePoints[4] = inVertices[i];
				//continue searching for other extreme points
			}

			//test min z
			if (inVertices[i].z > minZ)
			{
				//min point found
				extremePoints[5] = inVertices[i];
				//continue searching for other extreme points
			}

		}
		return true;
	}
}