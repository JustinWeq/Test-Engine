#pragma once

namespace JR_Math
{
#include <D3DX10math.h>
	//put math and related algorithmic methods here
	//quickHull-- outputs the passed in vertices as the convex hill
	// using the passed in vertices and the quick hull algorithm returns true is a convex hull was found, 
	// false otherwise
	//inVertices- the vertices to construct the convex hull from
	//outVertices- the vertices to output the found convex hull to, outputs null if no hull could be found
	//numOfVertices- the number of vertices for the passed in vertices
	//numOfHullVertices- the number of vertices to output the convex hull to.
	bool quickHull(D3DXVECTOR3* inVertices,D3DXVECTOR3*& outVertices,int numOfVertices,int& numOfHullVertices);

	//distance-- returns the distance between two points in 3d space
	// distance is returned squared to save on processing time
	//p1- the first point to be considred in the distance
	//p2- the second point to be considred in the distance
	float distance(D3DXVECTOR3 p1, D3DXVECTOR3 p2);

	//distanceFromLine-- returns the passed in points distance from
	// the line defined be two passed in points
	//lineP1- the first point on the line
	//lineP2- the second point in the line
	//p1- the point to get the distance from the line from
	float distanceFromLine(D3DXVECTOR3 lineP1, D3DXVECTOR3 lineP2, D3DXVECTOR3 p1);

	//crossProduct-- returns the cross product
	//p1- th first point to calculate the cross product with
	//p2- the second point to calculate the cross product with
	D3DXVECTOR3 crossProduct(D3DXVECTOR3 p1,D3DXVECTOR3 p2);
}