#pragma once

#include "Terrain.h"
#include "Frustum.h"
#include "Shader.h"

namespace JR_TerrainQuadTree
{
	using namespace JR_Frustum;
	using namespace JR_Terrain;
	using namespace JR_Shader;
	
	//TerrainQuadTree- a simple quad tree class that enables faster collisons and culling with the terrain
	class TerrainQuadTree
	{
	private:

		//Vertex- a struct that defines a vertex
		struct Vertex
		{
			//position- the position of the vertex
			D3DXVECTOR3 position;
			//texture- the texture of the vertex
			D3DXVECTOR2 texture;
			//normal- the normal of the vertex
			D3DXVECTOR3 normal;
		};

		//Vector- a simpe struct that contains data for a 3D vector
		struct Vector
		{
			//x- the x coord of the vector
			float x;
			//y- the y coord of the vector
			float y;
			//z- the z coord of the vector
			float z;
		};

		//Node- a struct tha defines a tree branch
		struct Node
		{
			//positionX- the x position of the node
			float positionX;
			//positionZ- the z position of the node
			float positionZ;
			//width- the width of the node
			float width;
			//triangleCount- the number of triangles in this node
			int triangleCount;
			//vertexBuffer- the vertex buffer for this nodes model
			ID3D11Buffer* vertexBuffer;
			//indexBuffer- the index buffer for this nodes model
			ID3D11Buffer* indexBuffer;
			//nodes- the branches of this node
			Node* nodes[4];
			//vertexArray- the vertices of the node
			Vector* vertexArray;
		};

		public:
			//defualt constructor-- creates a new instance of TerrainQuadTree with defualt parameters
			TerrainQuadTree();

			//deconstructor-- cleans up memory for the TerrainQuadTree object
			~TerrainQuadTree();

			//init-- intializes the quad tree
			//terrain- the terrain to build the quad tree from
			//device- the device to use for initialization
			bool init(Terrain* terrain, ID3D11Device* device);

			//shutdown-- cleans up memory for the terrain quad tree
			void shutdown();

			//render- renders the tree
			//frustum- the frustum to use for culling
			//deviceContext- the device context to use for rendering
			//shader- the shader to use for rendering
			void render(Frustum* frustum, ID3D11DeviceContext* deviceContext, Shader* shader);

			//getDrawCount- returns the number of draw calls in a frame
			int getDrawCount();

			//getHeightAtPosition-- reutrns the height at the passed in position
			//x- the x coord of the position
			//y- the y coord of the position
			//outputHeight- the output to assign the height to
			bool getHeightAtPosition(float x, float z, float& outputHeight);

	private:

		//calculateMeshDimensions-- calculates the dimensions of the passed in mesh
		//vertexCount- the amount of vertices for the mesh
		//centerX- the center along the x axis
		//centerZ- the center along the z axis
		//meshWidth- the width(size) of the mesh
		void calculateMeshDimensions(int vertexCount, float& centerX, float& centerZ, float& meshWidth);

		//createTreeNode- creates the node for the tree
		//positionX- the x position of the node
		//positionZ- the z position of the node
		//width- the width of the node
		//device- the device to use
		void createTreeNode(Node* node, float positionX, float positionZ, float width, ID3D11Device* device);

		//countTriangles-- returns the amount of triangles in the tree
		//positionX- the x position of the tree
		//positionZ- the z position of the tree
		//width- the wodth of the tree
		int countTriangles(float positionX, float positionZ, float width);

		//isTriangleContinained-- reutrns wheter the triangle is contained
		//index- the index of the triangle
		//positionX- the x position of the triangle
		//positionZ- the z position of the triangle
		//width- the width of the triangle
		bool isTriangleContained(int index, float positionX, float positionZ, float width);

		//releaseNode-- releases the passed in node
		//node- the node to be released
		void releaseNode(Node* node);

		//renderNode- renders the passed in node
		//frustum- the frustum to use for culling
		//deviceContext- the device context to use for renering
		//shader- the shader to use for rendering
		void renderNode(Node* node, Frustum* frustum, ID3D11DeviceContext* deviceContext, Shader* shader);

		//setMaxTriangles-- sets the maximum number of triangles the tree can have in each node
		//max- the maximum number of triangles
		void setMaxTriangles(int max);

		//findNode- finds the node at the passed in position
		//node- the node 
		//x- the x coord of the position
		//y- the y coord of the position
		//outputHeight- the output to assign the height to
		void findNode(Node* node, float x, float z, float& outputHeight);

		//checkHeightOfTriangle- checks the height of the triangle
		//x- the x coord of the position
		//y- the y coord of the position
		//outputHeight- the output to assign the height to
		//v0-vertex 1
		//v1- vertex 2;
		//v3 vertex 3;
		bool checkHeightOfTriangle(float x, float z, float& outputHeight, float v0[3], float v1[3], float v2[3]);

	private:
		//triangleCount- the number of triangles in the tree
		int m_triangleCount;
		//drawCount- the number of draw calls in the tree
		int m_drawCount;
		//vertexList- the list of vertices in the tree
		Vertex* m_vertexList;
		//parentNode- the starting root node
		Node* m_parentNode;
		//maxTriangles- the maximum number of triangles per node
		int m_maxTriangles;
	};
}