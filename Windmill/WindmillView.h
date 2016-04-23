
// WindmillView.h : CWindmillView ��Ľӿ�
//
#pragma once


class CWindmillView : public CView
{
protected: // �������л�����
	CWindmillView();
	DECLARE_DYNCREATE(CWindmillView)

// ����
public:
	CWindmillDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

	CDC*		m_pDC;
	HGLRC m_hGLContext;
	COLORREF Color1;
	COLORREF Color2;
	COLORREF Color3;
	COLORREF Color4;
	float Sp;
	float Ac;
	int TempFPSTime;
	float T;
	bool IsAccStop;
	BOOL MotionFlur;
	float InitAngle;
	float Step;
	GLuint	texture[3];
	bool UseTexture;
	int VaneKind;
	float Alpha;
// ʵ��
public:
	virtual ~CWindmillView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:

	bool SetWindowPixelFormat();
	BOOL OpenGLInit();//��ʼ��
	void DrawWindmill();//��һ���糵
	void DrawFlabellum(COLORREF Color);//��һ��Ҷ
	void Background();
	void DrawBase();
	AUX_RGBImageRec* LoadBMP(char *Filename);//����λͼͼ��
	int LoadGLTextures();
	int UseLight();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetColor();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRotate();
	afx_msg void OnStop();
	afx_msg void OnRotateSpeed();
	afx_msg void OnEnableMotionFlur();
	afx_msg void OnDisableMotionFlur();
	afx_msg void OnTexture();
	
	afx_msg void OnChangeVane();
};

#ifndef _DEBUG  // WindmillView.cpp �еĵ��԰汾
inline CWindmillDoc* CWindmillView::GetDocument() const
   { return reinterpret_cast<CWindmillDoc*>(m_pDocument); }
#endif

