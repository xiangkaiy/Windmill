// SetColorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Windmill.h"
#include "SetColorDlg.h"
#include "afxdialogex.h"

extern DataEx Data;
// SetColorDlg 对话框

IMPLEMENT_DYNAMIC(SetColorDlg, CDialogEx)

SetColorDlg::SetColorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetColorDlg::IDD, pParent)
{
	this->ColorBtn1.SetColor(RGB(0,0,0));
	this->ColorBtn2.SetColor(RGB(255,0,0));
	this->ColorBtn3.SetColor(RGB(0,255,0));
	this->ColorBtn4.SetColor(RGB(0,0,255));
}

SetColorDlg::~SetColorDlg()
{
}

void SetColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, ColorBtn1);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON2, ColorBtn2);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON3, ColorBtn3);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON4, ColorBtn4);
}


BEGIN_MESSAGE_MAP(SetColorDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &SetColorDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &SetColorDlg::OnCancel)
END_MESSAGE_MAP()


void SetColorDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	Data.C1 = ColorBtn1.GetColor();
	Data.C2 = ColorBtn2.GetColor();
	Data.C3 = ColorBtn3.GetColor();
	Data.C4 = ColorBtn4.GetColor();
	CDialog::OnOK();
}


void SetColorDlg::OnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}
