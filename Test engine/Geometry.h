#pragma once

namespace JR_Geometry
{
	//Geometry--A simple class that contains information for geometry physics objects
	class Geometry
	{

	public:
		//defualt constructor-- creates a new instance of Geometry with defualt parameters
		Geometry();

		//decontructor-- cleans up memory for the Geometry instance
		~Geometry();

		//init-- initializes the Geometry
		//x- the x coord
		//y- the y coord
		//z- the z coord
		void init(float x,float y,float z);

		//setX- sets the x coord for the geometry
		//x- the new x coord for the geometry
		void setX(float x);

		//setX- sets the x coord for the geometry
		//x- the new x coord for the geometry
		void setY(float y);

		//setX- sets the x coord for the geometry
		//x- the new x coord for the geometry
		void setZ(float z);

		//frame-- updates the position of the geometry
		//frameTime- the time to multiply the frame by
		virtual void frame(float frameTime);

		//setXImpulse-- sets the impulse speed along the x axis
		//xImpulse- the new impulse speed along the x Axis
		void setXImpulse(float xImpulse);

		//setYImpulse-- sets the impulse speed along the y axis
		//yImpulse- the new impulse speed along the y axis
		void setYImpulse(float yImpulse);

		//setZImpulse-- sets the impulse speed along the z axis
		//zImpulse- the new implulse speed along the z axis
		void setZImpulse(float zImpulse);

		float getX();

		float getDX();

	private:
		//x- the x coord of the geometry
		float m_x;
		//y- the y coord of the geometry
		float m_y;
		//z- the z coord of the geometry
		float m_z;
		//dx- the delta x of this geometry
		float m_dx;
		//dy- the delta y of this geometry
		float m_dy;
		//dz- the delta z of this geometry
		float m_dz;
	};
}