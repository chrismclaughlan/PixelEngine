#pragma once
#include "win32_windows.h"

class GameWindow :public win32::MainWindow
{
public:
	void run()
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

		renderer.ClearScreen(colour);
		renderer.DrawGridV2(100, 100);
		renderer.DrawCircleP(100, 100, 50, 0xffffff);

		if (input.mouse_x_pos > 0 && input.mouse_y_pos > 0)
		{
			render::RenderState* rs = renderer.getRenderState();
			uint32 colour = 0xff0000;
			if (input.mouse_click)
				renderer.DrawCircleP(input.mouse_x_pos, input.mouse_y_pos, 30, 0xff0000);
			renderer.DrawCircleP(input.mouse_x_pos, input.mouse_y_pos, 20, colour);
		}
	}
};