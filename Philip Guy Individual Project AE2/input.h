#pragma once

class input
{
private:
	~input();
public:
	HRESULT InitialiseInput(HINSTANCE g_hInst, HWND g_hWnd);
	void ReadInputStates();
	bool IsKeyPressed(unsigned char DI_keycode);
};