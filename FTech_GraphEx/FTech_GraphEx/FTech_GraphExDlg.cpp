
// FTech_GraphExDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FTech_GraphEx.h"
#include "FTech_GraphExDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFTech_GraphExDlg dialog

CFTech_GraphExDlg::CFTech_GraphExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFTech_GraphExDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFTech_GraphExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_wndGrp);
}

BEGIN_MESSAGE_MAP(CFTech_GraphExDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnType1, &CFTech_GraphExDlg::OnBnClickedBtntype1)
	ON_BN_CLICKED(IDC_BtnType2, &CFTech_GraphExDlg::OnBnClickedBtntype2)
	ON_BN_CLICKED(IDC_BtnType3, &CFTech_GraphExDlg::OnBnClickedBtntype3)
	ON_BN_CLICKED(IDC_BtnType4, &CFTech_GraphExDlg::OnBnClickedBtntype4)
END_MESSAGE_MAP()


// CFTech_GraphExDlg message handlers

BOOL CFTech_GraphExDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_wndGrp.SetCountGraph(1);
	m_wndGrp.SetMaxValueAxis(10,10);

	stCoordinate* pData = new stCoordinate[10];

	pData[0].x = 0;
	pData[0].y = 0.5;

	pData[1].x = 0.5;
	pData[1].y = 1;

	pData[2].x = 1;
	pData[2].y = 1;

	pData[3].x = 1.7;
	pData[3].y = 3;

	pData[4].x = 3;
	pData[4].y = 2;

	pData[5].x = 5;
	pData[5].y = 0.4;

	pData[6].x = 7.7;
	pData[6].y = 7;

	pData[7].x = 8;
	pData[7].y = 10;

	pData[8].x = 9;
	pData[8].y = 5.5;

	pData[9].x = 9.5;
	pData[9].y = 2.4;

	m_wndGrp.SetGraphData(0,pData,10);

	delete []pData;
	pData = NULL;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFTech_GraphExDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFTech_GraphExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFTech_GraphExDlg::OnBnClickedBtntype1()
{
	m_wndGrp.SetColorGrpBkg(255,128,0,0);
	m_wndGrp.SetColorGraphData(0,255,255,255,255);
	m_wndGrp.SetColorGrpBorder(255,0,0,255);
}


void CFTech_GraphExDlg::OnBnClickedBtntype2()
{
	m_wndGrp.LoadGrpImageFromFile(L"bkg.jpg");
}


void CFTech_GraphExDlg::OnBnClickedBtntype3()
{
	m_wndGrp.SetColorTextLabel(255,255,0,0);
}


void CFTech_GraphExDlg::OnBnClickedBtntype4()
{
	m_wndGrp.SetColorTextValue(255,0,255,0);
}
