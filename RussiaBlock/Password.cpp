// Password.cpp : implementation file
//

#include "stdafx.h"
#include "RussiaBlock.h"
#include "Password.h"
#include "TellYou.h"


// CPassword dialog

IMPLEMENT_DYNAMIC(CPassword, CDialog)

CPassword::CPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CPassword::IDD, pParent)
	, m_strPassword(_T("i love you dudu"))
{

}

CPassword::~CPassword()
{
}

void CPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPassword, CDialog)
	ON_BN_CLICKED(IDOK, &CPassword::OnBnClickedOk)
END_MESSAGE_MAP()


// CPassword message handlers

void CPassword::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
		CString strPassword;	//Í¨¹ØÃÜÂë
		GetDlgItemTextW(IDC_EDIT1, strPassword);
		if (strPassword == m_strPassword)
		{
			CString strTellYou;
			strTellYou.LoadStringW(IDS_TELLYOU);
			CTellYou* ptydlg = new CTellYou;
			ptydlg->Create(IDD_TELLYOU);
			ptydlg->ShowWindow(SW_SHOW);
			ptydlg->SetDlgItemTextW(IDC_EDIT1, strTellYou);
			
		}
		

	OnOK();
}
