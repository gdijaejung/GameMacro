
#include <windows.h>
#include <stdio.h>

void main()
{
	while (true)
	{
// 		INPUT input = { 0 };
// 		input.type = INPUT_MOUSE;
// 		input.mi.dx = 0;
// 		input.mi.dy = 0;
// 		input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN);
// 		input.mi.mouseData = 0;
// 		input.mi.dwExtraInfo = NULL;
// 		input.mi.time = 0;
// 		SendInput(1, &input, sizeof(INPUT));

//		Sleep(3000);
// 
// 		INPUT input2 = { 0 };
// 		input2.type = INPUT_MOUSE;
// 		input2.mi.dx = 0;
// 		input2.mi.dy = 0;
// 		input2.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP);
// 		input2.mi.mouseData = 0;
// 		input2.mi.dwExtraInfo = NULL;
// 		input2.mi.time = 0;
// 		SendInput(1, &input2, sizeof(INPUT));
// 		Sleep(1000);

		INPUT input;
		WORD vkey = VK_DOWN; // see link below
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		input.ki.wVk = vkey;
		input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
		SendInput(1, &input, sizeof(INPUT));

		Sleep(100);

		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));

		Sleep(3000);
	}
}
