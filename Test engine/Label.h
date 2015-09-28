#pragma once
#include "GuiElement.h"
#include "Text.h"
namespace JR_Label
{
	using namespace JR_Text;
	using namespace JR_Gui;
	//Label- a class that contains methods and propertys for a label
	class Label : GuiElement
	{
	private:
		const int  MAX_SENTENCES = 5;
	public:
		//defualt constructor-- creates a new instance of Label with defualt parameters
		Label();

		//init-- initializes the label
		virtual void init(float x,float y,float width,float height) override;

		//update-- updates the label
		//updateInfo- the update information for the last frame
		virtual void update(const UpdateInfo* updateInfo) override;

		//shutdown-- cleans up dynamic memory for the label
		virtual void shutdown() override;

		//setText-- sets the text for the label, can read '\n's, number of lines cannot be greater then
		// 5
		//text- the text to set the label to
		void setText(char* text);



		char* getText();


		//draw-- draws the label, overridden from parent:GuiElement
		//deviceContext- the device context to use for drawing
		//shader- the shader to use for drawing
		//view- the view matrix to use for drawing
		//ortho- the ortho matrix to use for drawing
		virtual void draw(ID3D11DeviceContext* deviceContext, Shader shader, D3DXMATRIX view, D3DXMATRIX ortho) override ;
	private:

	};
}