// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenShotEffect.h"
#include "TestDlg.h"
#include "afxdialogex.h"


// CTestDlg dialog

IMPLEMENT_DYNAMIC(CTestDlg, CDialogEx)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestDlg::IDD, pParent)
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestDlg message handlers
