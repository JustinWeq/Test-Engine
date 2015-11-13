#pragma once
#include "Font.h"
#include "App.h"
using namespace JR_Font;
using namespace Application;
namespace JR_GuiManager
{
	//GuiManager-- a simple class that contains methods and propertys for managing a GuiManager
	class GuiManager
	{
	public:
		//defualt constructor-- constructs a new instance of GuiManager
		GuiManager();

		//deconstructor-- cleans up dynamic memory for this GuiManager instance
		~GuiManager();
		
		//shutdown-- cleans up dynamic memory for this instance
		void shutdown();

		//init-- intializes the GuiManager instance
		//device- the device to use for initialization
		//fontFileName- the name of file address of the fonts data file
		//fontTexture- the name of the file address the fonts texture data
		//app- the point to the application for the window, used to obtain information like screen dimensions
		bool init(ID3D11Device* device, const char* fontFileName, WCHAR* fontTexture,App* app);

		//getFont-- returns the font for the GuiManager
		Font* getFont();

		//getApp-- returns the application the GuiManager is using
		App* getApp();

	private:
		Font* m_font;
		App* m_app;
	};
}
