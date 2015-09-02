#pragma once

#include <D3DX10math.h>

namespace JR_Frustum
{

	class Frustum
	{
	public:
		//default constructor-- creates a new instance of frustum with defualt parameters
		Frustum();

		//deconstructor-- cleans up memory for the frustum instance
		~Frustum();

		//construcFrustum-- creates the planes for the frustum
		//screenDepth- the depth of the screen
		//projectionMatrix- the projection matrix
		//viewMatrix- the view matrix
		void constructFrustum(float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix);

		//checkPoint-- checks to see if the pont is inside of the frustum or not
		//x- the x coord of the point
		//y- the y coord of the point
		//z- the z coord of the point
		bool checkPoint(float x, float y, float z);

		//checkCube-- checks to see if the passed in cube is in the frustrum
		//xCenter- the x center of the cube
		//yCenter- the y center of the cube
		//zCenter- the z center of the cube
		//radius- the radius of the cube
		bool checkCube(float xCenter, float yCenter, float zCenter, float radius);

		//checkSphere-- checks to see if the passed in cube is in the frustrum
		//xCenter- the x center of the cube
		//yCenter- the y center of the cube
		//zCenter- the z center of the cube
		//radius- the radius of the cube
		bool checkSphere(float xCenter, float yCenter, float zCenter, float radius);

		//checkRectangle- checks to see if the passed in ractangle is in the frustrum
		//xCenter- the x center of the cube
		//yCenter- the y center of the cube
		//zCenter- the z center of the cube
		//xSize- the width along the x axis
		//ySize- the width along the y axis
		//zSize- the width along the z axis
		bool checkRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);

	private:
		//planes- the planes of the viewing frustum
		D3DXPLANE m_planes[6];



	};
}