#include "Model.h"
namespace JR_Model
{
	//defualt constructor--creates a new Model instance with defualt parameters
	Model::Model()
	{
		m_indexBuffer = NULL;
		m_indexCount = NULL;
		m_texture = NULL;
		m_vertexBuffer = NULL;
		m_vertexCount = NULL;
	}

	//deconstructor--  cleans up dynamic memory of the model object
	Model::~Model()
	{
		shutdown();
	}

	//init-- initializes the model
	//device- the device to use for initialization
	//modelFileName- the name of the model to load
	//textureFilename-- the name of the texture to load
	bool Model::init(ID3D11Device* device, char* modelFilename, LPCSTR textureFilename)
	{
		bool result;

		//load in the model data
		result = loadObjModel(modelFilename);
		//result = loadModel(modelFilename);
		if (!result)
		{
			return false;
		}

		//init the vertex and index buffers
		result = initBuffers(device);
		if (!result)
		{
			return false;
		}

		//Load the texture for this model
		result = loadTexture(device, textureFilename);
		if (!result)
		{
			return false;
		}

		return true;
	}

	//shutdown-- releases all dynamic memory
	void Model::shutdown()
	{
		//release the models texture
		releaseTexture();
		//Shutdown the vertex and index buffers.
		shutdownBuffers();
		//Release the model data
		releaseModel();
	}

	//render-- renders the model
	//deviceContext- the device context to use for rendering
	void Model::render(ID3D11DeviceContext* deviceContext)
	{
		//Put the vertex and index buffers into the device context to preparre them for rendering
		renderBuffers(deviceContext);
	}

	//getIndexCount-- returns the number of indices for this model
	int Model::getIndexCount()
	{
		return m_indexCount;
	}

	//getTexture-- returns the texture for this model
	ID3D11ShaderResourceView* Model::getTexture()
	{
		return  m_texture->getTexture();
	}

	//initBuffers-- initializes the vertex and index buffer for this model
	//device- the device to use for initialization
	bool Model::initBuffers(ID3D11Device* device)
	{
		Vertex* vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;

		//create the vertex array
		vertices = new Vertex[m_vertexCount];
		if (!vertices)
		{
			return false;
		}

		//Create the index array
		indices = new unsigned long[m_indexCount];
		if (!indices)
		{
			return  false;
		}

		//Load the vertex array and index array with data
		for (int i = 0; i < m_vertexCount; i++)
		{
			vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].z, m_model[i].y);
			vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
			vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

			indices[i] = i;
		}

		//Set up the description of the static vertex buffer
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(Vertex)* m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		//Give the subresource structure a pointer to the vertex data
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		//Now create a vertex buffer
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		//Set up the description of the static index buffer
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		//give the subresouce data for the index buffer a pointer to the indicies
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;


		//Create hte index buffer
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		//Release the arrays now that the vertex and index arrays are no longer needed
		delete[] vertices;
		vertices = NULL;

		delete[] indices;
		indices = NULL;

		return true;
	}

	//shutdownBuffers-- cleans up memory for the buffers
	void Model::shutdownBuffers()
	{
		//release the buffers for the verticies and indices
		if (m_indexBuffer)
		{
			m_indexBuffer->Release();
			m_indexBuffer = NULL;
		}

		if (m_vertexBuffer)
		{
			m_vertexBuffer->Release();
			m_vertexBuffer = NULL;
		}

		return;
	}

	//renderBuffers-- renders the buffers
	//deviceContext- the device context to put the index and vertex buffers on
	void Model::renderBuffers(ID3D11DeviceContext* deviceContext)
	{
		unsigned int stride;
		unsigned int offset;

		//set the vertex buffer stride and offset
		stride = sizeof(Vertex);
		offset = 0;

		//Set the vertex buffer to be active on the device context
		deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		//Set the index buffer to active in the input assembler so it can be renderered
		deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//set the type of primitive topology to use for rendering
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	//loadTexture-- loads the texture for this model
	//device- the device to use for loading
	//filename- the name of the file the texture is in
	bool Model::loadTexture(ID3D11Device* device, LPCSTR filename)
	{
		bool result;
		//Create the texture object
		m_texture = new Texture();
		result = m_texture->init(device, filename);
		if (!result)
		{
			return false;
		}

		return true;
	}

	//releaseTexture-- cleans up memory for the texture
	void Model::releaseTexture()
	{
		if (m_texture)
		{
			m_texture->shutdown();
			delete m_texture;
			m_texture = NULL;
		}
	}

	//loadModel-- loads the model
	//filename- the name of the file to load the model from
	bool Model::loadModel(char* filename)
	{
		ifstream fin;
		char input;

		//Open the model file
		fin.open(filename);

		//If the file could not be opened then exit
		if (fin.fail())
		{
			return false;
		}

		//Read up to the value of vertex count
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}

		//Read in the vertex count
		fin >> m_vertexCount;

		//Set the number of indices to be the same as the vertex count
		m_indexCount = m_vertexCount;

		//Create the model using the vertex count that was read in.
		m_model = new ModelType[m_vertexCount];
		if (!m_model)
		{
			return false;
		}

		//Read up to the begining of the data
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		fin.get(input);
		fin.get(input);

		//Read in the vertex data
		for (int i = 0; i < m_vertexCount; i++)
		{
			fin >> m_model[m_vertexCount - 1 - i].x >> m_model[m_vertexCount - 1 - i].y >> m_model[m_vertexCount - 1 - i].z;
			fin >> m_model[m_vertexCount - 1 - i].tu >> m_model[m_vertexCount - 1 - i].tv;
			fin >> m_model[m_vertexCount - 1 - i].nx >> m_model[m_vertexCount - 1 - i].ny >> m_model[m_vertexCount - 1 - i].nz;
		}

		//Close the model file.
		fin.close();

		return true;
	}

	//releaseModel-- releases the memory for the model
	void Model::releaseModel()
	{
		if (m_model)
		{
			delete[] m_model;
			m_model = NULL;
		}
	}

	//loadFBXModel--load the model in a fbx format
	//filename- the name of the file to load
	bool Model::loadFBXModel(char* filename)
	{
		bool result;
		vector<Vertex> vertices;
		vector<D3DXVECTOR3> coords;
		vector<D3DXVECTOR2> uvCoordsf;
		vector<D3DXVECTOR3> normalsf;


		//set up the fbxmanager
		FbxManager* fbxManager = FbxManager::Create();

		//set the settings for the manager
		FbxIOSettings* ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
		fbxManager->SetIOSettings(ioSettings);
		/*(*(fbxManager->GetIOSettings())).SetBoolProp(IMP_FBX_MATERIAL, true);
		(*(fbxManager->GetIOSettings())).SetBoolProp(IMP_FBX_TEXTURE, true);
		(*(fbxManager->GetIOSettings())).SetBoolProp(IMP_FBX_LINK, false);
		(*(fbxManager->GetIOSettings())).SetBoolProp(IMP_FBX_SHAPE, false);
		(*(fbxManager->GetIOSettings())).SetBoolProp(IMP_FBX_GOBO, false);
		(*(fbxManager->GetIOSettings())).SetBoolProp(IMP_FBX_ANIMATION, true);
		(*(fbxManager->GetIOSettings())).SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);*/

		//set the settings for the fbx io settings
		//create and init the importer
		FbxImporter *importer = FbxImporter::Create(fbxManager,"");

		//create and init the scene
		FbxScene* fbxScene = FbxScene::Create(fbxManager, "");
		//init the importer
		result = importer->Initialize(filename,-1, fbxManager->GetIOSettings());
		if (!result)
		{
			string error = importer->GetStatus().GetErrorString();
			FbxStatus status = importer->GetStatus().GetCode();
			if (importer->GetStatus().GetCode() == FbxStatus::eFailure)
			{
				int test = 0;
			}
			return false;
		}

		//importer the scene
		result = importer->Import(fbxScene);
		if (!result)
		{
			return false;
		}

		//destroy the importer
		importer->Destroy();

		//get the root node
		FbxNode* fbxRootNode = fbxScene->GetRootNode();

		if (fbxRootNode)
		{
			//get the x,y,z coords of the vertices
			for (int i = 0; i < fbxRootNode->GetChildCount(); i++)
			{
				FbxNode* fbxChildNode = fbxRootNode->GetChild(i);

				if (fbxChildNode->GetNodeAttribute() == NULL)
				{
					continue;
				}

				FbxNodeAttribute::EType attributeType = fbxChildNode->GetNodeAttribute()->GetAttributeType();

				if (attributeType != FbxNodeAttribute::eMesh)
				{
					continue;
				}

				FbxMesh* mesh =(FbxMesh*) fbxChildNode->GetNodeAttribute();

				//get the vertices
				FbxVector4* vertices = mesh->GetControlPoints();

				for (int j = 0; j < mesh->GetPolygonCount(); j++)
				{
					int numVerticies = mesh->GetPolygonSize(j);

					for (int k = 0; k < numVerticies; k++)
					{
						int iControlPointIndex = mesh->GetPolygonVertex(j, k);

						D3DXVECTOR3 vertex;
						vertex.x = vertices[iControlPointIndex].mData[0];
						vertex.y = vertices[iControlPointIndex].mData[1];
						vertex.z = vertices[iControlPointIndex].mData[2];
						coords.push_back(vertex);
					}
				}
				FbxLayerElementArrayTemplate<FbxVector4>** normals;
				//get the normals
				mesh->GetNormals(normals);
				for (int j = 0; i < mesh->GetPolygonCount(); j++)
				{
					int numVertices = mesh->GetPolygonSize(j);
					for (int k = 0; k < numVertices; k++)
					{
						D3DXVECTOR3 vertex;
						vertex.x = normals[i][j][k].mData[0];
						vertex.y = normals[i][j][k].mData[1];
						vertex.z = normals[i][j][k].mData[2];
						normalsf.push_back(vertex);
					}
				}
				FbxLayerElementArrayTemplate<FbxVector2>** uvCoords;
			    //get UV coords
				mesh->GetTextureUV(uvCoords);
				for (int j = 0; j < mesh->GetPolygonCount(); j++)
				{
					int numvertices = mesh->GetPolygonSize(j);
					for (int k = 0; k < numvertices; k++)
					{
						D3DXVECTOR2 vertex;
						vertex.x = uvCoords[i][j][k].mData[0];
						vertex.y = uvCoords[i][j][k].mData[1];
						uvCoordsf.push_back(vertex);
					}
				}
			}

		}

		//now add the obtained model information to the m_model
		m_model = new ModelType[coords.size()];
		for (int i = 0; i < coords.size(); i++)
		{
			m_model[i].x = coords[i].x;
			m_model[i].y = coords[i].y;
			m_model[i].z = coords[i].z;
			m_model[i].nx = normalsf[i].x;
			m_model[i].ny = normalsf[i].y;
			m_model[i].nz = normalsf[i].z;
			m_model[i].tu = uvCoordsf[i].x;
			m_model[i].tv = uvCoordsf[i].y;
		}
		return true;
	}

	//loadObjModel--loads the model in the obj format
	//filename- the name of the model to load
	bool Model::loadObjModel(char * filename)
	{
		vector<D3DXVECTOR2> uv_coords;
		vector<D3DXVECTOR3> coords;
		vector<D3DXVECTOR3> normals;
		vector<int> vertexIndices;
		vector<int> uvIndices;
		vector<int> normalIndices;
		//load the model

		FILE* file = fopen(filename, "r");

		//if the file failed to open return false;
		if (file == NULL)
		{
			return false;
		}
		//start reading 
		while (true)
		{
			char line[128];
			int result = fscanf(file, "%s", line);
			//check to make sure that it is not the end of the file
			if (result == EOF)
				break;

			//parse the model information
			
			//check to see if its a vertex
			if (strcmp(line,"v")==0)
			{
				D3DXVECTOR3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				coords.push_back(vertex);
			}
			else
			//check to see if its a uv coords
			if (strcmp(line, "vt") == 0)
			{
				D3DXVECTOR2 uv_coord;
				fscanf(file, "%f %f\n", &uv_coord.x, &uv_coord.y);
				uv_coords.push_back(uv_coord);
			}
			else
			//check to see if its the normal coords
			if (strcmp(line, "vn") == 0)
			{
				D3DXVECTOR3 normal;
				fscanf(file, "%f %f %f", &normal.x, &normal.y, &normal.z);
				normals.push_back(normal);
			}
			else
			//check to see if the line is a face
			if (strcmp(line, "f") == 0)
			{
				string v1, v2, v3;
				unsigned int vertexIndex[3],uvIndex[3],normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9)
				{
					//close the file before returning
					fclose(file);
					return false;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}


		}

		//information has been parsed, now load the vertices into the model
		
		//set up m_model
		m_model = new ModelType[vertexIndices.size()];
		m_vertexCount = vertexIndices.size();
		m_indexCount = vertexIndices.size();

		//read each vertex of the triangle
		for (int i = 0; i < vertexIndices.size(); i++)
		{
			//read coords
			m_model[i].x = coords[vertexIndices[i] - 1].x;
			m_model[i].y = coords[vertexIndices[i] - 1].y;
			m_model[i].z = coords[vertexIndices[i] - 1].z;
			//read uvs
			m_model[i].tu = uv_coords[uvIndices[i] - 1].x;
			m_model[i].tv = uv_coords[uvIndices[i] - 1].y;
			//read normals
			m_model[i].nx = normals[normalIndices[i] - 1].x;
			m_model[i].ny = normals[normalIndices[i] - 1].y;
			m_model[i].nz = normals[normalIndices[i] - 1].z;

		}

		//close the file now that we are done reading it.
		fclose(file);
		//succesfully loaded model;
		return true;;
	}

	//copyVertices--copys the models vertices to the passed in 
	// array, mostly to be used for debugging
	//vertices- the vertices array to be copyied to
	void Model::copyVertices(D3DXVECTOR3* vertices)
	{
		int size = m_vertexCount;
		for (int i = 0;i < size;i++)
		{
			D3DXVECTOR3 vector;
			vector.x = m_model[i].x;
			vector.y = m_model[i].y;
			vector.z = m_model[i].z;

			vertices[i] = vector;
		}
	}

	//getModelVertexCount-- returns the number of vertices for the base model
	int Model::getModelVertexCount()
	{
		return m_vertexCount;
	}
}
