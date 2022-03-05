#include "Input.h"
#include "framework.h"

int Input::GetJump()
{
	if (GetKey(VK_SPACE).bHeld == true)
		return 1;
	return 0;
}

int Input::GetHorizontal()
{
	if (GetKey(VK_LEFT).bHeld == true)
		return -1;
	if (GetKey(VK_RIGHT).bHeld == true)
		return 1;
	return 0;
}


void Input::KeyInput()
{
	for (int i = 0; i < 256; i++)
	{
		m_keyNewState[i] = GetAsyncKeyState(i);

		m_keys[i].bPressed = false;
		m_keys[i].bReleased = false;

		if (m_keyNewState[i] != m_keyOldState[i])
		{
			if (m_keyNewState[i] & 0x8000)
			{
				m_keys[i].bPressed = !m_keys[i].bHeld;
				m_keys[i].bHeld = true;
			}
			else
			{
				m_keys[i].bReleased = true;
				m_keys[i].bHeld = false;
			}
		}
		m_keyOldState[i] = m_keyNewState[i];
	}
}
