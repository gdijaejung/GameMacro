// ScreenShotEffect.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ScreenShotEffect.h"
#include <iostream>
#include <fstream>
#include <windows.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cvaux.h"

#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#pragma comment(lib, "opencv_core2411d.lib")
#pragma comment(lib, "opencv_highgui2411d.lib")
#pragma comment(lib, "opencv_imgproc2411.lib")
#else
#pragma comment(lib, "opencv_core2411.lib")
#pragma comment(lib, "opencv_highgui2411.lib")
#pragma comment(lib, "opencv_imgproc2411.lib")
#endif


using namespace std;
using namespace cv;


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

IplImage *g_image = NULL;
IplImage* output = 0;
IplImage* gray = 0;

bool FocusWindow();
void PressScreenShot();
IplImage* GetClipboardImage();


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
			cout << "screenshot start..." << endl;

			int threshold = 172;
			cvNamedWindow("screenshot", 0);
			cvResizeWindow("screenshot", 640, 480);
			cvCreateTrackbar("Threthold", "screenshot", &threshold, 255, NULL); // "HUV05-camera" 윈도우에 bar 를 붙이고 

			cvNamedWindow("screenshot2", 0);
			cvResizeWindow("screenshot2", 640, 480);

			cout << "capture loop " << endl;

			while (1)
			{
				cout << "focus window.." << endl;
				if (!FocusWindow())
				{
					const int key = waitKey(1000);
					if (key == VK_ESCAPE)
						break;
					continue;
				}

				cout << "press screenshot " << endl;
				PressScreenShot();

				cout << "get clipboard image" << endl;
				IplImage *img = GetClipboardImage();
				if (img)
				{
					if (!output)
					{
						gray = cvCreateImage({img->width, img->height}, IPL_DEPTH_8U, 1); // 흑백 이미지 생성
						output = cvCreateImage({ img->width, img->height}, IPL_DEPTH_8U, 1); // 흑백 이미지 생성
					}
					else if ((output->width != img->width) ||
						(output->height != img->height))
					{
						gray = cvCreateImage({ img->width, img->height }, IPL_DEPTH_8U, 1); // 흑백 이미지 생성
						output = cvCreateImage({ img->width, img->height }, IPL_DEPTH_8U, 1); // 흑백 이미지 생성
					}
					
					cout << "binarization" << endl;
					cvCvtColor(img, gray, CV_RGB2GRAY); // 컬러를 흑백으로 변환
					cvThreshold(gray, output, threshold, 255, CV_THRESH_BINARY);
					output->origin = img->origin; // 방향이 뒤집어 진것을 바로 잡아줌

					imshow("screenshot", Mat(output));
					imshow("screenshot2", Mat(img));
					waitKey(1);
				}

				cout << "wait" << endl << endl;
				const int key = waitKey(5000);
				if (key == VK_ESCAPE)
					break;
			}

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


bool FocusWindow()
{
	HWND hwnd = FindWindow(L"NeonClass_41", L"DiRT 3");
	cout << "focus window..." << hwnd << endl;

	if (!hwnd)
		return false;

	int cnt = 0;
	while (cnt < 100)
	{
		if (SetForegroundWindow(hwnd))
			break;
		Sleep(100);
		++cnt;
	}

	return true;
}


void PressScreenShot()
{
	// press screen shot button
// 	keybd_event(VK_LMENU, 0, KEYEVENTF_EXTENDEDKEY, 0);
// 	Sleep(100);
// 	keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_EXTENDEDKEY, 0);
// 	Sleep(100);
// 	keybd_event(VK_LMENU, 0, KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP, 0);
// 	keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP, 0);

	const int delayTime = 50;

	// KeyDown
	INPUT input;
	WORD vkey = VK_LMENU; // see link below
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = vkey;
	input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
	SendInput(1, &input, sizeof(INPUT));
	Sleep(delayTime);

	input.ki.wScan = MapVirtualKey(VK_SNAPSHOT, MAPVK_VK_TO_VSC);
	input.ki.wVk = VK_SNAPSHOT;
	SendInput(1, &input, sizeof(INPUT));
	Sleep(delayTime);

	// KeyUp
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
	Sleep(delayTime);

	input.ki.wScan = MapVirtualKey(VK_LMENU, MAPVK_VK_TO_VSC);
	input.ki.wVk = VK_LMENU;
	SendInput(1, &input, sizeof(INPUT));
	Sleep(delayTime);
}


IplImage* GetClipboardImage()
{
	std::cout << "Format Bitmap: " << IsClipboardFormatAvailable(CF_BITMAP) << "\n";
	std::cout << "Format DIB: " << IsClipboardFormatAvailable(CF_DIB) << "\n";
	std::cout << "Format DIBv5: " << IsClipboardFormatAvailable(CF_DIBV5) << "\n";


	if (IsClipboardFormatAvailable(CF_BITMAP) || IsClipboardFormatAvailable(CF_DIB) || IsClipboardFormatAvailable(CF_DIBV5))
	{
		if (OpenClipboard(NULL))
		{
			HANDLE hClipboard = GetClipboardData(CF_DIB);

			if (!hClipboard)
			{
				hClipboard = GetClipboardData(CF_DIBV5);
				cout << "No CF_DIB clipboard" << endl;
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

// 					std::cout << "Type: " << std::hex << bmp.header.type << std::dec << "\n";
// 					std::cout << "bfSize: " << bmp.header.bfSize << "\n";
// 					std::cout << "Reserved: " << bmp.header.reserved << "\n";
// 					std::cout << "Offset: " << bmp.header.offset << "\n";
// 					std::cout << "biSize: " << bmp.dib.biSize << "\n";
// 					std::cout << "Width: " << bmp.dib.biWidth << "\n";
// 					std::cout << "Height: " << bmp.dib.biHeight << "\n";
// 					std::cout << "Planes: " << bmp.dib.biPlanes << "\n";
// 					std::cout << "Bits: " << bmp.dib.biBitCount << "\n";
// 					std::cout << "Compression: " << bmp.dib.biCompression << "\n";
// 					std::cout << "Size: " << bmp.dib.biSizeImage << "\n";
// 					std::cout << "X-res: " << bmp.dib.biXPelsPerMeter << "\n";
// 					std::cout << "Y-res: " << bmp.dib.biYPelsPerMeter << "\n";
// 					std::cout << "ClrUsed: " << bmp.dib.biClrUsed << "\n";
// 					std::cout << "ClrImportant: " << bmp.dib.biClrImportant << "\n";

					if (!g_image)
					{
						g_image = cvCreateImage(cvSize(bmp.dib.biWidth, bmp.dib.biHeight),
							IPL_DEPTH_8U, bmp.dib.biBitCount / 8);
					}
					else
					{
						if ((g_image->width != bmp.dib.biWidth) ||
							(g_image->height != bmp.dib.biHeight))
						{
							cvReleaseImage(&g_image);

							g_image = cvCreateImage(cvSize(bmp.dib.biWidth, bmp.dib.biHeight),
								IPL_DEPTH_8U, bmp.dib.biBitCount / 8);
						}
					}

					CopyMemory(g_image->imageData, (info + 1), bmp.dib.biSizeImage);
					cvFlip(g_image);

					GlobalUnlock(dib);
				}
				else
				{
					cout << "Error GlobalLock()" << endl;
				}
			}
			else
			{
				cout << "No CF_DIBV5 clipboard" << endl;
			}

			CloseClipboard();
		}
		else
		{
			cout << "OpenClipboard() Error" << endl;
		}
	}
	else
	{
		cout << "Is NoT DIB fomrat" << endl;
	}

	return g_image;
}
