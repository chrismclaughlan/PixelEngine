#pragma once
#include "win32_windows.h"
#include "render.h"
#include "input.h"

class Instance
{
private:
	// Window attributes
	Input*			W_input;
	Render*			W_renderer;
	Performance*	W_performance;
	bool*			W_is_running;

public:
	Instance(Performance* performance, Render* renderer, Input* input, bool* is_running)
	{
		W_performance = performance;
		W_renderer = renderer;
		W_input = input;
		W_is_running = is_running;
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

		W_renderer->ClearScreen(colour);
		W_renderer->DrawGridV2(100, 100);
	}
};