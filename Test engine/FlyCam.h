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

		D3DXMATRIX getViewMatrix();

		float getRotX();

		float getRotY();

		float getRotZ();

		float getPosX();

		float getPosY();

		float getPosZ();

		void setRotX(float x);

		void setRotY(float y);

		void setRotZ(float z);

		void setPosX(float x);

		void setPosY(float y);

		void setPosZ(float z);

		void setDeltaX(float dx);

		void setDeltaY(float dy);

		void setDeltaZ(float dz);

		void setView(D3DXMATRIX view);
	private:
		float m_rotX;
		float m_rotY;
		float m_rotZ;

		float m_dx;
		float m_dy;
		float m_dz;

		float m_x;
		float m_y;
		float m_z;

		D3DXMATRIX m_view;
	};
}
