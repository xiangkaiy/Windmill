
// WindmillView.h : CWindmillView 类的接口
//
#pragma once


class CWindmillView : public CView
{
protected: // 仅从序列化创建
	CWindmillView();
	DECLARE_DYNCREATE(CWindmillView)

// 特性
public:
	CWindmillDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
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
// 实现
public:
	virtual ~CWindmillView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:

	bool SetWindowPixelFormat();
	BOOL OpenGLInit();//初始化
	void DrawWindmill();//画一个风车
	void DrawFlabellum(COLORREF Color);//画一个叶
	void Background();
	void DrawBase();
	AUX_RGBImageRec* LoadBMP(char *Filename);//载入位图图像
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

#ifndef _DEBUG  // WindmillView.cpp 中的调试版本
inline CWindmillDoc* CWindmillView::GetDocument() const
   { return reinterpret_cast<CWindmillDoc*>(m_pDocument); }
#endif

