// RussiaBlockDlg.h : ͷ�ļ�
//

#pragma once

class CPassword;
// CRussiaBlockDlg �Ի���
class CRussiaBlockDlg : public CDialog
{
// ����
public:
	CRussiaBlockDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RUSSIABLOCK_DIALOG };

	enum { GAME_TIME, DISPLAY_TIME, BKP_TIME	};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CFrame m_frm;		//���
	CRect m_rectClient;	//�ͻ���rect
	CRect m_rectGame;	//��Ϸ��rect
	CRect m_rectNext;	//next��rect
	CBitmap m_bitmapAry[7];	//�����������ɫλͼ
	
	CBlock* m_pCurBlock;	//��ǰ��������ķ���ָ��
	CBlock* m_pNextBlock;	//��һ������ָ��
	vector< vector<CRect> > m_rectBlock;	//ÿ����������һ��rect��
	CImage m_imgBK;		//����ͼƬ
	int m_nBkpIndex;	//����ͼƬ������
	vector<CString>	m_bkpFileName;	//���б���ͼƬ���ļ���
	HWND   m_Audio;		//�������־��
	vector<CString> m_MusicFileName;	//���б������ֵ��ļ���

	//TEST
	
	//

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// //��ʼ�����
	void InitFrame(void);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnDestroy();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnBnClickedBtnStart();
public:
	afx_msg void OnBnClickedBtnOption();
public:
	afx_msg void OnBnClickedBtnPause();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	void GameOver(void);
public:
	int m_nTime;
public:
	const CRect GetCurrentBlockRect(void);

public:
	void DrawBlock(const CRect& rt, CDC* pDC, int num = 0);
public:
	int m_nColor;	//�������ɫ
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	bool GetBkpFile(void);
public:
	bool GetMusicFile(void);
	afx_msg LRESULT OnMCINotify(WPARAM wParam,LPARAM lParam);	//��������״̬����Ϣ��Ӧ����
public:
	bool IsGameWin(void);
public:
	CPassword* m_pDlg;
};
