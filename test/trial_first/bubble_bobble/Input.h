#pragma once

// Âü°í: https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h
class Input
{
protected:
	struct sKeyState
	{
		bool bPressed;
		bool bReleased;
		bool bHeld;
	} m_keys[256];

	int m_keyNewState[256];
	int m_keyOldState[256];

public:
	int GetJump();
	int GetHorizontal();

	sKeyState GetKey(int nKeyID) { return m_keys[nKeyID]; }

	void KeyInput();
};

