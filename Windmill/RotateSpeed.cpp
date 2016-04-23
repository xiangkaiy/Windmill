// RotateSpeed.cpp : 实现文件
//

#include "stdafx.h"
#include "Windmill.h"
#include "RotateSpeed.h"
#include "afxdialogex.h"

extern DataEx Data;
// RotateSpeed 对话框

IMPLEMENT_DYNAMIC(RotateSpeed, CDialogEx)

RotateSpeed::RotateSpeed(CWnd* pParent /*=NULL*/)
	: CDialogEx(RotateSpeed::IDD, pParent)
	, Speed(_T(""))
	, Acc(_T(""))
{
}

RotateSpeed::~RotateSpeed()
{
}

void RotateSpeed::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, Speed);
	DDX_Text(pDX, IDC_EDIT1, Acc);
}


BEGIN_MESSAGE_MAP(RotateSpeed, CDialogEx)
	ON_BN_CLICKED(IDOK, &RotateSpeed::OnBnClickedOk)
END_MESSAGE_MAP()


// RotateSpeed 消息处理程序


void RotateSpeed::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	Data.S = Speed;
	Data.Acce = Acc;
	CDialogEx::OnOK();
}
