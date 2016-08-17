
// FTech_GraphEx.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFTech_GraphExApp:
// See FTech_GraphEx.cpp for the implementation of this class
//

class CFTech_GraphExApp : public CWinApp
{
public:
	CFTech_GraphExApp();

	ULONG_PTR m_gdiplusToken;
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFTech_GraphExApp theApp;