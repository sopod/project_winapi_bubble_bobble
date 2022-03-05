#pragma once

class Input
{
protected:
	struct KeyState
	{
		bool pressed;
		bool released;
		bool held;
	} keys[256];

	int newState[256];
	int oldState[256];

	bool onceState[256];

public:
	KeyState GetKey(int keyID) { return keys[keyID]; }

	char GetChar();
	int GetUpDown();
	int GetBackspace();

	int GetEnter();
	int GetEsc();

	int GetAttack();
	int GetJump();
	int GetHorizontal();

	void KeyInput();
	void KeyClear();
};	