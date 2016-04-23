#pragma once
#include "afxcolorbutton.h"

// SetColorDlg 对话框

class SetColorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetColorDlg)

public:
	SetColorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetColorDlg();

// 对话框数据
	enum { IDD = IDD_SetColor };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMFCColorButton ColorBtn1;
	CMFCColorButton ColorBtn2;
	CMFCColorButton ColorBtn3;
	CMFCColorButton ColorBtn4;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCancel();
};
