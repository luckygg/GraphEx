
// FTech_GraphExDlg.h : header file
//

#pragma once
#include "graphex.h"


// CFTech_GraphExDlg dialog
class CFTech_GraphExDlg : public CDialogEx
{
// Construction
public:
	CFTech_GraphExDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FTECH_GRAPHEX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CGraphEx m_wndGrp;
	afx_msg void OnBnClickedBtntype1();
	afx_msg void OnBnClickedBtntype2();
	afx_msg void OnBnClickedBtntype3();
	afx_msg void OnBnClickedBtntype4();
};
