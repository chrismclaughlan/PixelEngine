#pragma once
#include "win32_windows.h"
#include "render.h"
#include "input.h"

class Instance
{
private:
	Input* input;
	Render* renderer;
	Performance* performance;
	bool* is_running;

public:
	Instance(Performance* nperformance, Render* nrenderer, Input* ninput, bool* nis_running)
	{
		performance = nperformance;
		renderer = nrenderer;
		input = ninput;
		is_running = nis_running;
	}

	void simulate()
	{
		uint32 colour = 0x000000;
		if (is_down(BUTTON_UP))
			colour = 0xff0000;
		if (is_down(BUTTON_DOWN))
			colour = 0x0ff000;
		if (is_down(BUTTON_LEFT))
			colour = 0x00ff00;
		if (is_down(BUTTON_RIGHT))
			colour = 0x000ff0;

		renderer->ClearScreen(colour);
		renderer->DrawGridV2(100, 100);
	}
};