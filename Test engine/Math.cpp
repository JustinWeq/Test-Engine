#include "Math.h"


	bool quickHull(D3DXVECTOR3* inVertices, D3DXVECTOR3* outVertices, int numOfVertices, int& numOfHullVertices)
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
				maxX = inVertices[i].x;
				//max x point found
				extremePoints[0] = inVertices[i];
				//continue searching for other extreme points
				continue;
			}

			//test min x
			if (inVertices[i].x < minX)
			{
				minX = inVertices[i].x;
				//min x point found
				extremePoints[1] = inVertices[i];
				//continue searching for other extreme points
				continue;
			}

			//test max y
			if (inVertices[i].y > maxY)
			{
				maxY = inVertices[i].y;
				//max y point found
				extremePoints[2] = inVertices[i];
				//continue searching for other extreme points
				continue;
			}

			//test min y
			if (inVertices[i].y < minY)
			{
				minY = inVertices[i].y;
				//min y point found
				extremePoints[3] = inVertices[i];
				//continue searching for other extreme points
				continue;
			}

			//test max z
			if (inVertices[i].z > maxZ)
			{
				maxZ = inVertices[i].z;
				//max z point found
				extremePoints[4] = inVertices[i];
				//continue searching for other extreme points
				continue;
			}

			//test min z
			if (inVertices[i].z > minZ)
			{
				minZ = inVertices[i].z;
				//min point found
				extremePoints[5] = inVertices[i];
				//continue searching for other extreme points
				continue;
			}
			
		}


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
				startSimplex[2] = extremePoints[i];
				greatestDistance = d;
			}

		}

		//now that the first three points are found find the greatest
		// distance point from all of the points from the triangle 
		// to form the starting pyramid simplex
		greatestDistance = 0;
		for (int i = 0;i < numOfVertices;i++)
		{
			float d = 0;
			d = distanceFromTrangle(inVertices[i], startSimplex[0], startSimplex[1], startSimplex[3]);
			if (d > greatestDistance)
			{
				greatestDistance = d;
				startSimplex[3] = inVertices[i];
			}
		}

		//temporaly return the starting simplex(pyramid)
		memcpy(outVertices, startSimplex, 4*sizeof(D3DXVECTOR3));
		return true;

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
		float x, x1, x2;
		float y, y1, y2;
		float z, z1, z2;

		x = p1.x;
		y = p1.y;
		z = p1.z;

		x1 = lineP1.x;
		y1 = lineP1.y;
		z1 = lineP1.z;

		x2 = lineP2.x;
		y2 = lineP2.y;
		z2 = lineP2.z;

		D3DXVECTOR3 AB = D3DXVECTOR3(x2-x1,y2-y1,z2-z1);

		D3DXVECTOR3 AP = D3DXVECTOR3(x-x1,y-y1,z-z1);

		D3DXVECTOR3 a = crossProduct(AB, AP);

		float A = sqrtf(powf(a.x, 2) + powf(a.y, 2) + powf(a.z, 2));

		float ab = sqrtf(powf(AB.x, 2) + powf(AB.y, 2) + powf(AB.z, 2));

		return (A / ab);

	}

	//crossProduct-- returns the cross product
	//p1- th first point to calculate the cross product with
	//p2- the second point to calculate the cross product with
	D3DXVECTOR3 crossProduct(D3DXVECTOR3 p1, D3DXVECTOR3 p2)
	{
		D3DXVECTOR3 product;
		product.x = ((p1.y*p2.z) - (p1.z*p2.y));
		product.y = (-((p1.x*p2.z) - (p2.x*p1.z)));
		product.z = ((p1.x*p2.y) - (p2.x*p1.y));

		return product;
	}

	//distanceFromTriangle-- returns the distance between a point and a triangle
	//P0- the point to get the distance from
	//P1- the first point int the triangle
	//P2- the second point in the triangle
	//P3- the third point in the triangle.
	float distanceFromTrangle(D3DXVECTOR3 P0, D3DXVECTOR3 P1, D3DXVECTOR3 P2, D3DXVECTOR3 P3)
	{
		//find averidge point in the triangle
		float sumX;
		float sumY;
		float sumZ;

		sumX = 0;
		sumY = 0;
		sumZ = 0;

		sumX = P1.x + P2.x + P3.x;
		sumY = P1.y + P2.y + P3.y;
		sumZ = P1.z + P2.z + P3.z;

		D3DXVECTOR3 averidgePoint = D3DXVECTOR3(sumX / 3, sumY / 3, sumZ / 3);

		//get the distance between the point and the triangle

		return distance(P0, averidgePoint);
	}
