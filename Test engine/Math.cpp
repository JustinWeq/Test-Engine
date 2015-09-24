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

		//temporaly return extreme points
		memcpy(outVertices, extremePoints, 6);
		return true;
		//all extreme points found,
		// out of the extreme points fin the two that have the most distance

		float greatestDistance = 0.0f;
		D3DXVECTOR3 startSimplex[4];
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
					startSimplex[0] = extremePoints[i];
					startSimplex[1] = extremePoints[j];
				}
			}
		}

		//now that the two points with the greatest distance have been
		// found, find the most distance point from the line of the two
		// greatest distance point

		greatestDistance = 0;
		for (int i = 0;i < 6;i++)
		{
			float d = distanceFromLine(startSimplex[0], startSimplex[1], extremePoints[i]);

			//if the distance is greater then the greatest found distance make it the greatest distance point
			
			if (d > greatestDistance)
			{
				startSimplex[3] = extremePoints[i];
				greatestDistance = d;
			}

		}

		//now that the first three points are found find the greatest
		// distance point from all of the points from the triangle 
		// to form the starting pyramid simplex

		



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

	//distanceFromLine-- returns the passed in points distance from
	// the line defined be two passed in points, returns exact distance
	//lineP1- the first point on the line
	//lineP2- the second point in the line
	//p1- the point to get the distance from the line from
	float distanceFromLine(D3DXVECTOR3 lineP1, D3DXVECTOR3 lineP2, D3DXVECTOR3 p1)
	{
		float x, x1, x2, x3, x4;
		float y, y1, y2, y3, y4;
		float z, z1, z2, z3, z4;

		x = p1.x;
		y = p1.y;
		z = p1.z;

		x1 = lineP1.x;
		y1 = lineP1.y;
		z1 = lineP1.z;

		x2 = lineP2.x;
		y2 = lineP2.y;
		z2 = lineP2.z;




	}

	//crossProduct-- returns the cross product
	//p1- th first point to calculate the cross product with
	//p2- the second point to calculate the cross product with
	D3DXVECTOR3 crossProduct(D3DXVECTOR3 p1, D3DXVECTOR3 p2)
	{

	}
}