#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// RotateSpeed �Ի���

class RotateSpeed : public CDialogEx
{
	DECLARE_DYNAMIC(RotateSpeed)

public:
	RotateSpeed(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~RotateSpeed();
// �Ի�������
	enum { IDD = IDD_SetSpeed };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOk();
	
	CString Speed;
	CString Acc;
};
