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
}