#include "FlyCam.h"

namespace JR_FlyCam
{
	FlyCam::FlyCam()
	{
		m_rotX = m_rotY =
			m_rotZ = m_x =
			m_y = m_z = m_dx =
			m_dy = m_dz =0;
	}

	void FlyCam::init(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
	{
		m_rotX = rotX;
		m_rotY = rotY;
		m_rotZ = rotZ;

		m_x = posX;
		m_y = posY;
		m_z = posZ;

	}

	void FlyCam::updateView()
	{
		D3DXVECTOR3 up, position,relativePosition, lookAt;
		float yaw, pitch, roll;
		D3DXMATRIX rotationMatrix;


		// Setup the vector that points upwards.
		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;

		//// Setup the position of the camera in the world.
		//position.x = -m_x;
		//position.y = -m_y;
		//position.z = -m_z;

		////Setup the reative position

		// Setup where the camera is looking by default.
		lookAt.x = 0.0f;
		lookAt.y = 0.0f;
		lookAt.z = 1.0f;

		// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
		pitch = m_rotX * 0.0174532925f;
		yaw = m_rotY * 0.0174532925f;
		roll = m_rotZ * 0.0174532925f;

		// Create the rotation matrix from the yaw, pitch, and roll values.
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

		// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
		D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
		D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

		//set up the relative position
		relativePosition.x = m_dx;
		relativePosition.y = m_dy;
		relativePosition.z = m_dz;

		//rotate the relative position 
		D3DXVec3TransformCoord(&relativePosition, &relativePosition, &rotationMatrix);


		//adde the new position to the m

		m_x += relativePosition.x;
		m_y += relativePosition.y;
		m_z += relativePosition.z;

		position.x = -m_x;
		position.y = -m_y;
		position.z = -m_z;
		// Translate the rotated camera position to the location of the viewer.
		lookAt = position + lookAt;

		// Finally create the view matrix from the three updated vectors.
		D3DXMatrixLookAtLH(&m_view, &position, &lookAt, &up);
	}

	void FlyCam::getViewMatrix(D3DXMATRIX &matrix)
	{
		matrix = m_view;
	}

	float FlyCam::getRotX()
	{
		return m_rotX;
	}

	float FlyCam::getRotY()
	{
		return m_rotY;
	}

	float FlyCam::getRotZ()
	{
		return m_rotZ;
	}

	float FlyCam::getPosX()
	{
		return m_x;
	}

	float FlyCam::getPosY()
	{
		return m_y;
	}

	float FlyCam::getPosZ()
	{
		return m_z;
	}

	void FlyCam::setRotX(float rotX)
	{
		m_rotX = rotX;
		if (m_rotX > 90)
		{
			m_rotX = 90;
		}

		if (m_rotX < -90)
		{
			m_rotX = -90;
		}
	}

	void FlyCam::setRotY(float rotY)
	{
		m_rotY = rotY;

	}

	void FlyCam::setRotZ(float rotZ)
	{
		m_rotZ = rotZ;
	}

	void FlyCam::setPosX(float x)
	{
		m_x = x;
	}

	void FlyCam::setPosY(float y)
	{
		m_y = y;
	}

	void FlyCam::setPosZ(float z)
	{
		m_z = z;
	}

	D3DXMATRIX FlyCam::getViewMatrix()
	{
		return m_view;
	}

	void FlyCam::setView(D3DXMATRIX view)
	{
		m_view = view;
	}

	void FlyCam::setDeltaX(float dx)
	{
		m_dx = dx;
	}

	void FlyCam::setDeltaY(float dy)
	{
		m_dy = dy;
	}

	void FlyCam::setDeltaZ(float dz)
	{
		m_dz = dz;
	}
}