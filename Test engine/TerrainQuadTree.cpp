#include "TerrainQuadTree.h"

namespace JR_TerrainQuadTree
{
	//defualt constructor-- creates a new instance of TerrainQuadTree with defualt parameters
	TerrainQuadTree::TerrainQuadTree()
	{
		m_vertexList = NULL;
		m_parentNode = NULL;
		//defualt number of triangles
		m_maxTriangles = 10000;
	}

	//deconstructor-- cleans up memory for the TerrainQuadTree object
	TerrainQuadTree::~TerrainQuadTree()
	{
		shutdown();
	}

	//init-- intializes the quad tree
	//terrain- the terrain to build the quad tree from
	//device- the device to use for initialization
	bool TerrainQuadTree::init(Terrain* terrain, ID3D11Device* device)
	{
		int vertexCount;
		float centerX, centerZ, width;

		//Get the number of vertices in the terrain vertex array
		vertexCount = terrain->getVertexCount();

		//store the total triangle count for the vertex list
		m_triangleCount = vertexCount / 3;

		//create a vertex array to hold ll of the terrain vertices
		m_vertexList = new Vertex[vertexCount];
		if (!m_vertexList)
		{
			return false;
		}

		//copy the terrain vertices into the vertex list
		terrain->copyVertexArray((void*)m_vertexList);

		//Calculate the center x,z and the width of the mesh
		calculateMeshDimensions(vertexCount, centerX, centerZ, width);

		//create the parent node for the quadtree
		m_parentNode = new Node;
		if (!m_parentNode)
		{
			return false;
		}

		//recusivly build th quad tree based on the vertex list data and mesh dimensions.
		createTreeNode(m_parentNode, centerX, centerZ, width, device);

		//release the vertex list since it is needed
		if (m_vertexList)
		{
			delete[] m_vertexList;
			m_vertexList = NULL;
		}

		return true;

	}

	//shutdown-- cleans up memory for the terrain quad tree
	void TerrainQuadTree::shutdown()
	{
		//recusivly release the quad tree data
		if (m_parentNode)
		{
			releaseNode(m_parentNode);
			delete m_parentNode;
			m_parentNode = NULL;
		}
	}

	//render- renders the tree
	//frustum- the frustum to use for culling
	//deviceContext- the device context to use for rendering
	//shader- the shader to use for rendering
	void TerrainQuadTree::render(Frustum* frustum, ID3D11DeviceContext* deviceContext, Shader* shader)
	{
		//reset the number of trianglles that are drawn this fram.
		m_drawCount = 0;

		//Render eash node that is visable starting at the parent node and moving down the tree.
		renderNode(m_parentNode,frustum, deviceContext, shader);
	}

	//getDrawCount- returns the number of draw calls in a frame
	int TerrainQuadTree::getDrawCount()
	{
		return m_drawCount;
	}

	//calculateMeshDimensions-- calculates the dimensions of the passed in mesh
	//vertexCount- the amount of vertices for the mesh
	//centerX- the center along the x axis
	//centerZ- the center along the z axis
	//meshWidth- the width(size) of the mesh
	void TerrainQuadTree::calculateMeshDimensions(int vertexCount, float& centerX, float& centerZ, float& meshWidth)
	{
		int i;
		float maxWidth, maxDepth, minWidth, minDepth, width, depth, maxX, maxZ;

		//init the center position of the mesh to zero
		centerX = 0.0f;
		centerZ = 0.0f;

		//sum all the vertices in the mesh.
		for (i = 0;i < vertexCount; i++)
		{
			centerX += m_vertexList[i].position.x;
			centerZ += m_vertexList[i].position.z;
		}

		//And then devide it by the number of vertices to find the midpoint(averidge)
		centerX /= (float)vertexCount;
		centerZ /= (float)vertexCount;

		//init the maximum and minimum size of the mesh
		maxWidth = 0.0f;
		maxDepth = 0.0f;

		minWidth = fabsf(m_vertexList[0].position.x - centerX);
		minDepth = fabs(m_vertexList[0].position.z - centerZ);

		//Go through all of the verticea and find the maximum and minimum depth of the mesh
		for (i = 0;i < vertexCount;i++)
		{
			width = fabsf(m_vertexList[i].position.x - centerX);
			depth = fabsf(m_vertexList[i].position.z - centerZ);

			if (width > maxWidth)
				maxWidth = width;
			if (depth > maxDepth)
				maxDepth = depth;
			if (width < minWidth)
				minWidth = width;
			if (depth < minDepth)
				minDepth = depth;
		}

		//find the absolute maximum value between the min and max depth and width
		maxX = (float)max(fabs(minWidth), fabs(maxWidth));
		maxZ = (float)max(fabs(minDepth), fabs(maxDepth));

		//Calculate the maximum diameter of the mesh
		meshWidth = max(maxX, maxZ) * 2.0;
	}

	//createTreeNode- creates the node for the tree
	//positionX- the x position of the node
	//positionZ- the z position of the node
	//width- the width of the node
	//device- the device to use
	void TerrainQuadTree::createTreeNode(Node* node, float positionX, float positionZ, float width, ID3D11Device* device)
	{
		int numTriangles, i, count, vertexCount, index, vertexIndex;
		float offsetX, offsetZ;
		Vertex* vertices;
		unsigned long* indices;
		bool result;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;

		// Store the node position and size.
		node->positionX = positionX;
		node->positionZ = positionZ;
		node->width = width;

		// Initialize the triangle count to zero for the node.
		node->triangleCount = 0;

		// Initialize the vertex and index buffer to null.
		node->vertexBuffer = 0;
		node->indexBuffer = 0;

		//init the vertex array to null

		node->vertexArray = NULL;

		// Initialize the children nodes of this node to null.
		node->nodes[0] = 0;
		node->nodes[1] = 0;
		node->nodes[2] = 0;
		node->nodes[3] = 0;

		// Count the number of triangles that are inside this node.
		numTriangles = countTriangles(positionX, positionZ, width);

		// If there are no triangles in this node then return as it is empty and requires no processing.
		if (numTriangles == 0)
		{
			return;
		}

		// If there are too many triangles in this node then split it into four equal sized smaller tree nodes.
		if (numTriangles > m_maxTriangles)
		{
			for (i = 0; i<4; i++)
			{
				// Calculate the position offsets for the new child node.
				offsetX = (((i % 2) < 1) ? -1.0f : 1.0f) * (width / 4.0f);
				offsetZ = (((i % 4) < 2) ? -1.0f : 1.0f) * (width / 4.0f);

				// See if there are any triangles in the new node.
				count = countTriangles((positionX + offsetX), (positionZ + offsetZ), (width / 2.0f));
				if (count > 0)
				{
					// If there are triangles inside where this new node would be then create the child node.
					node->nodes[i] = new Node;

					// Extend the tree starting from this new child node now.
					createTreeNode(node->nodes[i], (positionX + offsetX), (positionZ + offsetZ), (width / 2.0f), device);
				}
			}

			return;
		}

		// If this node is not empty and the triangle count for it is less than the max then 
		// this node is at the bottom of the tree so create the list of triangles to store in it.
		node->triangleCount = numTriangles;

		// Calculate the number of vertices.
		vertexCount = numTriangles * 3;

		// Create the vertex array.
		vertices = new Vertex[vertexCount];

		// Create the index array.
		indices = new unsigned long[vertexCount];

		//Create the vertex array
		node->vertexArray = new Vector[vertexCount];

		// Initialize the index for this new vertex and index array.
		index = 0;

		// Go through all the triangles in the vertex list.
		for (i = 0; i<m_triangleCount; i++)
		{
			// If the triangle is inside this node then add it to the vertex array.
			result = isTriangleContained(i, positionX, positionZ, width);
			if (result == true)
			{
				// Calculate the index into the terrain vertex list.
				vertexIndex = i * 3;

				// Get the three vertices of this triangle from the vertex list.
				vertices[index].position = m_vertexList[vertexIndex].position;
				vertices[index].texture = m_vertexList[vertexIndex].texture;
				vertices[index].normal = m_vertexList[vertexIndex].normal;
				indices[index] = index;


				// Also store the vertex position information in the node vertex array.
				node->vertexArray[index].x = m_vertexList[vertexIndex].position.x;
				node->vertexArray[index].y = m_vertexList[vertexIndex].position.y;
				node->vertexArray[index].z = m_vertexList[vertexIndex].position.z;

				// Increment the indexes.
				index++;
				vertexIndex++;

				// Do the same for the next point.
				vertices[index].position = m_vertexList[vertexIndex].position;
				vertices[index].texture = m_vertexList[vertexIndex].texture;
				vertices[index].normal = m_vertexList[vertexIndex].normal;
				indices[index] = index;
				node->vertexArray[index].x = m_vertexList[vertexIndex].position.x;
				node->vertexArray[index].y = m_vertexList[vertexIndex].position.y;
				node->vertexArray[index].z = m_vertexList[vertexIndex].position.z;
				index++;
				vertexIndex++;

				// Do the same for the next point also.
				vertices[index].position = m_vertexList[vertexIndex].position;
				vertices[index].texture = m_vertexList[vertexIndex].texture;
				vertices[index].normal = m_vertexList[vertexIndex].normal;
				indices[index] = index;
				node->vertexArray[index].x = m_vertexList[vertexIndex].position.x;
				node->vertexArray[index].y = m_vertexList[vertexIndex].position.y;
				node->vertexArray[index].z = m_vertexList[vertexIndex].position.z;
				index++;

			}
		}

		// Set up the description of the vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now finally create the vertex buffer.
		device->CreateBuffer(&vertexBufferDesc, &vertexData, &node->vertexBuffer);

		// Set up the description of the index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * vertexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		device->CreateBuffer(&indexBufferDesc, &indexData, &node->indexBuffer);

		// Release the vertex and index arrays now that the data is stored in the buffers in the node.
		delete[] vertices;
		vertices = 0;

		delete[] indices;
		indices = 0;


	}

	//countTriangles-- returns the amount of triangles in the tree
	//positionX- the x position of the tree
	//positionZ- the z position of the tree
	//width- the wodth of the tree
	int TerrainQuadTree::countTriangles(float positionX, float positionZ, float width)
	{
		int count, i;
		bool result;


		// Initialize the count to zero.
		count = 0;

		// Go through all the triangles in the entire mesh and check which ones should be inside this node.
		for (i = 0; i<m_triangleCount; i++)
		{
			// If the triangle is inside the node then increment the count by one.
			result = isTriangleContained(i, positionX, positionZ, width);
			if (result == true)
			{
				count++;
			}
		}

		return count;

	}

	//isTriangleContinained-- reutrns wheter the triangle is contained
	//index- the index of the triangle
	//positionX- the x position of the triangle
	//positionZ- the z position of the triangle
	//width- the width of the triangle
	bool TerrainQuadTree::isTriangleContained(int index, float positionX, float positionZ, float width)
	{
		float radius;
		int vertexIndex;
		float x1, z1, x2, z2, x3, z3;
		float minimumX, maximumX, minimumZ, maximumZ;


		// Calculate the radius of this node.
		radius = width / 2.0f;

		// Get the index into the vertex list.
		vertexIndex = index * 3;

		// Get the three vertices of this triangle from the vertex list.
		x1 = m_vertexList[vertexIndex].position.x;
		z1 = m_vertexList[vertexIndex].position.z;
		vertexIndex++;

		x2 = m_vertexList[vertexIndex].position.x;
		z2 = m_vertexList[vertexIndex].position.z;
		vertexIndex++;

		x3 = m_vertexList[vertexIndex].position.x;
		z3 = m_vertexList[vertexIndex].position.z;

		// Check to see if the minimum of the x coordinates of the triangle is inside the node.
		minimumX = min(x1, min(x2, x3));
		if (minimumX > (positionX + radius))
		{
			return false;
		}

		// Check to see if the maximum of the x coordinates of the triangle is inside the node.
		maximumX = max(x1, max(x2, x3));
		if (maximumX < (positionX - radius))
		{
			return false;
		}

		// Check to see if the minimum of the z coordinates of the triangle is inside the node.
		minimumZ = min(z1, min(z2, z3));
		if (minimumZ >(positionZ + radius))
		{
			return false;
		}

		// Check to see if the maximum of the z coordinates of the triangle is inside the node.
		maximumZ = max(z1, max(z2, z3));
		if (maximumZ < (positionZ - radius))
		{
			return false;
		}

		return true;

	}

	//releaseNode-- releases the passed in node
	//node- the node to be released
	void TerrainQuadTree::releaseNode(Node* node)
	{
		int i;


		// Recursively go down the tree and release the bottom nodes first.
		for (i = 0; i<4; i++)
		{
			if (node->nodes[i] != 0)
			{
				releaseNode(node->nodes[i]);
			}
		}

		// Release the vertex buffer for this node.
		if (node->vertexBuffer)
		{
			node->vertexBuffer->Release();
			node->vertexBuffer = 0;
		}

		// Release the index buffer for this node.
		if (node->indexBuffer)
		{
			node->indexBuffer->Release();
			node->indexBuffer = 0;
		}

		//Release the vertex array for this node
		if (node->vertexArray)
		{
			delete[] node->vertexArray;
			node->vertexArray = NULL;
		}

		// Release the four child nodes.
		for (i = 0; i<4; i++)
		{
			if (node->nodes[i])
			{
				delete node->nodes[i];
				node->nodes[i] = 0;
			}
		}



		return;

	}

	//renderNode- renders the passed in node
	//frustum- the frustum to use for culling
	//deviceContext- the device context to use for renering
	//shader- the shader to use for rendering
	void TerrainQuadTree::renderNode(Node* node, Frustum* frustum, ID3D11DeviceContext* deviceContext, Shader* shader)
	{
		bool result;
		int count, i, indexCount;
		unsigned int stride, offset;

		// Check to see if the node can be viewed, height doesn't matter in a quad tree.
		result = frustum->checkCube(node->positionX, 0.0f, node->positionZ, (node->width / 2.0f));

		// If it can't be seen then none of its children can either so don't continue down the tree, this is where the speed is gained.
		if (!result)
		{
			return;
		}

		// If it can be seen then check all four child nodes to see if they can also be seen.
		count = 0;
		for (i = 0; i<4; i++)
		{
			if (node->nodes[i] != 0)
			{
				count++;
				renderNode(node->nodes[i], frustum, deviceContext, shader);
			}
		}

		// If there were any children nodes then there is no need to continue as parent nodes won't contain any triangles to render.
		if (count != 0)
		{
			return;
		}

		// Otherwise if this node can be seen and has triangles in it then render these triangles.

		// Set vertex buffer stride and offset.
		stride = sizeof(Vertex);
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, &node->vertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetIndexBuffer(node->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Determine the number of indices in this node.
		indexCount = node->triangleCount * 3;

		// Call the terrain shader to render the polygons in this node.
		shader->renderTerrainShader(deviceContext, indexCount);

		// Increase the count of the number of polygons that have been rendered during this frame.
		m_drawCount += node->triangleCount;


	}

	//setMaxTriangles-- sets the maximum number of triangles the tree can have in each node
	//max- the maximum number of triangles
	void TerrainQuadTree::setMaxTriangles(int max)
	{
		m_maxTriangles = max;
	}

	//getHeightAtPosition-- reutrns the height at the passed in position
	//x- the x coord of the position
	//y- the y coord of the position
	//outputHeight- the output to assign the height to
	bool TerrainQuadTree::getHeightAtPosition(float x, float z, float& outputHeight)
	{
		float meshMinX, meshMaxX, meshMinZ, meshMaxZ;


		meshMinX = m_parentNode->positionX - (m_parentNode->width / 2.0f);
		meshMaxX = m_parentNode->positionX + (m_parentNode->width / 2.0f);

		meshMinZ = m_parentNode->positionZ - (m_parentNode->width / 2.0f);
		meshMaxZ = m_parentNode->positionZ + (m_parentNode->width / 2.0f);

		// Make sure the coordinates are actually over a polygon.
		if ((x < meshMinX) || (x > meshMaxX) || (z < meshMinZ) || (z > meshMaxZ))
		{
			return false;
		}

		// Find the node which contains the polygon for this position.
		findNode(m_parentNode, x, z, outputHeight);

		return true;

	}

	//findNode- finds the node at the passed in position
	//node- the node 
	//x- the x coord of the position
	//y- the y coord of the position
	//outputHeight- the output to assign the height to
	void TerrainQuadTree::findNode(Node* node, float x, float z, float& outputHeight)
	{
		float xMin, xMax, zMin, zMax;
		int count, i, index;
		float vertex1[3], vertex2[3], vertex3[3];
		bool foundHeight;


		// Calculate the dimensions of this node.
		xMin = node->positionX - (node->width / 2.0f);
		xMax = node->positionX + (node->width / 2.0f);

		zMin = node->positionZ - (node->width / 2.0f);
		zMax = node->positionZ + (node->width / 2.0f);

		// See if the x and z coordinate are in this node, if not then stop traversing this part of the tree.
		if ((x < xMin) || (x > xMax) || (z < zMin) || (z > zMax))
		{
			return;
		}

		// If the coordinates are in this node then check first to see if children nodes exist.
		count = 0;

		for (i = 0; i<4; i++)
		{
			if (node->nodes[i] != 0)
			{
				count++;
				findNode(node->nodes[i], x, z, outputHeight);
			}
		}

		// If there were children nodes then return since the polygon will be in one of the children.
		if (count > 0)
		{
			return;
		}

		// If there were no children then the polygon must be in this node.  Check all the polygons in this node to find 
		// the height of which one the polygon we are looking for.
		for (i = 0; i<node->triangleCount; i++)
		{
			index = i * 3;
			vertex1[0] = node->vertexArray[index].x;
			vertex1[1] = node->vertexArray[index].y;
			vertex1[2] = node->vertexArray[index].z;

			index++;
			vertex2[0] = node->vertexArray[index].x;
			vertex2[1] = node->vertexArray[index].y;
			vertex2[2] = node->vertexArray[index].z;

			index++;
			vertex3[0] = node->vertexArray[index].x;
			vertex3[1] = node->vertexArray[index].y;
			vertex3[2] = node->vertexArray[index].z;

			// Check to see if this is the polygon we are looking for.
			foundHeight = checkHeightOfTriangle(x, z, outputHeight, vertex1, vertex2, vertex3);

			// If this was the triangle then quit the function and the height will be returned to the calling function.
			if (foundHeight)
			{
				return;
			}
		}

	}

	//checkHeightOfTriangle- checks the height of the triangle
	//x- the x coord of the position
	//y- the y coord of the position
	//outputHeight- the output to assign the height to
	//v0-vertex 1
	//v1- vertex 2;
	//v3 vertex 3;
	bool TerrainQuadTree::checkHeightOfTriangle(float x, float z, float& outputHeight, float v0[3], float v1[3], float v2[3])
	{
		float startVector[3], directionVector[3], edge1[3], edge2[3], normal[3];
		float Q[3], e1[3], e2[3], e3[3], edgeNormal[3], temp[3];
		float magnitude, D, denominator, numerator, t, determinant;


		// Starting position of the ray that is being cast.
		startVector[0] = x;
		startVector[1] = 0.0f;
		startVector[2] = z;

		// The direction the ray is being cast.
		directionVector[0] = 0.0f;
		directionVector[1] = -1.0f;
		directionVector[2] = 0.0f;

		// Calculate the two edges from the three points given.
		edge1[0] = v1[0] - v0[0];
		edge1[1] = v1[1] - v0[1];
		edge1[2] = v1[2] - v0[2];

		edge2[0] = v2[0] - v0[0];
		edge2[1] = v2[1] - v0[1];
		edge2[2] = v2[2] - v0[2];

		// Calculate the normal of the triangle from the two edges.
		normal[0] = (edge1[1] * edge2[2]) - (edge1[2] * edge2[1]);
		normal[1] = (edge1[2] * edge2[0]) - (edge1[0] * edge2[2]);
		normal[2] = (edge1[0] * edge2[1]) - (edge1[1] * edge2[0]);

		magnitude = (float)sqrt((normal[0] * normal[0]) + (normal[1] * normal[1]) + (normal[2] * normal[2]));
		normal[0] = normal[0] / magnitude;
		normal[1] = normal[1] / magnitude;
		normal[2] = normal[2] / magnitude;

		// Find the distance from the origin to the plane.
		D = ((-normal[0] * v0[0]) + (-normal[1] * v0[1]) + (-normal[2] * v0[2]));

		// Get the denominator of the equation.
		denominator = ((normal[0] * directionVector[0]) + (normal[1] * directionVector[1]) + (normal[2] * directionVector[2]));

		// Make sure the result doesn't get too close to zero to prevent divide by zero.
		if (fabs(denominator) < 0.0001f)
		{
			return false;
		}

		// Get the numerator of the equation.
		numerator = -1.0f * (((normal[0] * startVector[0]) + (normal[1] * startVector[1]) + (normal[2] * startVector[2])) + D);

		// Calculate where we intersect the triangle.
		t = numerator / denominator;

		// Find the intersection vector.
		Q[0] = startVector[0] + (directionVector[0] * t);
		Q[1] = startVector[1] + (directionVector[1] * t);
		Q[2] = startVector[2] + (directionVector[2] * t);

		// Find the three edges of the triangle.
		e1[0] = v1[0] - v0[0];
		e1[1] = v1[1] - v0[1];
		e1[2] = v1[2] - v0[2];

		e2[0] = v2[0] - v1[0];
		e2[1] = v2[1] - v1[1];
		e2[2] = v2[2] - v1[2];

		e3[0] = v0[0] - v2[0];
		e3[1] = v0[1] - v2[1];
		e3[2] = v0[2] - v2[2];

		// Calculate the normal for the first edge.
		edgeNormal[0] = (e1[1] * normal[2]) - (e1[2] * normal[1]);
		edgeNormal[1] = (e1[2] * normal[0]) - (e1[0] * normal[2]);
		edgeNormal[2] = (e1[0] * normal[1]) - (e1[1] * normal[0]);

		// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
		temp[0] = Q[0] - v0[0];
		temp[1] = Q[1] - v0[1];
		temp[2] = Q[2] - v0[2];

		determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));

		// Check if it is outside.
		if (determinant > 0.001f)
		{
			return false;
		}

		// Calculate the normal for the second edge.
		edgeNormal[0] = (e2[1] * normal[2]) - (e2[2] * normal[1]);
		edgeNormal[1] = (e2[2] * normal[0]) - (e2[0] * normal[2]);
		edgeNormal[2] = (e2[0] * normal[1]) - (e2[1] * normal[0]);

		// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
		temp[0] = Q[0] - v1[0];
		temp[1] = Q[1] - v1[1];
		temp[2] = Q[2] - v1[2];

		determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));

		// Check if it is outside.
		if (determinant > 0.001f)
		{
			return false;
		}

		// Calculate the normal for the third edge.
		edgeNormal[0] = (e3[1] * normal[2]) - (e3[2] * normal[1]);
		edgeNormal[1] = (e3[2] * normal[0]) - (e3[0] * normal[2]);
		edgeNormal[2] = (e3[0] * normal[1]) - (e3[1] * normal[0]);

		// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
		temp[0] = Q[0] - v2[0];
		temp[1] = Q[1] - v2[1];
		temp[2] = Q[2] - v2[2];

		determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));

		// Check if it is outside.
		if (determinant > 0.001f)
		{
			return false;
		}

		// Now we have our height.
		outputHeight = Q[1];

		return true;

	}
}