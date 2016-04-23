#pragma once
#include "afxcolorbutton.h"

// SetColorDlg �Ի���

class SetColorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetColorDlg)

public:
	SetColorDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetColorDlg();

// �Ի�������
	enum { IDD = IDD_SetColor };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CMFCColorButton ColorBtn1;
	CMFCColorButton ColorBtn2;
	CMFCColorButton ColorBtn3;
	CMFCColorButton ColorBtn4;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCancel();
};
