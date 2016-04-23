// RotateSpeed.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Windmill.h"
#include "RotateSpeed.h"
#include "afxdialogex.h"

extern DataEx Data;
// RotateSpeed �Ի���

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


// RotateSpeed ��Ϣ�������


void RotateSpeed::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	Data.S = Speed;
	Data.Acce = Acc;
	CDialogEx::OnOK();
}
