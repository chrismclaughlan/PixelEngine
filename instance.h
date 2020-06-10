#pragma once
#include "win32_windows.h"
#include "render.h"
#include "input.h"

class Instance
{
private:
	// Window attributes
	bool*				W_is_running;
	input::Input*		W_input;
	render::Render*		W_renderer;
	win32::Performance*	W_performance;

public:
	Instance(bool* is_running, win32::Performance* performance, render::Render* renderer,
		input::Input* input)
	{
		W_is_running = is_running;
		W_performance = performance;
		W_renderer = renderer;
		W_input = input;
	}

	void simulate()
	{
		// tests

		uint32 colour = 0x000000;
		if (is_down(input::BUTTON_UP))
			colour = 0xff0000;
		if (is_down(input::BUTTON_DOWN))
			colour = 0x0ff000;
		if (is_down(input::BUTTON_LEFT))
			colour = 0x00ff00;
		if (is_down(input::BUTTON_RIGHT))
			colour = 0x000ff0;

		W_renderer->ClearScreen(colour);
		W_renderer->DrawGridV2(100, 100);
		W_renderer->DrawCircleP(100, 100, 50, 0xffffff);

		if (W_input->mouse_x_pos > 0 && W_input->mouse_y_pos > 0)
		{
			render::RenderState* rs = W_renderer->getRenderState();
			uint32 colour = 0xff0000;
			if (W_input->mouse_click)
				W_renderer->DrawCircleP(W_input->mouse_x_pos, W_input->mouse_y_pos, 30, 0xff0000);
			W_renderer->DrawCircleP(W_input->mouse_x_pos, W_input->mouse_y_pos, 20, colour);
		}
	}
};