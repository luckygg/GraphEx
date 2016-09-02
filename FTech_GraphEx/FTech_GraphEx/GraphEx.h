#pragma once

//----------------------------------------------------------
// CGraphEx Control
//----------------------------------------------------------
// Programmed by William Kim
//----------------------------------------------------------
// Last Update : 2016-09-02
//----------------------------------------------------------

#define WINCLASSNAME_GRAPHEX L"GRAPHEX_CLASS"

struct stCoordinate
{
	double x;
	double y;
};

struct stGraphData
{
	Color clrGraphData;		// Color Graph Line
	Color clrGraphPoint;	// Color Graph Point
	float fSizeGraph;		// Size Graph Line
	int nSizeGraphPoint;	// Size Graph Point
	bool bDrawGraph;		// Check Draw Graph
	bool bDrawGraphPoint;	// Check Draw Graph Point
	CArray<stCoordinate> arrGraphData;	// Graph Data Position Array
};

class CGraphEx : public CWnd
{
public:
	CGraphEx();
	virtual ~CGraphEx();

	//----- Set Size -----//
	void SetSizeGraph		(CRect rcRect)	{ m_rcGrp = rcRect; Invalidate(FALSE); }
	void SetSizeGraph		(int nOrgX, int nOrgY, int nWidth, int nHeight) { m_rcGrp = CRect(nOrgX, nOrgY, nOrgX+nWidth, nOrgY+nHeight); Invalidate(FALSE); }
	void SetSizeGraphData	(int nIndex, float fSize)	{ if (nIndex <0 || fSize <0) return; m_pStGraphData[nIndex].fSizeGraph		= fSize; Invalidate(); }
	void SetSizeGraphPoint	(int nIndex, int nSize)		{ if (nIndex <0 || nSize <0) return; m_pStGraphData[nIndex].nSizeGraphPoint	= nSize; Invalidate(); }
	void SetSizeFrmBorder	(float fSize) { m_fSizeFrmBorder= fSize; Invalidate(); }
	void SetSizeGrpBorder	(float fSize) { m_fSizeGrpBorder= fSize; Invalidate(); }
	void SetSizeScale		(float fSize) { m_fSizeScale	= fSize; Invalidate(); }
	void SetSizeGrid		(float fSize) { m_fSizeGrid		= fSize; Invalidate(); }
	void SetSizeTextValue	(float fSize) { m_fSizeValue	= fSize; Invalidate(); }
	void SetSizeTextLabel	(float fSize) { m_fSizeLabel	= fSize; Invalidate(); }
	void SetSizeTextInfo	(float fSize) { m_fSizeInfo		= fSize; Invalidate(); }
	
	//----- Set Color -----//
	void SetColorFrmBkg		(int nA, int nR, int nG, int nB) { m_clrFrmBkg		= Color(nA, nR, nG, nB); Invalidate(); }
	void SetColorFrmBorder	(int nA, int nR, int nG, int nB) { m_clrFrmBorder	= Color(nA, nR, nG, nB); Invalidate(); }
	void SetColorGrpBkg		(int nA, int nR, int nG, int nB) { m_clrGrpBkg		= Color(nA, nR, nG, nB); Invalidate(); }
	void SetColorGrpBorder	(int nA, int nR, int nG, int nB) { m_clrGrpBorder	= Color(nA, nR, nG, nB); Invalidate(); }
	void SetColorScale		(int nA, int nR, int nG, int nB) { m_clrScale		= Color(nA, nR, nG, nB); Invalidate(); }
	void SetColorGrid		(int nA, int nR, int nG, int nB) { m_clrGrid		= Color(nA, nR, nG, nB); Invalidate(); }
	void SetColorTextValue	(int nA, int nR, int nG, int nB) { m_clrTextValue	= Color(nA, nR, nG, nB); Invalidate(); }
	void SetColorTextLabel	(int nA, int nR, int nG, int nB) { m_clrTextLabel	= Color(nA, nR, nG, nB); Invalidate(); }
	void SetColorGraphData	(int nIndex, int nA, int nR, int nG, int nB) { if (nIndex <0) return; m_pStGraphData[nIndex].clrGraphData  = Color(nA, nR, nG, nB); Invalidate(); }
	void SetColorGraphPoint	(int nIndex, int nA, int nR, int nG, int nB) { if (nIndex <0) return; m_pStGraphData[nIndex].clrGraphPoint = Color(nA, nR, nG, nB); Invalidate(); }
	void SetColorDataInfo	(int nA, int nR, int nG, int nB) { m_clrInfoBkg		= Color(nA, nR, nG, nB); Invalidate(); }
	
	//----- Set Text -----//
	void SetTextLabelAxisX(CString strlabel) { m_strAxisX = strlabel; Invalidate(); }
	void SetTextLabelAxisY(CString strlabel) { m_strAxisY = strlabel; Invalidate(); }
	
	//----- Set & Get Enable Option -----//
	void SetEnableDrawGrid  (bool bEnable) { m_bDrawGrid   = bEnable; Invalidate(); }
	void SetEnableDrawScale (bool bEnable) { m_bDrawScale  = bEnable; Invalidate(); }
	void SetEnableShowValue (bool bEnable) { m_bShowValue  = bEnable; Invalidate(); }
	void SetEnableShowLabel (bool bEnable) { m_bShowLabel  = bEnable; Invalidate(); }
	void SetEnableShowOrigin(bool bEnable) { m_bShowOrigin = bEnable; Invalidate(); }
	void SetEnableDrawGraph (int nIndex, bool bEnable)  { if (nIndex <0) return; m_pStGraphData[nIndex].bDrawGraph = bEnable; Invalidate(); }
	void SetEnableDrawGraphPoint (int nIndex, bool bEnable)  { if (nIndex <0) return; m_pStGraphData[nIndex].bDrawGraphPoint = bEnable; Invalidate(); }
	void SetEnableValueInteger(bool bEnable) { m_bShowInteger = bEnable; Invalidate(); }
	bool GetEnableDrawGraph (int nIndex)  { if (nIndex <0) return false; return m_pStGraphData[nIndex].bDrawGraph; }
	bool GetEnableDrawGraphPoint (int nIndex)  { if (nIndex <0) return false; return m_pStGraphData[nIndex].bDrawGraphPoint; }
	
	//----- Set Graph Data -----//
	void SetCountGraph(int nMax);
	void SetMaxValueAxis	(double dValueX, double dValueY) { if (dValueX < 0 || dValueY < 0) return; m_dMaxAxisX = dValueX; m_dMaxAxisY = dValueY; Invalidate();}
	void SetMaxValueAxisX	(double dValue) { if (dValue <0) return; m_dMaxAxisX = dValue; Invalidate();}
	void SetMaxValueAxisY	(double dValue) { if (dValue <0) return; m_dMaxAxisY = dValue; Invalidate();}
	void SetCountScaleAxisX	(int nScale) { if (nScale < 0) return; m_nCntScaleAxisX = nScale; Invalidate(); }
	void SetCountScaleAxisY	(int nScale) { if (nScale < 0) return; m_nCntScaleAxisY = nScale; Invalidate(); }
	void SetCountScale		(int nScaleX, int nScaleY) { if (nScaleX < 0 || nScaleY < 0) return; m_nCntScaleAxisX = nScaleX; m_nCntScaleAxisY = nScaleY; Invalidate(); }
	
	//----- Add & Clear Graph Data -----//
	void SetGraphData(int nIndex, stCoordinate* pData, int nArrSize);
	void ClearGraphData(int nIndex);
	void ClearAllGraphData();

	//----- Set Image -----//
	bool LoadFrmImageFromFile(CString strPath);
	bool LoadFrmImageFromResource(UINT ID, bool bPNG);
	bool LoadGrpImageFromFile(CString strPath);
	bool LoadGrpImageFromResource(UINT ID, bool bPNG);
	bool LoadImageFromFile(CString strFrm, CString strGrp);
	bool LoadImageFromResource(UINT nIdFrm, UINT nIdGrp, bool bPNG);

private:
	stGraphData *m_pStGraphData;	// �׷��� ������ ����ü.
	Bitmap* m_pBmpFrmBkg;			// ������ ���� �̹���.
	Bitmap* m_pBmpGrpBkg;			// �׷��� ���� �̹���.
	Color m_clrFrmBkg;				// ������ ���� ������.
	Color m_clrFrmBorder;			// ������ ���� ������ ����.
	Color m_clrGrpBkg;				// �׷��� ���� ������.
	Color m_clrGrpBorder;			// �׷��� ���� ������ ����.
	Color m_clrScale;				// ���м��� ����.
	Color m_clrGrid;				// ���ڼ��� ����.
	Color m_clrTextValue;			// Value Text�� ���ڻ���.
	Color m_clrTextLabel;			// Label Text�� ���ڻ���.
	Color m_clrInfoBkg;				// ���콺�� ����Ű�� ����Ʈ�� ������ �����ִ� ������ ����.
	CString m_strAxisX;				// X�� Label�� ���ڿ�.
	CString m_strAxisY;				// Y�� Label�� ���ڿ�.
	CRect m_rcGrp;					// �׷��� ����.
	CRect m_rcFrm;					// ������ ����.
	CSize m_szInfo;					// ���콺�� ����Ű�� ����Ʈ�� ������ �����ִ� ����.
	double m_dMaxAxisX;				// X�� �ִ� ��.
	double m_dMaxAxisY;				// Y�� �ִ� ��.
	float m_fSizeFrmBorder;			// ������ ���� ���� ���� ����.
	float m_fSizeGrpBorder;			// �׷��� ���� ���� ���� ����.
	float m_fSizeScale;				// ���м��� ����.
	float m_fSizeGrid;				// ���ڼ��� ����.
	float m_fSizeValue;				// Value Text�� ����ũ��.
	float m_fSizeLabel;				// Label Text�� ����ũ��.
	float m_fSizeInfo;				// ���콺�� ����Ű�� ����Ʈ�� ������ �����ִ� ����ũ��.
	int m_nCntGraph;				// �׷����� ����.
	int m_nCntScaleAxisX;			// X������ �� ��� �� ������.
	int m_nCntScaleAxisY;			// Y������ �� ��� �� ������.
	bool m_bDrawGrid;				// ���ڼ��� �׸� ������.
	bool m_bDrawScale;				// ���м��� �׸� ������.
	bool m_bShowLabel;				// Label text�� �׸� ������.
	bool m_bShowValue;				// Value text�� �׸� ������.
	bool m_bShowOrigin;				// ���� ǥ��.
	bool m_bShowInteger;			// ���� / �Ҽ������� �����͸� ������.

	CArray<PointF> *m_pArrPosGrp;	// �׷����� �׷����� ��ǥ ����.
	bool	m_bDrawInfo;			// ���콺 ������ �׸� ������ Ȯ�� ��.
	CPoint	m_ptMouse;				// ���� ���콺�� ��ġ.
	int		m_nCurrGrpPoint;		// ���� ���콺�� ����Ű�� �ִ� Point�� ����.
	int		m_nCurrGrpData;			// ���� ���콺�� ����Ű�� �ִ� Graph�� ����.
	
	void RegisterWindowClass(void);
	void DrawFrameBkg(Graphics* pG);
	void DrawFrameBorder(Graphics* pG);
	void DrawGraphBkg(Graphics* pG);
	void DrawGraphBorder(Graphics* pG);
	void DrawGraphData(Graphics* pG);
	void DrawScaleGridValue(Graphics* pG);
	void DrawLabelText(Graphics* pG);
	void DrawGraphDataInfo(Graphics* pG);
	bool IsFileExist(CString strPath);

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};