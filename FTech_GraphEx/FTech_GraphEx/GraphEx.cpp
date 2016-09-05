// CGraphEx.cpp : implementation file
//

#include "stdafx.h"
#include "GraphEx.h"

// CGraphEx
CGraphEx::CGraphEx()
{
	RegisterWindowClass();
	m_pBmpFrmBkg		= NULL;
	m_pBmpGrpBkg		= NULL;
	m_clrFrmBkg			= Color(255,41,41,41);
	m_clrFrmBorder		= Color(255,128,128,255);
	m_clrGrpBkg			= Color(255,71,71,71);
	m_clrGrpBorder		= Color(255,128,128,255);
	m_clrGrid			= Color(255,128,128,128);
	m_clrScale			= Color(255,255,255,255);
	m_clrTextValue		= Color(255,255,255,255);
	m_clrTextLabel		= Color(255,255,255,255);
	m_clrInfoBkg		= Color(255,128,128,128);
	m_fSizeFrmBorder	= 4;
	m_fSizeGrpBorder	= 2;
	m_fSizeInfo			= 12;
	m_fSizeValue		= 14;
	m_fSizeLabel		= 15;
	m_fSizeGrid			= 1;

	m_rcGrp = CRect(0,0,0,0);
	m_szInfo = CSize(65,50);
	
	m_dMaxAxisX		 = 10.0;
	m_dMaxAxisY		 = 55.0;
	m_nCntScaleAxisX = 10;
	m_nCntScaleAxisY = 10;
	m_nCntGraph		 = 0;

	m_pStGraphData  = NULL;

	m_bDrawGrid		= true;
	m_bDrawScale	= true;
	m_bShowLabel	= true;
	m_bShowValueX	= true;
	m_bShowValueY	= true;
	m_bShowOrigin	= true;
	m_bShowInteger	= false;

	m_strAxisX = L"Label X";
	m_strAxisY = L"Label Y";

	m_nCurrGrpPoint = 0;
	m_nCurrGrpData	= 0;
	m_bDrawInfo = false;
	m_pArrPosGrp = NULL;
	m_ptMouse = CPoint(0,0);
}

CGraphEx::~CGraphEx()
{
	if (m_pBmpFrmBkg != NULL)
	{
		delete m_pBmpFrmBkg;
		m_pBmpFrmBkg = NULL;
	}

	if (m_pBmpGrpBkg != NULL)
	{
		delete m_pBmpGrpBkg;
		m_pBmpGrpBkg = NULL;
	}

	if (m_pStGraphData != NULL)
	{
		for (int i=0; i<m_nCntGraph; i++)
		{
			if (m_pStGraphData[i].arrGraphData.GetSize() != 0)
				m_pStGraphData[i].arrGraphData.RemoveAll();
		}

		delete []m_pStGraphData;
		m_pStGraphData = NULL;
	}

	if (m_pArrPosGrp != NULL)
	{
		int i=0;
		do{
			if (m_pArrPosGrp[i].GetSize() != 0)
				m_pArrPosGrp[i].RemoveAll();

			i++;
		}while(i<m_nCntGraph);

		delete []m_pArrPosGrp;
		m_pArrPosGrp = NULL;
	}
}

void CGraphEx::RegisterWindowClass(void)
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc      = ::DefWindowProc;
	wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
	wndcls.hInstance        = hInst;
	wndcls.hIcon            = NULL;
	wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
	wndcls.lpszMenuName     = NULL;
	wndcls.lpszClassName    = WINCLASSNAME_GRAPHEX;

	AfxRegisterClass(&wndcls);

}
void CGraphEx::PreSubclassWindow()
{
	GetClientRect(m_rcFrm);	

	m_rcGrp.SetRect(m_rcFrm.left + 75, m_rcFrm.top + 10, m_rcFrm.right - 35, m_rcFrm.bottom - 30);

	CWnd::PreSubclassWindow();
}

BEGIN_MESSAGE_MAP(CGraphEx, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CGraphEx message handlers

void CGraphEx::OnPaint() 
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	Graphics mainG (dc.GetSafeHdc());

	// 메모리 생성
	Bitmap memBmp(rect.Width(), rect.Height());

	Graphics memG(&memBmp);

	// 잔상을 없애기 위해 white 채움.
	SolidBrush brs(Color(255,0,0,0));
	memG.FillRectangle(&brs,0,0,rect.Width(),rect.Height());

	//----- 메모리상에 그리는 부분 -----//
	DrawFrameBkg(&memG);
	DrawFrameBorder(&memG);
	DrawGraphBkg(&memG);
	DrawGraphBorder(&memG);
	DrawScaleGridValue(&memG);
	DrawGraphData(&memG);
	DrawLabelText(&memG);
	DrawGraphDataInfo(&memG);
	//---------------------------------//

	mainG.DrawImage(&memBmp,0,0);
}

void CGraphEx::SetGraphData(int nIndex, stCoordinate* pData, int nArrSize)
{
	if (m_pStGraphData == NULL) return;
	ClearGraphData(nIndex);

	if (pData == NULL) return;
	if (nIndex < 0 || nIndex > m_nCntGraph-1) return;

	double dStepAxisX = (double) m_rcGrp.Width () / (m_nCntScaleAxisX+1); //0과 Max의 눈금 때문에 +1을 함.
	double dStepAxisY = (double) m_rcGrp.Height() / (m_nCntScaleAxisY+1); 
	double dValueAxisX= m_dMaxAxisX / m_nCntScaleAxisX;
	double dValueAxisY= m_dMaxAxisY / m_nCntScaleAxisY;
	double y_start = m_rcGrp.top + dStepAxisY;
	double y_end   = m_rcGrp.top + dStepAxisY*(m_nCntScaleAxisY+1);
	double x_start = m_rcGrp.left;
	double x_end   = m_rcGrp.left + dStepAxisX*(m_nCntScaleAxisX);
	double x_pos=0.0, y_pos=0.0;

	if (m_pArrPosGrp[nIndex].GetSize() != 0)
		m_pArrPosGrp[nIndex].RemoveAll();

	for (int i=0; i<nArrSize; i++)
	{
		stCoordinate coordinate;
		coordinate.x = pData[i].x;
		coordinate.y = pData[i].y;
		m_pStGraphData[nIndex].arrGraphData.Add(coordinate);

		y_pos = y_start + (y_end - y_start)*(m_dMaxAxisY - pData[i].y)/m_dMaxAxisY;
		x_pos = x_start + (x_end - x_start)*(pData[i].x)/m_dMaxAxisX;

		m_pArrPosGrp[nIndex].Add(PointF((REAL)x_pos,(REAL)y_pos));
	}
}

void CGraphEx::SetGraphData(int nIndex, int* pXdata, int* pYdata, int nArrSize)
{
	if (m_pStGraphData == NULL) return;
	ClearGraphData(nIndex);

	if (pXdata == NULL) return;
	if (pYdata == NULL) return;
	if (nIndex < 0 || nIndex > m_nCntGraph-1) return;

	double dStepAxisX = (double) m_rcGrp.Width () / (m_nCntScaleAxisX+1); //0과 Max의 눈금 때문에 +1을 함.
	double dStepAxisY = (double) m_rcGrp.Height() / (m_nCntScaleAxisY+1); 
	double dValueAxisX= m_dMaxAxisX / m_nCntScaleAxisX;
	double dValueAxisY= m_dMaxAxisY / m_nCntScaleAxisY;
	double y_start = m_rcGrp.top + dStepAxisY;
	double y_end   = m_rcGrp.top + dStepAxisY*(m_nCntScaleAxisY+1);
	double x_start = m_rcGrp.left;
	double x_end   = m_rcGrp.left + dStepAxisX*(m_nCntScaleAxisX);
	double x_pos=0.0, y_pos=0.0;

	if (m_pArrPosGrp[nIndex].GetSize() != 0)
		m_pArrPosGrp[nIndex].RemoveAll();

	for (int i=0; i<nArrSize; i++)
	{
		stCoordinate coordinate;
		coordinate.x = pXdata[i];
		coordinate.y = pYdata[i];
		m_pStGraphData[nIndex].arrGraphData.Add(coordinate);

		y_pos = y_start + (y_end - y_start)*(m_dMaxAxisY - pYdata[i])/m_dMaxAxisY;
		x_pos = x_start + (x_end - x_start)*(pXdata[i])/m_dMaxAxisX;

		m_pArrPosGrp[nIndex].Add(PointF((REAL)x_pos,(REAL)y_pos));
	}
}

void CGraphEx::ClearGraphData(int nIndex)
{
	if (m_pStGraphData == NULL) return;
	if (nIndex < 0) return;

	if (m_pStGraphData[nIndex].arrGraphData.GetSize() > 0)
		m_pStGraphData[nIndex].arrGraphData.RemoveAll();

	Invalidate(FALSE);
}

void CGraphEx::ClearAllGraphData()
{
	if (m_pStGraphData == NULL) return;

	for (int i=0; i<m_nCntGraph; i++)
	{
		if (m_pStGraphData[i].arrGraphData.GetSize() > 0)
			m_pStGraphData[i].arrGraphData.RemoveAll();
	}

	Invalidate(FALSE);
}

void CGraphEx::SetCountGraph(int nMax)
{
	if (nMax <0) return;

	if (m_pStGraphData != NULL)
	{
		int i=0;
		do{
			if (m_pStGraphData[i].arrGraphData.GetSize() != 0)
				m_pStGraphData[i].arrGraphData.RemoveAll();

			i++;
		}while(i<m_nCntGraph);

		delete []m_pStGraphData;
		m_pStGraphData = NULL;
	}
	
	m_pStGraphData = new stGraphData[nMax];
	

	for (int i=0; i<nMax; i++)
	{
		m_pStGraphData[i].clrGraphData	= Color(255,144 + 10* i,193 + 0.1 * i,215 + 5 * i);
		m_pStGraphData[i].clrGraphPoint  = Color(255,72 + 10* i,147 + 0.1 * i,172 + 5 * i);
		m_pStGraphData[i].fSizeGraph	 = 1;
		m_pStGraphData[i].nSizeGraphPoint= 3;
		m_pStGraphData[i].bDrawGraph = true;
		m_pStGraphData[i].bDrawGraphPoint = true;
		m_pStGraphData[i].arrGraphData.RemoveAll();
	}

	if (m_pArrPosGrp != NULL)
	{
		int i=0;
		do{
			if (m_pArrPosGrp[i].GetSize() != 0)
				m_pArrPosGrp[i].RemoveAll();

			i++;
		}while(i<m_nCntGraph);

		delete []m_pArrPosGrp;
		m_pArrPosGrp = NULL;
	}

	m_pArrPosGrp = new CArray<PointF>[nMax];

	m_nCntGraph = nMax;
}

void CGraphEx::DrawFrameBkg(Graphics* pG)
{
	CRect rect;
	GetClientRect(&rect);

	if (m_pBmpFrmBkg == NULL)
	{
		SolidBrush brs(m_clrFrmBkg);
		pG->FillRectangle(&brs,Rect(rect.left, rect.top, rect.Width(), rect.Height()));
	}
	else
		pG->DrawImage(m_pBmpFrmBkg, Rect(rect.left, rect.top, rect.Width(), rect.Height()), 0,0, m_pBmpFrmBkg->GetWidth(), m_pBmpFrmBkg->GetHeight(), UnitPixel);
}

void CGraphEx::DrawFrameBorder(Graphics* pG)
{
	CRect rect;
	GetClientRect(&rect);

	// 이미지가 있으면 그리지 않는다.
	if (m_pBmpFrmBkg != NULL) return;
	
	Pen pen(m_clrFrmBorder,m_fSizeFrmBorder);

	pG->DrawRectangle(&pen,rect.left,rect.top,rect.Width(),rect.Height());
}

void CGraphEx::DrawGraphBkg(Graphics* pG)
{
	if (m_pBmpGrpBkg == NULL)
	{
		SolidBrush brs(m_clrGrpBkg);
		pG->FillRectangle(&brs,Rect(m_rcGrp.left, m_rcGrp.top, m_rcGrp.Width(), m_rcGrp.Height()));
	}
	else
		pG->DrawImage(m_pBmpGrpBkg, Rect(m_rcGrp.left, m_rcGrp.top, m_rcGrp.Width(), m_rcGrp.Height()), 0,0, m_pBmpGrpBkg->GetWidth(), m_pBmpGrpBkg->GetHeight(), UnitPixel);
}

void CGraphEx::DrawGraphBorder(Graphics* pG)
{
	// 이미지가 있으면 그리지 않는다.
	if (m_pBmpGrpBkg != NULL) return;

	Pen pen(m_clrGrpBorder,m_fSizeGrpBorder);

	pG->DrawRectangle(&pen,m_rcGrp.left,m_rcGrp.top,m_rcGrp.Width(),m_rcGrp.Height());
}

void CGraphEx::DrawGraphData(Graphics* pG)
{
	if (m_nCntGraph == 0) return;

	pG->SetSmoothingMode(SmoothingModeAntiAlias);

	for (int i=0; i<m_nCntGraph; i++)
	{
		if (m_pStGraphData[i].bDrawGraph == false) continue;

		Pen data_pen(m_pStGraphData[i].clrGraphData,m_pStGraphData[i].fSizeGraph);
		data_pen.SetLineJoin(LineJoinRound);
		
		SolidBrush brs(m_pStGraphData[i].clrGraphPoint);

		int nSize = m_pStGraphData[i].arrGraphData.GetSize();

		PointF *pData = NULL;
		pData = new PointF[nSize];
		memset(pData, 0, nSize);

		// for 문을 빼기위해 멤버로 이중 포인터 PointF** 를 추가해 봤지만, Debug 및 Release 에서 실제 tact time의 차이는 크게 없음.
		// X축 4096개의 데이터로 처리해 보았지만 차이 없었음.
		for (int j=0; j<nSize; j++)
		{
			PointF pt = m_pArrPosGrp[i].GetAt(j);
			
			pData[j].X = pt.X;
			pData[j].Y = pt.Y;			
		}

		pG->DrawLines(&data_pen,pData,nSize);
		
		for (int j=0; j<nSize; j++)
		{
			if (m_pStGraphData[i].bDrawGraphPoint)
			{
				int size = m_pStGraphData[i].nSizeGraphPoint;
				PointF pt = m_pArrPosGrp[i].GetAt(j);
				
				pG->FillEllipse(&brs,(REAL)(pt.X-size), (REAL)(pt.Y-size), (REAL)size*2, (REAL)size*2);
			}
		}

		delete pData;
		pData = NULL;
	}
}

void CGraphEx::DrawGraphDataInfo(Graphics* pG)
{
	CRect rect;
	GetClientRect(&rect);

	if (m_bDrawInfo == false) return;
	FontFamily fontptroll(L"Arial");
	Gdiplus::Font font(&fontptroll, m_fSizeInfo, FontStyleRegular, UnitPixel);
	
	StringFormat formatAlign;
	formatAlign.SetAlignment(StringAlignmentNear);
	formatAlign.SetLineAlignment(StringAlignmentNear);
	
	REAL orgX=0,orgY=0;
	if (m_ptMouse.x+m_szInfo.cx > rect.right)
		orgX = (REAL)(rect.right - m_szInfo.cx);
	else
		orgX = (REAL)m_ptMouse.x;

	if (m_ptMouse.y+m_szInfo.cy > rect.bottom)
		orgY = (REAL)(rect.bottom - m_szInfo.cy);
	else
		orgY = (REAL)m_ptMouse.y;

	stCoordinate ptInfo = m_pStGraphData[m_nCurrGrpData].arrGraphData.GetAt(m_nCurrGrpPoint);
	CString strInfo=L"";
	strInfo.Format(L"Graph : %d",m_nCurrGrpData);
	//strInfo.Format(L"Graph : %d\r\nX: %.2f\r\nY: %.2f",m_nCurrGrpData,ptInfo.x,ptInfo.y);

	SolidBrush brsBkg(m_clrInfoBkg);
	pG->FillRectangle(&brsBkg,orgX+5, orgY,(REAL)m_szInfo.cx,(REAL)m_szInfo.cy);

	SolidBrush brsText(m_pStGraphData[m_nCurrGrpData].clrGraphData);
	pG->DrawString(strInfo,strInfo.GetLength(),&font,RectF((REAL)orgX+10,(REAL)orgY+2,(REAL)m_szInfo.cx,(REAL)m_szInfo.cx/2),&formatAlign,&brsText);

	brsText.SetColor(Color(255,255,255,255));
	strInfo.Format(L"X: %.2f\r\nY: %.2f",ptInfo.x,ptInfo.y);
	pG->DrawString(strInfo,strInfo.GetLength(),&font,RectF((REAL)orgX+10,(REAL)orgY+m_szInfo.cx/3,(REAL)m_szInfo.cx,(REAL)m_szInfo.cx/2),&formatAlign,&brsText);
}

void CGraphEx::DrawScaleGridValue(Graphics* pG)
{
	FontFamily fontptroll(L"Arial");
	Gdiplus::Font font(&fontptroll, m_fSizeValue, FontStyleRegular, UnitPixel);
	SolidBrush brs(m_clrTextValue);
	StringFormat formatAlign;
	formatAlign.SetAlignment(StringAlignmentCenter);
	formatAlign.SetLineAlignment(StringAlignmentCenter);

	double dStepAxisX = (double) m_rcGrp.Width () / (m_nCntScaleAxisX+1); //0과 Max의 눈금 때문에 +1을 함.
	double dStepAxisY = (double) m_rcGrp.Height() / (m_nCntScaleAxisY+1); 
	double dValueAxisX= m_dMaxAxisX / m_nCntScaleAxisX;
	double dValueAxisY= m_dMaxAxisY / m_nCntScaleAxisY;

	double dPosY=0.0,dPosX=0.0;
	CString textValue=L"";
	CRect rect=CRect(0,0,0,0);

	// 세로 축 눈금을 그리기 위한 루프.
	for (int i=0; i<m_nCntScaleAxisY; i++)
	{
		dPosY = m_rcGrp.top + dStepAxisY * (i+1);
		if (m_bDrawScale)
		{
			Pen pen(m_clrScale,m_fSizeScale);
			pG->DrawLine(&pen,Point(m_rcGrp.left-1, int(dPosY)), Point(m_rcGrp.left-6, int(dPosY)));
		}
		if (m_bDrawGrid)
		{
			Pen pen(m_clrGrid,m_fSizeGrid);
			pG->DrawLine(&pen,Point(m_rcGrp.left+2, int(dPosY)), Point(m_rcGrp.right-2, int(dPosY)));
		}
		if (m_bShowValueY)
		{
			rect.SetRect(m_rcGrp.left - 45, int(dPosY - 8), m_rcGrp.left - 8, int(dPosY + 10));
			if (m_bShowInteger)
				textValue.Format(L"%d", (int)(m_dMaxAxisY - dValueAxisY*i));
			else
				textValue.Format(L"%.1f", m_dMaxAxisY - dValueAxisY*i);
			
			pG->DrawString(textValue,textValue.GetLength(),&font,RectF((REAL)rect.left,(REAL)rect.top,(REAL)rect.Width(),(REAL)rect.Height()),&formatAlign,&brs);
		}
	}
	if (m_bShowOrigin)
	{
		// 원점 표시를 위해 한번 더 출력.
		dPosY = m_rcGrp.top + dStepAxisY * (m_nCntScaleAxisY+1);
		rect.SetRect(m_rcGrp.left - 45 +15, int(dPosY - 8 +7), m_rcGrp.left - 8 +15, int(dPosY + 10 +7));
		if (m_bShowInteger)
			textValue.Format(L"0");
		else
			textValue.Format(L"0.0");
		
		pG->DrawString(textValue,textValue.GetLength(),&font,RectF((REAL)rect.left,(REAL)rect.top,(REAL)rect.Width(),(REAL)rect.Height()),&formatAlign,&brs);
	}

	// 가로 축 눈금을 그리기 위한 루프.
	for (int i=0; i<m_nCntScaleAxisX; i++)
	{
		dPosX = m_rcGrp.left + dStepAxisX * (i+1);

		if (m_bDrawScale)
		{
			Pen pen(m_clrScale,m_fSizeScale);
			pG->DrawLine(&pen,Point(int(dPosX), m_rcGrp.bottom+1), Point(int(dPosX), m_rcGrp.bottom+6));
		}
		if (m_bDrawGrid)
		{
			Pen pen(m_clrGrid,m_fSizeGrid);
			pG->DrawLine(&pen,Point(int(dPosX), m_rcGrp.top+2), Point(int(dPosX), m_rcGrp.bottom-2));
		}
		if (m_bShowValueX)
		{
			rect.SetRect(int(dPosX - 40), m_rcGrp.bottom + 8, int(dPosX + 40), m_rcGrp.bottom + 26);
			if (m_bShowInteger)
				textValue.Format(L"%d", (int)(dValueAxisX * (i+1)));
			else
				textValue.Format(L"%.1f", dValueAxisX * (i+1));

			pG->DrawString(textValue,textValue.GetLength(),&font,RectF((REAL)rect.left,(REAL)rect.top,(REAL)rect.Width(),(REAL)rect.Height()),&formatAlign,&brs);
		}
	}
	
}

void CGraphEx::DrawLabelText(Graphics* pG)
{
	FontFamily fontptroll(L"Arial");
	Gdiplus::Font font(&fontptroll, m_fSizeLabel, FontStyleRegular, UnitPixel);
	SolidBrush brs(m_clrTextLabel);
	StringFormat formatAlign;
	formatAlign.SetAlignment(StringAlignmentCenter);
	formatAlign.SetLineAlignment(StringAlignmentCenter);

	double dStepAxisX = (double) m_rcGrp.Width () / (m_nCntScaleAxisX+1); //0과 Max의 눈금 때문에 +1을 함.
	CRect rect(0,0,0,0);
	
	// 세로 축 라벨 표시.
	if (m_bShowLabel)
	{
		double dPosY=0.0;
		dPosY = m_rcGrp.top ;
		rect.SetRect(m_rcGrp.left - 70, int(dPosY - 8), m_rcGrp.left - 8, int(dPosY + 10));
		pG->DrawString(m_strAxisY,m_strAxisY.GetLength(),&font,RectF((REAL)rect.left,(REAL)rect.top,(REAL)rect.Width(),(REAL)rect.Height()),&formatAlign,&brs);
	}
	// 세로 축 라벨 표시.
	if (m_bShowLabel)
	{
		double dPosX=0.0;
		dPosX = m_rcGrp.left + dStepAxisX * (m_nCntScaleAxisX+1);
		rect.SetRect(int(dPosX - 40), m_rcGrp.bottom + 8, int(dPosX + 40), m_rcGrp.bottom + 26);
		pG->DrawString(m_strAxisX,m_strAxisX.GetLength(),&font,RectF((REAL)rect.left,(REAL)rect.top,(REAL)rect.Width(),(REAL)rect.Height()),&formatAlign,&brs);
	}
}

bool CGraphEx::LoadFrmImageFromFile(CString strPath)
{
	if (IsFileExist(strPath) == false) return false;

	if (m_pBmpFrmBkg!= NULL)
	{
		delete m_pBmpFrmBkg;
		m_pBmpFrmBkg = NULL;
	}

	m_pBmpFrmBkg = (Bitmap*)Image::FromFile(strPath);

	if (m_pBmpFrmBkg->GetLastStatus() != Gdiplus::Ok)
		return false;

	Invalidate();

	return true;
}

bool CGraphEx::LoadFrmImageFromResource(UINT ID, bool bPNG)
{
	if (m_pBmpFrmBkg != NULL)
	{
		delete m_pBmpFrmBkg;
		m_pBmpFrmBkg = NULL;
	}

	if (bPNG == false)
	{
		m_pBmpFrmBkg = Bitmap::FromResource(AfxGetInstanceHandle(),(WCHAR*)MAKEINTRESOURCE(ID));
		if (m_pBmpFrmBkg->GetLastStatus() != Gdiplus::Ok)
			return false;
	}
	else
	{
		// PNG 리소스는 Bitmap 리소스를 불러오는 것보다 복잡하다.
		HRSRC hResource = ::FindResource(AfxGetApp()->m_hInstance, (WCHAR*)MAKEINTRESOURCEW(ID), L"PNG");	// 3번째 인자는 실제 리소스가 분류되는 문자열이 전달되어야 한다.
		if (!hResource) return false;

		DWORD imageSize = ::SizeofResource(AfxGetApp()->m_hInstance, hResource);
		if (!imageSize) return false;

		const void* pResourceData = ::LockResource(::LoadResource(AfxGetApp()->m_hInstance, hResource));
		if (!pResourceData) return false;

		HGLOBAL hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
		if (hBuffer)
		{
			void* pBuffer = ::GlobalLock(hBuffer);
			if (pBuffer)
			{
				CopyMemory(pBuffer, pResourceData, imageSize);

				IStream* pStream = NULL;
				if (::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream) == S_OK)
				{
					m_pBmpFrmBkg = Gdiplus::Bitmap::FromStream(pStream);
					pStream->Release();

					if (m_pBmpFrmBkg)
					{
						if (m_pBmpFrmBkg->GetLastStatus() != Gdiplus::Ok)
						{
							delete m_pBmpFrmBkg;
							m_pBmpFrmBkg = NULL;
						}
					}
				}
			}
			::GlobalUnlock(hBuffer);
			::GlobalFree(hBuffer);
		}
	}

	Invalidate();

	return true;
}

bool CGraphEx::LoadGrpImageFromFile(CString strPath)
{
	if (IsFileExist(strPath) == false) return false;

	if (m_pBmpGrpBkg!= NULL)
	{
		delete m_pBmpGrpBkg;
		m_pBmpGrpBkg = NULL;
	}

	m_pBmpGrpBkg = (Bitmap*)Image::FromFile(strPath);

	if (m_pBmpGrpBkg->GetLastStatus() != Gdiplus::Ok)
		return false;

	Invalidate();

	return true;
}

bool CGraphEx::LoadGrpImageFromResource(UINT ID, bool bPNG)
{
	if (m_pBmpGrpBkg != NULL)
	{
		delete m_pBmpGrpBkg;
		m_pBmpGrpBkg = NULL;
	}

	if (bPNG == false)
	{
		m_pBmpGrpBkg = Bitmap::FromResource(AfxGetInstanceHandle(),(WCHAR*)MAKEINTRESOURCE(ID));
		if (m_pBmpGrpBkg->GetLastStatus() != Gdiplus::Ok)
			return false;
	}
	else
	{
		// PNG 리소스는 Bitmap 리소스를 불러오는 것보다 복잡하다.
		HRSRC hResource = ::FindResource(AfxGetApp()->m_hInstance, (WCHAR*)MAKEINTRESOURCEW(ID), L"PNG");	// 3번째 인자는 실제 리소스가 분류되는 문자열이 전달되어야 한다.
		if (!hResource) return false;

		DWORD imageSize = ::SizeofResource(AfxGetApp()->m_hInstance, hResource);
		if (!imageSize) return false;

		const void* pResourceData = ::LockResource(::LoadResource(AfxGetApp()->m_hInstance, hResource));
		if (!pResourceData) return false;

		HGLOBAL hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
		if (hBuffer)
		{
			void* pBuffer = ::GlobalLock(hBuffer);
			if (pBuffer)
			{
				CopyMemory(pBuffer, pResourceData, imageSize);

				IStream* pStream = NULL;
				if (::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream) == S_OK)
				{
					m_pBmpGrpBkg = Gdiplus::Bitmap::FromStream(pStream);
					pStream->Release();

					if (m_pBmpGrpBkg)
					{
						if (m_pBmpGrpBkg->GetLastStatus() != Gdiplus::Ok)
						{
							delete m_pBmpGrpBkg;
							m_pBmpGrpBkg = NULL;
						}
					}
				}
			}
			::GlobalUnlock(hBuffer);
			::GlobalFree(hBuffer);
		}
	}

	Invalidate();

	return true;
}

bool CGraphEx::LoadImageFromFile(CString strFrm, CString strGrp)
{
	if (IsFileExist(strFrm) == false) return false;
	
	if (m_pBmpFrmBkg != NULL)
	{
		delete m_pBmpFrmBkg;
		m_pBmpFrmBkg = NULL;
	}

	m_pBmpFrmBkg = (Bitmap*)Image::FromFile(strFrm);

	if (m_pBmpFrmBkg->GetLastStatus() != Gdiplus::Ok)
		return false;

	if (IsFileExist(strGrp) == false) return false;
	
	if (m_pBmpGrpBkg != NULL)
	{
		delete m_pBmpGrpBkg;
		m_pBmpGrpBkg = NULL;
	}

	m_pBmpGrpBkg = (Bitmap*)Image::FromFile(strGrp);

	if (m_pBmpGrpBkg->GetLastStatus() != Gdiplus::Ok)
		return false;

	Invalidate();

	return true;
}

bool CGraphEx::LoadImageFromResource(UINT nIdFrm, UINT nIdGrp, bool bPNG)
{
	if (m_pBmpFrmBkg != NULL)
	{
		delete m_pBmpFrmBkg;
		m_pBmpFrmBkg = NULL;
	}

	if (m_pBmpGrpBkg != NULL)
	{
		delete m_pBmpGrpBkg;
		m_pBmpGrpBkg = NULL;
	}

	if (bPNG == false)
	{
		m_pBmpFrmBkg = Bitmap::FromResource(AfxGetInstanceHandle(),(WCHAR*)MAKEINTRESOURCE(nIdFrm));
		if (m_pBmpFrmBkg->GetLastStatus() != Gdiplus::Ok)
			return false;

		m_pBmpGrpBkg = Bitmap::FromResource(AfxGetInstanceHandle(),(WCHAR*)MAKEINTRESOURCE(nIdGrp));
		if (m_pBmpGrpBkg->GetLastStatus() != Gdiplus::Ok)
			return false;
	}
	else
	{
		for (int i=0; i<2; i++)
		{
			UINT ID=0;
			i==0 ? ID=nIdFrm : ID=nIdGrp;

			// PNG 리소스는 Bitmap 리소스를 불러오는 것보다 복잡하다.
			HRSRC hResource = ::FindResource(AfxGetApp()->m_hInstance, (WCHAR*)MAKEINTRESOURCEW(ID), L"PNG");	// 3번째 인자는 실제 리소스가 분류되는 문자열이 전달되어야 한다.
			if (!hResource) return false;

			DWORD imageSize = ::SizeofResource(AfxGetApp()->m_hInstance, hResource);
			if (!imageSize) return false;

			const void* pResourceData = ::LockResource(::LoadResource(AfxGetApp()->m_hInstance, hResource));
			if (!pResourceData) return false;

			HGLOBAL hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
			if (hBuffer)
			{
				void* pBuffer = ::GlobalLock(hBuffer);
				if (pBuffer)
				{
					CopyMemory(pBuffer, pResourceData, imageSize);

					IStream* pStream = NULL;
					if (::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream) == S_OK)
					{
						if (i==0)
						{
							m_pBmpFrmBkg = Gdiplus::Bitmap::FromStream(pStream);
							pStream->Release();

							if (m_pBmpFrmBkg)
							{
								if (m_pBmpFrmBkg->GetLastStatus() != Gdiplus::Ok)
								{
									delete m_pBmpFrmBkg;
									m_pBmpFrmBkg = NULL;
								}
							}
						}
						else
						{
							m_pBmpGrpBkg = Gdiplus::Bitmap::FromStream(pStream);
							pStream->Release();

							if (m_pBmpGrpBkg)
							{
								if (m_pBmpGrpBkg->GetLastStatus() != Gdiplus::Ok)
								{
									delete m_pBmpGrpBkg;
									m_pBmpGrpBkg = NULL;
								}
							}
						}
					}
				}
				::GlobalUnlock(hBuffer);
				::GlobalFree(hBuffer);
			}
		}
	}

	Invalidate();

	return true;
}

bool CGraphEx::IsFileExist(CString strPath)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	if ((hFind = ::FindFirstFile(strPath, &fd)) == INVALID_HANDLE_VALUE) 
	{
		FindClose(hFind);
		return false;
	}

	return true;
}

// Picture Control을 상속받아 그래프를 생성하면 마우스 이벤트를 받지 못한다. 그래서 Custom Control을 사용.
void CGraphEx::OnMouseMove(UINT nFlags, CPoint point)
{
	bool bBreak=false;
	for (int j=0; j<m_nCntGraph; j++)
	{
		for (int i=0; i<m_pArrPosGrp[j].GetSize(); i++)
		{
			if (bBreak) break;
			int size = m_pStGraphData[j].nSizeGraphPoint;
			if (point.x > m_pArrPosGrp[j].GetAt(i).X-size && point.x < m_pArrPosGrp[j].GetAt(i).X+size &&
				point.y > m_pArrPosGrp[j].GetAt(i).Y-size && point.y < m_pArrPosGrp[j].GetAt(i).Y+size)
			{
				m_bDrawInfo = true;
				m_ptMouse = point;
				m_nCurrGrpPoint = i;
				m_nCurrGrpData = j;
				bBreak = true;
				break;
			}
			else
				m_bDrawInfo = false;
		}
	}
		
	Invalidate(false);

	CWnd::OnMouseMove(nFlags, point);
}


void CGraphEx::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	GetClientRect(m_rcFrm);	

	m_rcGrp.SetRect(m_rcFrm.left + 75, m_rcFrm.top + 10, m_rcFrm.right - 35, m_rcFrm.bottom - 30);
}
