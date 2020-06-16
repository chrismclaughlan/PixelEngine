#pragma once
#include "win32_windows.h"

class GameWindow :public win32::MainWindow
{
public:
	void run()
	{
		// demo visuals

		uint32 colour = 0x000000;
		if (is_down(input::BUTTON_UP))
			colour = 0xff0000;
		if (is_down(input::BUTTON_DOWN))
			colour = 0x0ff000;
		if (is_down(input::BUTTON_LEFT))
			colour = 0x00fff0;
		if (is_down(input::BUTTON_RIGHT))
			colour = 0x000fff;

		renderer.ClearScreen(colour);
		renderer.DrawGridV2(100, 100);
		renderer.DrawCircleP(100, 100, 50, 0xffffff);

		// Draw circle where cursor is
		if (input.mouse_click)
			renderer.DrawCircleP(input.mouse_x_pos, input.mouse_y_pos, 30, 0xff0000);
		renderer.DrawCircleP(input.mouse_x_pos, input.mouse_y_pos, 20, 0xff0000);
	}
};