#include "Object.h"

namespace JR_Object
{
	Object::Object()
	{
		m_model = NULL;
	}

	Object::~Object()
	{
		shutdown();
	}

	bool Object::init(ID3D11Device* device, char*filename, WCHAR* textureName)
	{
		bool result;
		m_model = new Model();
		result = m_model->init(device, filename, textureName);
		if (!result)
		{
			return false;
		}
		D3DXMatrixIdentity(&m_world);
		return true;
	}

	void Object::render(ID3D11DeviceContext* deviceContext)
	{
		m_model->render(deviceContext);
	}

	void Object::shutdown()
	{
		if (m_model)
		{
			m_model->shutdown();
			delete m_model;
			m_model = NULL;
		}
	}

	D3DXMATRIX Object::operator*(D3DXMATRIX other)
	{
		return (m_world*= other);
	}

	D3DXMATRIX Object::getWorld()
	{
		return m_world;
	}

	int Object::getIndexCount()
	{
		return m_model->getIndexCount();
	}

	ID3D11ShaderResourceView* Object::getTexture()
	{
		return m_model->getTexture();
	}

}