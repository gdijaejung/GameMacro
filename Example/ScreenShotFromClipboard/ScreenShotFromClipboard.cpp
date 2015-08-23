// ScreenShot3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ScreenShotFromClipboard.h"
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

namespace std
{
	typedef unsigned int uint32_t;
	typedef int int32_t;
	typedef unsigned short uint16_t;
	typedef short int16_t;
}


typedef struct
{
	std::uint32_t biSize;
	std::int32_t  biWidth;
	std::int32_t  biHeight;
	std::uint16_t  biPlanes;
	std::uint16_t  biBitCount;
	std::uint32_t biCompression;
	std::uint32_t biSizeImage;
	std::int32_t  biXPelsPerMeter;
	std::int32_t  biYPelsPerMeter;
	std::uint32_t biClrUsed;
	std::uint32_t biClrImportant;
} DIB;

typedef struct
{
	std::uint16_t type;
	std::uint32_t bfSize;
	std::uint32_t reserved;
	std::uint32_t offset;
} HEADER;

typedef struct
{
	HEADER header;
	DIB dib;
} BMP;



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			HWND hwnd = FindWindow(L"NeonClass_41", L"DiRT 3");
			//HWND hwnd = FindWindow(L"HwndWrapper[DefaultDomain;;4cda6b0d-23f2-4887-bf43-c7711d2b95f3]", L"GitHub");
			//HWND hwnd = FindWindow(L"Notepad", L"제목 없음 - 메모장");
			cout << "screenshot " << hwnd << endl;

			cout << "focus window delay" << endl;

			int cnt = 0;
			while (cnt < 100)
			{
				if (SetForegroundWindow(hwnd))
					break;
				Sleep(100);
				++cnt;
			}

			cout << "capture" << endl;

			// press screen shot button
			keybd_event(VK_LMENU, 0, KEYEVENTF_EXTENDEDKEY, 0);
			Sleep(100);
			keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_EXTENDEDKEY, 0);
			Sleep(100);

			keybd_event(VK_LMENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);


			// 클립보드 내용 가져와서 파일로 저장
			::OpenClipboard(NULL);

			std::cout << "Format Bitmap: " << IsClipboardFormatAvailable(CF_BITMAP) << "\n";
			std::cout << "Format DIB: " << IsClipboardFormatAvailable(CF_DIB) << "\n";
			std::cout << "Format DIBv5: " << IsClipboardFormatAvailable(CF_DIBV5) << "\n";

			HANDLE hClipboard = GetClipboardData(CF_BITMAP);
			if (!hClipboard)
				return 0;

			if (IsClipboardFormatAvailable(CF_BITMAP) || IsClipboardFormatAvailable(CF_DIB) || IsClipboardFormatAvailable(CF_DIBV5))
			{
				if (OpenClipboard(NULL))
				{
					HANDLE hClipboard = GetClipboardData(CF_DIB);

					if (!hClipboard)
					{
						hClipboard = GetClipboardData(CF_DIBV5);
					}

					if (hClipboard != NULL && hClipboard != INVALID_HANDLE_VALUE)
					{
						void* dib = GlobalLock(hClipboard);

						if (dib)
						{
							DIB *info = reinterpret_cast<DIB*>(dib);
							BMP bmp = { 0 };
							bmp.header.type = 0x4D42;
							bmp.header.offset = 54;
							bmp.header.bfSize = info->biSizeImage + bmp.header.offset;
							bmp.dib = *info;

							std::cout << "Type: " << std::hex << bmp.header.type << std::dec << "\n";
							std::cout << "bfSize: " << bmp.header.bfSize << "\n";
							std::cout << "Reserved: " << bmp.header.reserved << "\n";
							std::cout << "Offset: " << bmp.header.offset << "\n";
							std::cout << "biSize: " << bmp.dib.biSize << "\n";
							std::cout << "Width: " << bmp.dib.biWidth << "\n";
							std::cout << "Height: " << bmp.dib.biHeight << "\n";
							std::cout << "Planes: " << bmp.dib.biPlanes << "\n";
							std::cout << "Bits: " << bmp.dib.biBitCount << "\n";
							std::cout << "Compression: " << bmp.dib.biCompression << "\n";
							std::cout << "Size: " << bmp.dib.biSizeImage << "\n";
							std::cout << "X-res: " << bmp.dib.biXPelsPerMeter << "\n";
							std::cout << "Y-res: " << bmp.dib.biYPelsPerMeter << "\n";
							std::cout << "ClrUsed: " << bmp.dib.biClrUsed << "\n";
							std::cout << "ClrImportant: " << bmp.dib.biClrImportant << "\n";

							std::ofstream file("Test.bmp", std::ios::out | std::ios::binary);
							if (file)
							{
								file.write(reinterpret_cast<char*>(&bmp.header.type), sizeof(bmp.header.type));
								file.write(reinterpret_cast<char*>(&bmp.header.bfSize), sizeof(bmp.header.bfSize));
								file.write(reinterpret_cast<char*>(&bmp.header.reserved), sizeof(bmp.header.reserved));
								file.write(reinterpret_cast<char*>(&bmp.header.offset), sizeof(bmp.header.offset));
								file.write(reinterpret_cast<char*>(&bmp.dib.biSize), sizeof(bmp.dib.biSize));
								file.write(reinterpret_cast<char*>(&bmp.dib.biWidth), sizeof(bmp.dib.biWidth));
								file.write(reinterpret_cast<char*>(&bmp.dib.biHeight), sizeof(bmp.dib.biHeight));
								file.write(reinterpret_cast<char*>(&bmp.dib.biPlanes), sizeof(bmp.dib.biPlanes));
								file.write(reinterpret_cast<char*>(&bmp.dib.biBitCount), sizeof(bmp.dib.biBitCount));
								file.write(reinterpret_cast<char*>(&bmp.dib.biCompression), sizeof(bmp.dib.biCompression));
								file.write(reinterpret_cast<char*>(&bmp.dib.biSizeImage), sizeof(bmp.dib.biSizeImage));
								file.write(reinterpret_cast<char*>(&bmp.dib.biXPelsPerMeter), sizeof(bmp.dib.biXPelsPerMeter));
								file.write(reinterpret_cast<char*>(&bmp.dib.biYPelsPerMeter), sizeof(bmp.dib.biYPelsPerMeter));
								file.write(reinterpret_cast<char*>(&bmp.dib.biClrUsed), sizeof(bmp.dib.biClrUsed));
								file.write(reinterpret_cast<char*>(&bmp.dib.biClrImportant), sizeof(bmp.dib.biClrImportant));
								file.write(reinterpret_cast<char*>(info + 1), bmp.dib.biSizeImage);
							}

							GlobalUnlock(dib);
						}
					}

					CloseClipboard();
				}
			}


			CloseClipboard();


		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
