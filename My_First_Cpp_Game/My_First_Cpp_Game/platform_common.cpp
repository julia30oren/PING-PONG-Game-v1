struct Button_State
{
	bool is_down;
	bool changed;
};

enum MyEnum
{
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,

	BUTTON_COUNT, // Should be the last to count
};

struct Input
{
	Button_State buttons[BUTTON_COUNT];
};