// TellYou.cpp : implementation file
//

#include "stdafx.h"
#include "RussiaBlock.h"
#include "TellYou.h"


// CTellYou dialog

IMPLEMENT_DYNAMIC(CTellYou, CDialog)

CTellYou::CTellYou(CWnd* pParent /*=NULL*/)
	: CDialog(CTellYou::IDD, pParent)
{

}

CTellYou::~CTellYou()
{
}

void CTellYou::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTellYou, CDialog)
END_MESSAGE_MAP()


// CTellYou message handlers
