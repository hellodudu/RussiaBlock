// RussiaBlockDlg.h : 头文件
//

#pragma once

class CPassword;
// CRussiaBlockDlg 对话框
class CRussiaBlockDlg : public CDialog
{
// 构造
public:
	CRussiaBlockDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RUSSIABLOCK_DIALOG };

	enum { GAME_TIME, DISPLAY_TIME, BKP_TIME	};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CFrame m_frm;		//框架
	CRect m_rectClient;	//客户区rect
	CRect m_rectGame;	//游戏区rect
	CRect m_rectNext;	//next区rect
	CBitmap m_bitmapAry[7];	//方块的七种颜色位图
	
	CBlock* m_pCurBlock;	//当前正在下落的方块指针
	CBlock* m_pNextBlock;	//下一个方块指针
	vector< vector<CRect> > m_rectBlock;	//每个方块存放在一个rect中
	CImage m_imgBK;		//背景图片
	int m_nBkpIndex;	//背景图片的索引
	vector<CString>	m_bkpFileName;	//所有背景图片的文件名
	HWND   m_Audio;		//背景音乐句柄
	vector<CString> m_MusicFileName;	//所有背景音乐的文件名

	//TEST
	
	//

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// //初始化框架
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
	int m_nColor;	//方块的颜色
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	bool GetBkpFile(void);
public:
	bool GetMusicFile(void);
	afx_msg LRESULT OnMCINotify(WPARAM wParam,LPARAM lParam);	//播放音乐状态的消息响应函数
public:
	bool IsGameWin(void);
public:
	CPassword* m_pDlg;
};
