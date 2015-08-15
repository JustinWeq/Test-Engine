#pragma once
#include "Input.h"
#include <D3D11.h>
#include <D3DX10math.h>

namespace JR_FlyCam
{
	//a simple class that is meant for scene navigation
	class FlyCam
	{
	public:
		FlyCam();

		void init(float posX,float posY,float posZ,float rotX,float rotY,float rotZ);

		void updateView();

		void getViewMatrix(D3DXMATRIX &matrix);

		float getRotX();

		float getRotY();

		float getRotZ();

		float getPosX();

		float getPosY();

		float getPosZ();

		float setRotX(float x);

		float setRotY(float y);

		float setRotZ(float z);

		float setPosX(float x);

		float setPosY(float y);

		float setPosZ(float z);
	private:
		float m_rotX;
		float m_rotY;
		float m_rotZ;

		float m_x;
		float m_y;
		float m_z;

		D3DXMATRIX m_view;
	};
}
