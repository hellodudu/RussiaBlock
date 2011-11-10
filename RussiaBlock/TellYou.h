#pragma once


// CTellYou dialog

class CTellYou : public CDialog
{
	DECLARE_DYNAMIC(CTellYou)

public:
	CTellYou(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTellYou();

// Dialog Data
	enum { IDD = IDD_TELLYOU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
