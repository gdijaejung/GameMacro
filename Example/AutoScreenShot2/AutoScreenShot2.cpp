// AutoScreenShot2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AutoScreenShot2.h"
#include <iostream>
#include <atlimage.h>

#include <gdiplus.h> 
#pragma comment( lib, "gdiplus.lib" ) 
using namespace Gdiplus;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL Capture(HWND hTargetWnd, LPCTSTR lpszFilePath);
BOOL Save(HBITMAP& hBitmap, CString strFilePath);
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

// The one and only application object

CWinApp theApp;

using namespace std;

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
			// TODO: code your application's behavior here.
			ULONG_PTR gdiplusToken;
			GdiplusStartupInput GdiplusStartupInput;
			if (::GdiplusStartup(&gdiplusToken, &GdiplusStartupInput, NULL) != Ok)
			{
				AfxMessageBox(_T("Error : Failed to initialize GDI+ library!"));
				return FALSE;
			}

			cout << "screen shot" << endl;
			HWND hwnd = FindWindow(L"NeonClass_41", L"DiRT 3");
			//HWND hwnd = FindWindow(L"HwndWrapper[DefaultDomain;;4cda6b0d-23f2-4887-bf43-c7711d2b95f3]", L"GitHub");
			//HWND hwnd = FindWindow(L"Notepad", L"���� ���� - �޸���");
			cout << hwnd << endl;

			SetFocus(hwnd);
			Capture(hwnd, L"test1.bmp");

// 			CRect cr;
// 			GetWindowRect(hwnd, &cr);
// 
// 			CDC *pDC = CDC::FromHandle(::GetWindowDC(hwnd));
// 			CDC memDC;
// 			memDC.CreateCompatibleDC(pDC);
// 
// 			CBitmap m_Bitmap;
// 			m_Bitmap.CreateCompatibleBitmap(&memDC, cr.Width(), cr.Height());
// 			CBitmap *pOldBitmap = memDC.SelectObject(&m_Bitmap);
// 			memDC.Rectangle(CRect(0,0,cr.Width(), cr.Height()));
// 			//memDC.BitBlt(0, 0, cr.Width(), cr.Height(), pDC, 0, 0, SRCCOPY);
// 			PrintWindow(hwnd, memDC.GetSafeHdc(),0);
// 
// 			memDC.TextOutW(5, 0, L"test"); // maximum y
// 
// 			CImage img;
// 			img.Attach((HBITMAP)m_Bitmap.GetSafeHandle());
// 			img.Save(L"test1.jpg");

			::GdiplusShutdown(gdiplusToken);
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


BOOL Capture(HWND hTargetWnd, LPCTSTR lpszFilePath)
{
	CRect rct;
	if (hTargetWnd)
		::GetWindowRect(hTargetWnd, &rct);
	else
		return FALSE;

	HBITMAP hBitmap = NULL;
	HBITMAP hOldBitmap = NULL;
	BOOL bSuccess = FALSE;

	HDC hDC = ::GetDC(hTargetWnd);
	HDC hMemDC = ::CreateCompatibleDC(hDC);
	hBitmap = ::CreateCompatibleBitmap(hDC, rct.Width(), rct.Height());

	if (!hBitmap)
		return FALSE;

	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	if (!::PrintWindow(hTargetWnd, hMemDC, 0))
		bSuccess = FALSE;
	else
		bSuccess = TRUE;

	//Save �Լ� : http://blog.daum.net/odega/25
	if (bSuccess && !Save(hBitmap, lpszFilePath))
		bSuccess = FALSE;
	else
		bSuccess = TRUE;

	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hBitmap);
	::DeleteDC(hMemDC);

	return bSuccess;
}

BOOL Save(HBITMAP& hBitmap, CString strFilePath)
{
	if (!hBitmap)
		return FALSE;

	Gdiplus::Bitmap gBmp(hBitmap, NULL);
	CLSID clsid;
	GetEncoderClsid(_T("image/bmp"), &clsid);//HBITMAP�� bmp���Ϸ� ����
	if (gBmp.Save(strFilePath, &clsid) == Gdiplus::Ok)
		return TRUE;//����
	else
		return FALSE;//����

	return TRUE;
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // �̹��� ���ڴ��� ����
	UINT  size = 0;         // �̹��� ���ڴ� �迭�� ����Ʈ ũ��

	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);

	if (size == 0)
		return -1;  //����

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));

	if (pImageCodecInfo == NULL)
		return -1;  //����

	GetImageEncoders(num, size, pImageCodecInfo);//�̹��� ���ڴ� ������ ����

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)//����ڰ� �䱸�� �׷��� ���¿� ��ġ�ϸ�
		{
			*pClsid = pImageCodecInfo[j].Clsid;//�ڵ����̵� ����
			free(pImageCodecInfo);
			return j;  //����
		}
	}

	free(pImageCodecInfo);//����
	return -1;  //����
}