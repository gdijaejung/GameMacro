
#include <iostream>
#include <windows.h>
#include <string>

using namespace std;


void main()
{
	cout << "hello" << endl;

	HWND hwnd = FindWindow(L"NeonClass_41", L"DiRT 3");

	cout << hwnd << endl;

}

