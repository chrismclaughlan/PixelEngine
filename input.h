#pragma once

#define process_button(b, vk)\
case vk: {\
input.buttons[b].has_changed = new_is_down != input.buttons[b].is_down;\
input.buttons[b].is_down = new_is_down;\
} break;\

#define is_down(b) (W_input->buttons[b].is_down)
#define pressed(b) (W_input->buttons[b].is_down && W_input->buttons[b].has_changed)
#define released(b) (!W_input->buttons[b].is_down && W_input->buttons[b].has_changed)

struct ButtonState
{
	bool is_down;
	bool has_changed;
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

	BUTTON_COUNT,  // always last
};

struct Input
{
	ButtonState buttons[BUTTON_COUNT];
};