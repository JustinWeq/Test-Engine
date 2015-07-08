#pragma once
#include "App.h"
#include <thread>
namespace Game
{
	//prototypes
	void init();
	bool update();
	void draw();
	App* app;
	bool isFinished;
}