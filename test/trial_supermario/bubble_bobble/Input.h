#pragma once

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
	int GetVertical();

	sKeyState GetKey(int nKeyID) { return m_keys[nKeyID]; }

	void KeyInput();
};