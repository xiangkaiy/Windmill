#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// RotateSpeed 对话框

class RotateSpeed : public CDialogEx
{
	DECLARE_DYNAMIC(RotateSpeed)

public:
	RotateSpeed(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RotateSpeed();
// 对话框数据
	enum { IDD = IDD_SetSpeed };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOk();
	
	CString Speed;
	CString Acc;
};
