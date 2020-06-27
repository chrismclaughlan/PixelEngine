#pragma once

#define process_button(b, vk)\
case vk: {\
input.buttons[b].has_changed = new_is_down != input.buttons[b].is_down;\
input.buttons[b].is_down = new_is_down;\
} break;\

#define is_down(b) (input.buttons[b].is_down)
#define pressed(b) (input.buttons[b].is_down && input.buttons[b].has_changed)
#define released(b) (!input.buttons[b].is_down && input.buttons[b].has_changed)

namespace input
{
struct ButtonState
{
	bool is_down;
	bool has_changed;
};

struct MouseState
{
	// implement
};

enum
{
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_SPACE,
	BUTTON_RETURN,
	BUTTON_ESCAPE,

	BUTTON_0,
	BUTTON_1,
	BUTTON_2,
	BUTTON_3,
	BUTTON_4,
	BUTTON_5,
	BUTTON_6,
	BUTTON_7,
	BUTTON_8,
	BUTTON_9,

	BUTTON_COUNT,  // always last
};

struct Input
{
	ButtonState buttons[BUTTON_COUNT] = {};
	int32 mouse_x_pos = -1;
	int32 mouse_y_pos = -1;
	bool left_click = false;
	bool right_click = false;
	bool middle_click = false;
	int32 wheelDeltaCarry = 0;
	int32 wheelTurns = 0;
	//bool mouse_dragging = false;
};
}  // namespace input