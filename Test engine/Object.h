#pragma once
#include "Model.h"
#include <D3DX10math.h>
using namespace JR_Model;
namespace JR_Object
{
	class Object
	{
	public:

		Object();
		~Object();

		bool init(ID3D11Device* device,char*filename,WCHAR* textureName);

		void render(ID3D11DeviceContext* context);

		void shutdown();

		D3DXMATRIX operator*(D3DXMATRIX other);
		D3DXMATRIX getWorld();

		int getIndexCount();
		ID3D11ShaderResourceView* getTexture();

		void getVertices(D3DXVECTOR3* verticies);

		int getModelVertexCount();
	private:
		Model* m_model;
		D3DXMATRIX m_world;

	};
}