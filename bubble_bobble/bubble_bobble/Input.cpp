#include "Input.h"
#include "framework.h"

char Input::GetChar()
{	
	for (int i = 'A'; i < 'Z'; i++)
	{
		if (onceState[i] == true) return i;
	}
	return '0';
}

int Input::GetUpDown()
{
	if (onceState[VK_UP] == true) return -1;
	if (onceState[VK_DOWN] == true) return 1;
	return 0;
}

int Input::GetBackspace()
{
	if (onceState[VK_BACK] == true) return 1;
	return 0;
}

int Input::GetEnter()
{
	if (GetKey(VK_RETURN).pressed == true) return 1;
	return 0;
}

int Input::GetEsc()
{
	if (GetKey(VK_ESCAPE).pressed == true) return 1;
	return 0;
}

int Input::GetAttack()
{
	if (GetKey('S').pressed == true) return 1;
	return 0;
}

int Input::GetJump()
{
	if (GetKey(VK_SPACE).pressed == true) return 1;
	return 0;
}

int Input::GetHorizontal()
{
	if (GetKey(VK_LEFT).held == true) return -1;
	if (GetKey(VK_RIGHT).held == true) return 1;
	return 0;
}

void Input::KeyInput()
{
	for (int i = 0; i < 256; i++)
	{
		newState[i] = GetAsyncKeyState(i);

		keys[i].pressed = false;
		keys[i].released = false;
		onceState[i] = false;

		if (newState[i] != oldState[i])
		{
			if (newState[i] & 0x8000)
			{
				keys[i].pressed = !keys[i].held;
				keys[i].held = true;
			}
			else
			{
				keys[i].released = true;
				keys[i].held = false;
			}
			if (newState[i] & 0x0001)
			{
				onceState[i] = true;
			}
		}
		oldState[i] = newState[i];
	}
}

void Input::KeyClear()
{
	for (int i = 0; i < 256; i++)
	{
		keys[i].pressed = false;
		keys[i].released = false;
		keys[i].held = false;
		onceState[i] = false;
	}
}
