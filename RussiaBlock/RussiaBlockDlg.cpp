// RussiaBlockDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RussiaBlock.h"
#include "RussiaBlockDlg.h"
#include "Password.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRussiaBlockDlg 对话框




CRussiaBlockDlg::CRussiaBlockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRussiaBlockDlg::IDD, pParent)
	, m_nTime(0)
	, m_nColor(0)
	, m_Audio(NULL)
	, m_nBkpIndex(0)
	, m_pDlg(new (CPassword))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRussiaBlockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRussiaBlockDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_START, &CRussiaBlockDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_OPTION, &CRussiaBlockDlg::OnBnClickedBtnOption)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CRussiaBlockDlg::OnBnClickedBtnPause)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(MCIWNDM_NOTIFYMODE, &CRussiaBlockDlg::OnMCINotify)
END_MESSAGE_MAP()


// CRussiaBlockDlg 消息处理程序

BOOL CRussiaBlockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	((CRussiaBlockApp*)AfxGetApp())->m_hwndDlg = m_hWnd;	//键盘响应
	
	InitFrame();
	m_pCurBlock = NULL;
	m_pNextBlock = NULL;
	//整个Client区初始化
	GetClientRect(&m_rectClient);

	//载入方块位图
	for (int i=0; i!=7; ++i)
	{
		m_bitmapAry[i].LoadBitmapW(IDB_BMP_BLOCK1+i);
	}

	//如果背景文件夹中有jpg文件的话就将其载入进m_imgBK
	if (GetBkpFile())
	{
		m_imgBK.Load(m_bkpFileName[0]);
	}

	//如果背景音乐文件夹中有mp3文件的话就将其载入进m_MusicFileName
	if (GetMusicFile())
	{
		CString strPath;
		strPath = _T("bkm\\") + m_MusicFileName[0];
		m_Audio = ::MCIWndCreate(this->m_hWnd, AfxGetInstanceHandle(), WS_CHILD | MCIWNDF_NOTIFYMODE, 
			strPath); 
		MCIWndPlay(m_Audio);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRussiaBlockDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CPassword* dlgAbout = new CPassword;
		dlgAbout->Create(IDD_PASSWORD, this);
		dlgAbout->ShowWindow(SW_SHOW);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRussiaBlockDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);

		

		//绘制最外层框架
		CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
		dc.Rectangle(m_rectGame);
		dc.Rectangle(m_rectGame.left-1, m_rectGame.top-1, m_rectGame.right+1, m_rectGame.bottom+1);

		
		//画框架
		for (int x=0; x!=m_rectBlock.size(); ++x)
		{
			for (int y=0; y!=m_rectBlock[x].size(); ++y)
			{
				
				//dc.Rectangle(m_rectBlock[x][y]);

				if (m_frm.container[x][y])
				{	//dc.FillRect(m_rectBlock[x][y], &bru);	//填充已堆积的底部
					DrawBlock(m_rectBlock[x][y], &dc);

				}
			}
		}

		//画m_pNextBlock
		if (m_pNextBlock != NULL)
		{
			for (int i=0; i!=m_pNextBlock->GetBlock().size(); ++i)
			{
				int x = m_pNextBlock->GetBlock()[i].x;
				int y = m_pNextBlock->GetBlock()[i].y;
				
				CRect rt;
				rt.left = x*20 + 350;
				rt.top = 720 - y*20;
				rt.right = rt.left + 20;
				rt.bottom = rt.top + 20;
				
				DrawBlock(rt, &dc);
				
				//dc.FillRect(rt, &bru);
			}
		}
		

		//画下落的方块
		if (m_pCurBlock != NULL)
		{
			for (int i=0; i!=m_pCurBlock->GetBlock().size(); ++i)
			{
				int x = m_pCurBlock->GetBlock()[i].x;
				int y = m_pCurBlock->GetBlock()[i].y;
				DrawBlock(m_rectBlock[x][y], &dc, m_nColor);
				
				//dc.FillRect(m_rectBlock[x][y], &bru);
			}
		}
		
		dc.SelectObject(pOldBrush);

		
		
		
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CRussiaBlockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//初始化框架
void CRussiaBlockDlg::InitFrame(void)
{
	int xleft = 20;
	int ytop = 20;
	
	
	
	//游戏区rect初始化
	m_rectGame.left = xleft;
	m_rectGame.right = m_rectGame.left +  m_frm.GetWidth()*20;
	m_rectGame.top = ytop;
	m_rectGame.bottom = m_rectGame.top + m_frm.GetHeight()*20;
	//next区rect初始化
	m_rectNext.left = 410;
	m_rectNext.top = 340;
	m_rectNext.right = m_rectNext.left + 60;
	m_rectNext.bottom = m_rectNext.top + 80;
	//获取框架指针
	CBlock::GetFramePt(&m_frm);

	//初始化方块矩阵
	for (int x=0; x!=m_frm.GetWidth(); ++x)
	{
		vector<CRect> tmp;
		for (int y=0; y!=m_frm.GetHeight(); ++y)
		{
			tmp.push_back(CRect(m_rectGame.left + x*20, m_rectGame.bottom - (y+1)*20,
								m_rectGame.left + (x+1)*20, m_rectGame.bottom - y*20));
		}
		m_rectBlock.push_back(tmp);
	}

}

//键盘响应
void CRussiaBlockDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (m_pCurBlock != NULL)
	{
		//下落方块所在的稍大一点的矩形
		CRect rtCur, tmp;
		tmp = GetCurrentBlockRect();
		rtCur.left = tmp.left - 40;
		rtCur.right = tmp.right + 40;
		rtCur.top = tmp.top - 40;
		rtCur.bottom = tmp.bottom + 40;

		if (nChar == VK_DOWN)
		{
			if (!m_pCurBlock->MoveDown())
			{
				if (m_frm.IsOver())
				{
					GameOver();
				}
				if (m_frm.Check())	//检查是否能消行
				{
					::sndPlaySound(_T("slash"), SND_ASYNC);
					//判定通关
					IsGameWin();
					
				}

				//显示等级，消行次数
				CString tmp;
				tmp.Format(_T("%d"), m_frm.GetLevel());
				SetDlgItemText(IDC_LEVEL, tmp);
				tmp.Empty();
				tmp.Format(_T("%d"), m_frm.GetSlashTime());
				SetDlgItemText(IDC_SLASH, tmp);

				//如果达到升级条件的话重新设置定时器
				KillTimer(GAME_TIME);
				SetTimer(GAME_TIME, 1000 - 100*m_frm.level, NULL);

				//删除此方块指针，再创建一个新方块
				delete m_pCurBlock;
				m_pCurBlock = NULL;
				m_pCurBlock = m_pNextBlock;
				m_pNextBlock = CBlock::CreateObject();
				//新方块的颜色变换为下一种
				m_nColor = ++m_nColor % 7;

				InvalidateRect(&m_rectNext);	//使next区无效
				InvalidateRect(&m_rectGame);	//使game区无效
			}

			else
			{
				
				InvalidateRect(&rtCur);
				//Invalidate();
			}
		}
		if (nChar == VK_LEFT)
		{
			m_pCurBlock->MoveLeft();

			InvalidateRect(&rtCur);
		}
		if (nChar == VK_RIGHT)
		{
			m_pCurBlock->MoveRight();

			InvalidateRect(&rtCur);
		}
		if (nChar == VK_UP)
		{
			m_pCurBlock->Rotate();

			InvalidateRect(&rtCur);
		}
	}
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

//销毁
void CRussiaBlockDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	((CRussiaBlockApp*)AfxGetApp())->m_hwndDlg = NULL;
	if (m_Audio != NULL)
	{
		MCIWndDestroy(m_Audio);
		m_Audio = NULL;
	}
	KillTimer(GAME_TIME);
	KillTimer(DISPLAY_TIME);
	KillTimer(BKP_TIME);
}

//重绘背景
BOOL CRussiaBlockDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	static bool bInit = false;
	if (m_bkpFileName.size() != 0)
	{
		if (bInit)
		{
			m_imgBK.Destroy();
			bInit = false;
		}
		CString strPath;
		strPath = _T("bkp\\") + m_bkpFileName[m_nBkpIndex];
		m_imgBK.Load(strPath);
		
		m_imgBK.Draw(pDC->GetSafeHdc(), 0, 0, m_rectClient.Width(), m_rectClient.Height());
		bInit = true;
		return TRUE;
	}

	return CDialog::OnEraseBkgnd(pDC);
}




//开始按钮
void CRussiaBlockDlg::OnBnClickedBtnStart()
{
	// TODO: Add your control notification handler code here
	//将焦点放在对话框上，从而使得按钮不会响应方向键
	SetFocus();

	m_frm.Initial();
	/*	m_pCurBlock和m_pNextBlock都需要创建，不然按下开始按钮后执行到OnTimer时会出错	*/
	m_pCurBlock = CBlock::CreateObject();
	m_pNextBlock = CBlock::CreateObject();

	SetTimer(GAME_TIME, 1000 - 100*m_frm.level, NULL);	//游戏计时
	SetTimer(DISPLAY_TIME, 1000, NULL);	//显示计时
	SetTimer(BKP_TIME, 30000, NULL);	//背景更换计时
	m_nTime = 0;

	//显示等级，消行次数
	CString tmp;
	tmp.Format(_T("%d"), m_frm.GetLevel());
	SetDlgItemText(IDC_LEVEL, tmp);
	tmp.Empty();
	tmp.Format(_T("%d"), m_frm.GetSlashTime());
	SetDlgItemText(IDC_SLASH, tmp);

	InvalidateRect(&m_rectGame);
	InvalidateRect(&m_rectNext);
	
}

//选项按钮
void CRussiaBlockDlg::OnBnClickedBtnOption()
{
	// TODO: Add your control notification handler code here
	//将焦点放在对话框上，从而使得按钮不会响应方向键
	SetFocus();

	
}

//暂停按钮
void CRussiaBlockDlg::OnBnClickedBtnPause()
{
	// TODO: Add your control notification handler code here
	if (m_pCurBlock == NULL)
	{
		return;
	}

	//将焦点放在对话框上，从而使得按钮不会响应方向键
	SetFocus();
	static bool flag = false;
	if (!flag)
	{
		KillTimer(GAME_TIME);
		KillTimer(DISPLAY_TIME);
		((CButton*)GetDlgItem(IDC_BTN_PAUSE))->SetWindowTextW(_T("恢复"));
		flag = !flag;
	}
	else
	{
		SetTimer(GAME_TIME, 1000 - 100*m_frm.level, NULL);
		SetTimer(DISPLAY_TIME, 1000, NULL);
		((CButton*)GetDlgItem(IDC_BTN_PAUSE))->SetWindowTextW(_T("暂停"));
		flag = !flag;
	}
}

//定时器
void CRussiaBlockDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == GAME_TIME)
	{
		
		//方块到底或者落在其他的block上
		if (!m_pCurBlock->MoveDown())
		{
			if (m_frm.IsOver())
			{
				GameOver();
			}
			if (m_frm.Check())	//检查是否能消行
			{
				::sndPlaySound(_T("slash"), SND_ASYNC);
				IsGameWin();
				
			}

			//显示等级，消行次数
			CString tmp;
			tmp.Format(_T("%d"), m_frm.GetLevel());
			SetDlgItemText(IDC_LEVEL, tmp);
			tmp.Empty();
			tmp.Format(_T("%d"), m_frm.GetSlashTime());
			SetDlgItemText(IDC_SLASH, tmp);

			//如果达到升级条件的话重新设置定时器
			KillTimer(GAME_TIME);
			SetTimer(GAME_TIME, 1000 - 100*m_frm.level, NULL);

			//删除此方块指针，再创建一个新方块
			delete m_pCurBlock;
			m_pCurBlock = NULL;
			m_pCurBlock = m_pNextBlock;
			m_pNextBlock = CBlock::CreateObject();

			//新方块的颜色变换为下一种
			m_nColor = ++m_nColor % 7;

			InvalidateRect(&m_rectNext);	//使next区无效
			InvalidateRect(&m_rectGame);
		}
		else
		{
			//使下落方块所在的稍大一点的矩形无效重绘
			CRect rtCur, tmp;
			tmp = GetCurrentBlockRect();
			rtCur.left = tmp.left - 40;
			rtCur.right = tmp.right + 40;
			rtCur.top = tmp.top - 40;
			rtCur.bottom = tmp.bottom + 40;
			InvalidateRect(&rtCur);
		}
	}
	//显示控件时间
	if (nIDEvent == DISPLAY_TIME)
	{
		//显示时间
		CString strTime;
		strTime.Format(_T("%d"), m_nTime++);
		SetDlgItemText(IDC_TIME, strTime);
		
	}
	//背景图片更换的间隔
	if (nIDEvent == BKP_TIME)
	{
		m_nBkpIndex = ++m_nBkpIndex % static_cast<int>(m_bkpFileName.size());
		SendMessage(WM_NCPAINT);
		Invalidate();
	}

}
void CRussiaBlockDlg::GameOver(void)
{
	KillTimer(GAME_TIME);
	KillTimer(DISPLAY_TIME);

	if (IDOK == MessageBox(_T("你挂了！")))
	{
		OnBnClickedBtnStart();
	}
	
}

//获取当前下落的方块所在的rect，以便于调用InvalidateRect
const CRect CRussiaBlockDlg::GetCurrentBlockRect(void)
{
	//TODO: insert return statement here
	CRect rt;
	int left = m_pCurBlock->GetBlockBound(CBlock::LEFT);
	int right = m_pCurBlock->GetBlockBound(CBlock::RIGHT);
	int up = m_pCurBlock->GetBlockBound(CBlock::UP);
	int down = m_pCurBlock->GetBlockBound(CBlock::DOWN);

	rt.left = left*20 + 20;
	rt.right = (right+1)*20 + 20;
	rt.top = 400 - (up+1)*20;
	rt.bottom = 400 - down*20;

	return rt;
}

//给方块贴位图，num为位图索引
void CRussiaBlockDlg::DrawBlock(const CRect& rt, CDC* pDC, int num)
{
	/*
	static int num = 0;
	if (bRandom)
	{
		//产生随机数num
		double n = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
		int num = static_cast<int>(n * 7);
	}
	else
	{
		num = ++num%7;
	}
	*/

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	dcMem.SelectObject(m_bitmapAry[num]);
	pDC->BitBlt(rt.left, rt.top, rt.Width(), rt.Height(),
		&dcMem, 0, 0, SRCCOPY);

	dcMem.DeleteDC();
}

HBRUSH CRussiaBlockDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		CBrush bru;
		bru.CreateStockObject(NULL_BRUSH);
		return (HBRUSH)bru.GetSafeHandle();
	}

	return hbr;
}

//获取所有背景图片的文件名
bool CRussiaBlockDlg::GetBkpFile(void)
{
	CFileFind filefind;
	if (filefind.FindFile(_T("bkp\\*.jpg"), 0) != 0)
	{
		while (filefind.FindNextFileW() != 0)
		{
			m_bkpFileName.push_back(filefind.GetFileName());
		}
		m_bkpFileName.push_back(filefind.GetFileName());

		return true;
	}
	return false;
}

//获取所有背景音乐的文件名
bool CRussiaBlockDlg::GetMusicFile(void)
{
	CFileFind filefind;
	if (filefind.FindFile(_T("bkm\\*.mp3"), 0) != 0)
	{
		while (filefind.FindNextFileW() != 0)
		{
			m_MusicFileName.push_back(filefind.GetFileName());
		}
		m_MusicFileName.push_back(filefind.GetFileName());

		return true;
	}
	return false;
}

//播放音乐状态的消息响应函数
LRESULT CRussiaBlockDlg::OnMCINotify(WPARAM wParam,LPARAM lParam)
{
	if(lParam == MCI_MODE_STOP)
	{
		static int num = 1;
		num = num % static_cast<int>(m_MusicFileName.size());
		MCIWndStop(m_Audio);	//先停止再播放下首
		MCIWndDestroy(m_Audio);
		m_Audio = NULL;

		CString strPath;
		strPath = _T("bkm\\") + m_MusicFileName[num];
		m_Audio = ::MCIWndCreate(this->m_hWnd, AfxGetInstanceHandle(), WS_CHILD | MCIWNDF_NOTIFYMODE, 
			strPath); 
		MCIWndPlay(m_Audio);

		++num;
	}
	return 1;
}

//游戏通关
bool CRussiaBlockDlg::IsGameWin(void)
{
	//胜利条件
	if (m_frm.GetLevel() == 9 && m_frm.GetSlashTime() == 10)
	{
		KillTimer(GAME_TIME);
		
		KillTimer(DISPLAY_TIME);
		KillTimer(BKP_TIME);
		
		m_pDlg->Create(IDD_PASSWORD, this);
		m_pDlg->ShowWindow(SW_SHOW);
		
		return true;
	}

	return false;
}
