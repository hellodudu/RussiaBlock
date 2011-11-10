// RussiaBlockDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RussiaBlock.h"
#include "RussiaBlockDlg.h"
#include "Password.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CRussiaBlockDlg �Ի���




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


// CRussiaBlockDlg ��Ϣ�������

BOOL CRussiaBlockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	((CRussiaBlockApp*)AfxGetApp())->m_hwndDlg = m_hWnd;	//������Ӧ
	
	InitFrame();
	m_pCurBlock = NULL;
	m_pNextBlock = NULL;
	//����Client����ʼ��
	GetClientRect(&m_rectClient);

	//���뷽��λͼ
	for (int i=0; i!=7; ++i)
	{
		m_bitmapAry[i].LoadBitmapW(IDB_BMP_BLOCK1+i);
	}

	//��������ļ�������jpg�ļ��Ļ��ͽ��������m_imgBK
	if (GetBkpFile())
	{
		m_imgBK.Load(m_bkpFileName[0]);
	}

	//������������ļ�������mp3�ļ��Ļ��ͽ��������m_MusicFileName
	if (GetMusicFile())
	{
		CString strPath;
		strPath = _T("bkm\\") + m_MusicFileName[0];
		m_Audio = ::MCIWndCreate(this->m_hWnd, AfxGetInstanceHandle(), WS_CHILD | MCIWNDF_NOTIFYMODE, 
			strPath); 
		MCIWndPlay(m_Audio);
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRussiaBlockDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);

		

		//�����������
		CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
		dc.Rectangle(m_rectGame);
		dc.Rectangle(m_rectGame.left-1, m_rectGame.top-1, m_rectGame.right+1, m_rectGame.bottom+1);

		
		//�����
		for (int x=0; x!=m_rectBlock.size(); ++x)
		{
			for (int y=0; y!=m_rectBlock[x].size(); ++y)
			{
				
				//dc.Rectangle(m_rectBlock[x][y]);

				if (m_frm.container[x][y])
				{	//dc.FillRect(m_rectBlock[x][y], &bru);	//����Ѷѻ��ĵײ�
					DrawBlock(m_rectBlock[x][y], &dc);

				}
			}
		}

		//��m_pNextBlock
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
		

		//������ķ���
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CRussiaBlockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��ʼ�����
void CRussiaBlockDlg::InitFrame(void)
{
	int xleft = 20;
	int ytop = 20;
	
	
	
	//��Ϸ��rect��ʼ��
	m_rectGame.left = xleft;
	m_rectGame.right = m_rectGame.left +  m_frm.GetWidth()*20;
	m_rectGame.top = ytop;
	m_rectGame.bottom = m_rectGame.top + m_frm.GetHeight()*20;
	//next��rect��ʼ��
	m_rectNext.left = 410;
	m_rectNext.top = 340;
	m_rectNext.right = m_rectNext.left + 60;
	m_rectNext.bottom = m_rectNext.top + 80;
	//��ȡ���ָ��
	CBlock::GetFramePt(&m_frm);

	//��ʼ���������
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

//������Ӧ
void CRussiaBlockDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (m_pCurBlock != NULL)
	{
		//���䷽�����ڵ��Դ�һ��ľ���
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
				if (m_frm.Check())	//����Ƿ�������
				{
					::sndPlaySound(_T("slash"), SND_ASYNC);
					//�ж�ͨ��
					IsGameWin();
					
				}

				//��ʾ�ȼ������д���
				CString tmp;
				tmp.Format(_T("%d"), m_frm.GetLevel());
				SetDlgItemText(IDC_LEVEL, tmp);
				tmp.Empty();
				tmp.Format(_T("%d"), m_frm.GetSlashTime());
				SetDlgItemText(IDC_SLASH, tmp);

				//����ﵽ���������Ļ��������ö�ʱ��
				KillTimer(GAME_TIME);
				SetTimer(GAME_TIME, 1000 - 100*m_frm.level, NULL);

				//ɾ���˷���ָ�룬�ٴ���һ���·���
				delete m_pCurBlock;
				m_pCurBlock = NULL;
				m_pCurBlock = m_pNextBlock;
				m_pNextBlock = CBlock::CreateObject();
				//�·������ɫ�任Ϊ��һ��
				m_nColor = ++m_nColor % 7;

				InvalidateRect(&m_rectNext);	//ʹnext����Ч
				InvalidateRect(&m_rectGame);	//ʹgame����Ч
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

//����
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

//�ػ汳��
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




//��ʼ��ť
void CRussiaBlockDlg::OnBnClickedBtnStart()
{
	// TODO: Add your control notification handler code here
	//��������ڶԻ����ϣ��Ӷ�ʹ�ð�ť������Ӧ�����
	SetFocus();

	m_frm.Initial();
	/*	m_pCurBlock��m_pNextBlock����Ҫ��������Ȼ���¿�ʼ��ť��ִ�е�OnTimerʱ�����	*/
	m_pCurBlock = CBlock::CreateObject();
	m_pNextBlock = CBlock::CreateObject();

	SetTimer(GAME_TIME, 1000 - 100*m_frm.level, NULL);	//��Ϸ��ʱ
	SetTimer(DISPLAY_TIME, 1000, NULL);	//��ʾ��ʱ
	SetTimer(BKP_TIME, 30000, NULL);	//����������ʱ
	m_nTime = 0;

	//��ʾ�ȼ������д���
	CString tmp;
	tmp.Format(_T("%d"), m_frm.GetLevel());
	SetDlgItemText(IDC_LEVEL, tmp);
	tmp.Empty();
	tmp.Format(_T("%d"), m_frm.GetSlashTime());
	SetDlgItemText(IDC_SLASH, tmp);

	InvalidateRect(&m_rectGame);
	InvalidateRect(&m_rectNext);
	
}

//ѡ�ť
void CRussiaBlockDlg::OnBnClickedBtnOption()
{
	// TODO: Add your control notification handler code here
	//��������ڶԻ����ϣ��Ӷ�ʹ�ð�ť������Ӧ�����
	SetFocus();

	
}

//��ͣ��ť
void CRussiaBlockDlg::OnBnClickedBtnPause()
{
	// TODO: Add your control notification handler code here
	if (m_pCurBlock == NULL)
	{
		return;
	}

	//��������ڶԻ����ϣ��Ӷ�ʹ�ð�ť������Ӧ�����
	SetFocus();
	static bool flag = false;
	if (!flag)
	{
		KillTimer(GAME_TIME);
		KillTimer(DISPLAY_TIME);
		((CButton*)GetDlgItem(IDC_BTN_PAUSE))->SetWindowTextW(_T("�ָ�"));
		flag = !flag;
	}
	else
	{
		SetTimer(GAME_TIME, 1000 - 100*m_frm.level, NULL);
		SetTimer(DISPLAY_TIME, 1000, NULL);
		((CButton*)GetDlgItem(IDC_BTN_PAUSE))->SetWindowTextW(_T("��ͣ"));
		flag = !flag;
	}
}

//��ʱ��
void CRussiaBlockDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == GAME_TIME)
	{
		
		//���鵽�׻�������������block��
		if (!m_pCurBlock->MoveDown())
		{
			if (m_frm.IsOver())
			{
				GameOver();
			}
			if (m_frm.Check())	//����Ƿ�������
			{
				::sndPlaySound(_T("slash"), SND_ASYNC);
				IsGameWin();
				
			}

			//��ʾ�ȼ������д���
			CString tmp;
			tmp.Format(_T("%d"), m_frm.GetLevel());
			SetDlgItemText(IDC_LEVEL, tmp);
			tmp.Empty();
			tmp.Format(_T("%d"), m_frm.GetSlashTime());
			SetDlgItemText(IDC_SLASH, tmp);

			//����ﵽ���������Ļ��������ö�ʱ��
			KillTimer(GAME_TIME);
			SetTimer(GAME_TIME, 1000 - 100*m_frm.level, NULL);

			//ɾ���˷���ָ�룬�ٴ���һ���·���
			delete m_pCurBlock;
			m_pCurBlock = NULL;
			m_pCurBlock = m_pNextBlock;
			m_pNextBlock = CBlock::CreateObject();

			//�·������ɫ�任Ϊ��һ��
			m_nColor = ++m_nColor % 7;

			InvalidateRect(&m_rectNext);	//ʹnext����Ч
			InvalidateRect(&m_rectGame);
		}
		else
		{
			//ʹ���䷽�����ڵ��Դ�һ��ľ�����Ч�ػ�
			CRect rtCur, tmp;
			tmp = GetCurrentBlockRect();
			rtCur.left = tmp.left - 40;
			rtCur.right = tmp.right + 40;
			rtCur.top = tmp.top - 40;
			rtCur.bottom = tmp.bottom + 40;
			InvalidateRect(&rtCur);
		}
	}
	//��ʾ�ؼ�ʱ��
	if (nIDEvent == DISPLAY_TIME)
	{
		//��ʾʱ��
		CString strTime;
		strTime.Format(_T("%d"), m_nTime++);
		SetDlgItemText(IDC_TIME, strTime);
		
	}
	//����ͼƬ�����ļ��
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

	if (IDOK == MessageBox(_T("����ˣ�")))
	{
		OnBnClickedBtnStart();
	}
	
}

//��ȡ��ǰ����ķ������ڵ�rect���Ա��ڵ���InvalidateRect
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

//��������λͼ��numΪλͼ����
void CRussiaBlockDlg::DrawBlock(const CRect& rt, CDC* pDC, int num)
{
	/*
	static int num = 0;
	if (bRandom)
	{
		//���������num
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

//��ȡ���б���ͼƬ���ļ���
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

//��ȡ���б������ֵ��ļ���
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

//��������״̬����Ϣ��Ӧ����
LRESULT CRussiaBlockDlg::OnMCINotify(WPARAM wParam,LPARAM lParam)
{
	if(lParam == MCI_MODE_STOP)
	{
		static int num = 1;
		num = num % static_cast<int>(m_MusicFileName.size());
		MCIWndStop(m_Audio);	//��ֹͣ�ٲ�������
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

//��Ϸͨ��
bool CRussiaBlockDlg::IsGameWin(void)
{
	//ʤ������
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
