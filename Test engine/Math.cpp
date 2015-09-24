#include "Math.h"

namespace JR_Math
{
	bool quickHull(D3DXVECTOR3* inVertices, D3DXVECTOR3*& outVertices, int numOfVertices, int& numOfHullVertices)
	{
		//check to see if a convex hull can even be found
		if (numOfVertices < 4)
		{
			//no 3D convex hull possible so return false
			return false;
		}

		//check to see if the convex hull is less then or equal to 6 and
		// if it is return it as the convex hull for the set of
		// vertices since no smaller hull can be found
		if (numOfVertices <= 6)
		{
			//set of points is the convex hull
			
			//copy inVertices memory tot he out vertices memory
			memcpy(outVertices, inVertices, numOfVertices);

			//return true since convex hull was found
			return true;
		}
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
				continue;
			}

			//test min x
			if (inVertices[i].x < minX)
			{
				//min x point found
				extremePoints[1] = inVertices[i];
				//continue searching for other extreme points
				continue;
			}

			//test max y
			if (inVertices[i].y > maxY)
			{
				//max y point found
				extremePoints[2] = inVertices[i];
				//continue searching for other extreme points
				continue;
			}

			//test min y
			if (inVertices[i].y < minY)
			{
				//min y point found
				extremePoints[3] = inVertices[i];
				//continue searching for other extreme points
				continue;
			}

			//test max z
			if (inVertices[i].z > maxZ)
			{
				//max z point found
				extremePoints[4] = inVertices[i];
				//continue searching for other extreme points
				continue;
			}

			//test min z
			if (inVertices[i].z > minZ)
			{
				//min point found
				extremePoints[5] = inVertices[i];
				//continue searching for other extreme points
				continue;
			}
			
		}
		//all extreme points found,
		// out of the extreme points fin the two that have the most distance

		float greatestDistance = 0.0f;
		D3DXVECTOR3 startLine[2];
		for (int i = 0;i < 6;i++)
		{
			for (int j = 0;j < 6;j++)
			{
				//measure distance for two points

				//check to make sure that the points does not compare itself
				if (i == j)
				{
					break;
				}
				float d = distance(extremePoints[i], extremePoints[j]);
				if (distance(extremePoints[i], extremePoints[j]) > greatestDistance)
				{
					greatestDistance = d;
					startLine[0] = extremePoints[i];
					startLine[2] = extremePoints[j];
				}
			}
		}

		//convex hull was found so return true;
		return true;
	}

	//distance-- returns the distance between two points in 3d space
	// distance is returned squared to save on processing time
	//p1- the first point to be considred in the distance
	//p2- the second point to be considred in the distance
	float distance(D3DXVECTOR3 p1, D3DXVECTOR3 p2)
	{
		return (powf(p1.x - p2.x, 2) + powf(p1.y - p2.y, 2) + powf(p1.z - p2.z, 2));
	}
}