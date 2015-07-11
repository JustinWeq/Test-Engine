#pragma once
#include <D3D11.h>
#include <D3DX10math.h>

namespace JR_FBXModel
{
	class FBXModel
	{


	private:
		//vertexBuffer-- the vertex buffer for the model
		ID3D11Buffer* m_vertexBuffer;
		//indexBuffer-- the index buffer for the model
		ID3D11Buffer* m_indexBuffer;
		//numOfVertices-- the number of vertices
		unsigned int m_numOfVerticies;
		//numOfIndicies-- the number of indicies
		unsigned int m_numOfIndicies;

	};
}
