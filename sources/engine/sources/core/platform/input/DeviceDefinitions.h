#ifndef DEVICE_DEFINITIONS
#define DEVICE_DEFINITIONS

enum DEVICES
{
#if(WINDOWS_PLATFORM || LINUX_PLATFORM)

	DEVICE_KEYBOARD = 0,
	DEVICE_MOUSE,
	DEVICE_COUNT

#elif(ANDROID_PLATFORM)

	DEVICE_TOUCH_SCREEN = 0,
	DEVICE_COUNT

#endif
};


#if(WINDOWS_PLATFORM || LINUX_PLATFORM)

enum KEYBOARD_BUTTONS
{
	BUTTON_KEYBOARD_0 = 0,
	BUTTON_KEYBOARD_1,
	BUTTON_KEYBOARD_2,
	BUTTON_KEYBOARD_3,
	BUTTON_KEYBOARD_UP,
	BUTTON_KEYBOARD_DOWN,
	BUTTON_KEYBOARD_LEFT,
	BUTTON_KEYBOARD_RIGHT,
	BUTTON_KEYBOARD_ESC,
	BUTTON_KEYBOARD_SPACE,
	BUTTON_KEYBOARD_COUNT
};

enum KEYBOARD_BUTTON_STATUS
{
	KEYBOARD_BUTTON_PRESS = 0,
	KEYBOARD_BUTTON_RELEASE
};

enum MOUSE_BUTTONS
{
	BUTTON_MOUSE_LEFT = 0,
	BUTTON_MOUSE_RIGHT,
	BUTTON_MOUSE_MIDDLE,
	BUTTON_MOUSE_COUNT
};

enum MOUSE_BUTTON_STATUS
{
	MOUSE_BUTTON_PRESS = 0,
	MOUSE_BUTTON_RELEASE
};

#endif


#endif