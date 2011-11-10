#include "Block.h"
#include "Frame.h"

//初始化静态成员
CFrame* CBlock::g_pFrm = NULL;

//CBlock静态成员，获取CFrame指针
CFrame* CBlock::GetFramePt(CFrame* pfm)
{
	return g_pFrm = pfm;
}

//判断是否m_block会和frame重叠
bool CBlock::IsCrashed(POSITION pos) const
{
	Square_vec tmp(m_block);
	switch(pos)
	{
	case NO:
		for (int i=0; i!=tmp.size(); ++i)
		{
			if (g_pFrm->container[tmp[i].x][tmp[i].y])
			{
				return true;
			}
		}
		return false;
		break;
	case UP:
		break;
	case DOWN:
		for (int i=0; i!=tmp.size(); ++i)
		{
			--tmp[i].y;
			if (g_pFrm->container[tmp[i].x][tmp[i].y])
			{
				return true;
			}
		}
		return false;
		break;
	case LEFT:
		for (int i=0; i!=tmp.size(); ++i)
		{
			--tmp[i].x;
			if (g_pFrm->container[tmp[i].x][tmp[i].y])
			{
				return true;
			}
		}
		return false;
		break;
	case RIGHT:
		for (int i=0; i!=tmp.size(); ++i)
		{
			++tmp[i].x;
			if (g_pFrm->container[tmp[i].x][tmp[i].y])
			{
				return true;
			}
		}
		return false;
		break;
	}

	return true;
}

//方块向左移动
void CBlock::MoveLeft()
{
	if (GetBlockBound(LEFT) <= 0)
	{
		return;
	}
	//先判断是否会撞击
	if (IsCrashed(LEFT))
	{
		cout << "Crashed!" << endl;
		return;
	}
	else
	{
		for (int i=0; i!=m_block.size(); ++i)
		{
			--m_block[i].x;
		}
		--m_ptRotate.x;
		cout << "CBlock::MoveLeft" << endl;
	}
}

//方块向右移动
void CBlock::MoveRight()
{
	if (GetBlockBound(RIGHT) >= g_pFrm->GetWidth() - 1)
	{
		return;
	}
	//先判断是否会撞击
	if (IsCrashed(RIGHT))
	{
		cout << "Crashed!" << endl;
		return;
	}
	else
	{
		for (int i=0; i!=m_block.size(); ++i)
		{
			++m_block[i].x;
		}
		++m_ptRotate.x;
		cout << "CBlock::MoveRight" << endl;
	}
}

//方块向上移动
void CBlock::MoveUp()
{
	cout << "CBlock::MoveUp" << endl;
}

//方块向下移动
bool CBlock::MoveDown()
{
	if (GetBlockBound(DOWN) <= 0)
	{
		for (int i=0; i!=m_block.size(); ++i)
		{
			g_pFrm->SetFilled(m_block[i].x, m_block[i].y);
		}
		return false;
	}
	//先判断是否会撞击
	if (IsCrashed(DOWN))
	{
		for (int i=0; i!=m_block.size(); ++i)
		{
			g_pFrm->SetFilled(m_block[i].x, m_block[i].y);
		}
		cout << "Crashed!" << endl;
		return false;
	}
	else
	{
		for (int i=0; i!=m_block.size(); ++i)
		{
			--m_block[i].y;
		}
		--m_ptRotate.y;
		cout << "CBlock::MoveDown" << endl;
		return true;
	}
}

/*	正反L型方块，正反Z型方块和T型方块都调用基类的Rotate来旋转，
思路即是将其放入一个3X3的矩阵中，然后旋转矩阵就行了，各方块在旋转时旋转点不动	*/
void CBlock::Rotate()
{
	//在最顶上的时候不能旋转，因为可能会超出界限
	if (GetBlockBound(UP) >= g_pFrm->GetHeight()-1)
	{
		return;
	}

	int rect[3][3] = {};
	for (int i=0; i!=m_block.size(); ++i)
	{
		int x = m_block[i].x - m_ptRotate.x + 1;
		int y = m_block[i].y - m_ptRotate.y + 1;
		rect[y][x] = 1;
	}

	//旋转3x3数组
	int tmp[3][3] = {};
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            tmp[i][j] = rect[j][2-i];
        }
    }

	
	Square_vec tmpvec;
	tmpvec.reserve(m_block.size());
    for (int i=0;i<3;i++)
    {
        for (int j=0;j<3;j++)
        {
			if (tmp[i][j] == 1)
			{
				//Square newsqr(j + m_ptRotate.x-1, i + m_ptRotate.y-1);
				tmpvec.push_back(Square(j+m_ptRotate.x-1, i+m_ptRotate.y-1));
			}
        }
    }

	//判断旋转后的方块是否会发生越界
	/*
	for (int i=0; i!=tmpvec.size(); ++i)
	{
		if (tmpvec[i].x <= 0 || tmpvec[i].x >= g_pFrm->GetWidth() || 
			tmpvec[i].y <= 0 || tmpvec[i].y >= g_pFrm->GetHeight())
			return;
	}
	*/
	m_block.swap(tmpvec);
	//判断旋转后的方块是否会发生越界
	if (GetBlockBound(LEFT) < 0 || GetBlockBound(RIGHT) > g_pFrm->GetWidth()-1 ||
		GetBlockBound(DOWN) < 0)
	{
		m_block.swap(tmpvec);
		return;
	}

	//判断旋转后的方块是否会和frame内的方块发生重叠
	if (IsCrashed(NO))
	{
		m_block.swap(tmpvec);
		return;
	}

	cout << "CBlock::Rotate" << endl;
}

/*	获取方块的边界值，参数确定要获取边界的方向	*/
int CBlock::GetBlockBound(const POSITION pos) const
{
	
	vector<int> ivec;
	ivec.reserve(m_block.size());
	switch(pos)
	{
		//获取方块的上边界，即m_block中y坐标值最大的
		case UP:
			for (int i=0; i!=m_block.size(); ++i)
			{
				ivec.push_back(m_block[i].y);
			}
			return *max_element(ivec.begin(), ivec.end());
			break;
		//获取方块的下边界
		case DOWN:
			for (int i=0; i!=m_block.size(); ++i)
			{
				ivec.push_back(m_block[i].y);
			}
			return *min_element(ivec.begin(), ivec.end());
			break;
		//获取方块的左边界
		case LEFT:
			for (int i=0; i!=m_block.size(); ++i)
			{
				ivec.push_back(m_block[i].x);
			}
			return *min_element(ivec.begin(), ivec.end());
			break;
		//获取方块的右边界
		case RIGHT:
			for (int i=0; i!=m_block.size(); ++i)
			{
				ivec.push_back(m_block[i].x);
			}
			return *max_element(ivec.begin(), ivec.end());
			break;
		default:
			return -1;	//return error!
			break;
	}
}

//返回m_block
inline const CBlock::Square_vec& CBlock::GetBlock() const
{
	return m_block;
}

//返回旋转点
inline const Square& CBlock::GetRotatePt() const
{
	return m_ptRotate;
}

//随机产生方块
CBlock* CBlock::CreateObject()
{
	
	double n = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
	int num = static_cast<int>(n * CBlock::NUMBERS);	//七种方块

	switch(num)
	{
	case CBlock::OBL:
		return new CObverseL;
	case CBlock::OPL:
		return new COpposeL;
	case CBlock::OBZ:
		return new CObverseZ;
	case CBlock::OPZ:
		return new COpposeZ;
	case CBlock::T:
		return new CT;
	case CBlock::TIAN:
		return new CTian;
	case CBlock::BAR:
		return new CBar;
	default:
		return NULL;
	}
}

/*	反L方块类	*/
COpposeL::COpposeL()
{

	/*	将其传递进来的参数作为该方块的旋转点来进行方块的构造，
	这样能够统一各种形状的构造函数参数，因为所有形状的方块其旋
	转点是唯一的，即旋转的时候图形绕着该点进行旋转	*/
	int x = g_pFrm->GetWidth()/2 - 1;
	int y = g_pFrm->GetHeight() - 2;

	m_ptRotate = Square(x, y);
	m_block.reserve(4);	//缩减vector空间
	m_block.push_back(Square(x, y-1));
	m_block.push_back(Square(x+1, y-1));
	m_block.push_back(Square(x+1, y));
	m_block.push_back(Square(x+1, y+1));
	
}

/*	正L方块类	*/
CObverseL::CObverseL()
{
	int x = g_pFrm->GetWidth()/2 - 1;
	int y = g_pFrm->GetHeight() - 2;

	m_ptRotate = Square(x, y);
	m_block.reserve(4);	//缩减vector空间
	m_block.push_back(Square(x, y-1));
	m_block.push_back(Square(x-1, y-1));
	m_block.push_back(Square(x-1, y));
	m_block.push_back(Square(x-1, y+1));
}

/*	正Z方块类	*/
CObverseZ::CObverseZ()
{
	int x = g_pFrm->GetWidth()/2 - 1;
	int y = g_pFrm->GetHeight() - 1;

	m_ptRotate = Square(x, y);
	m_block.reserve(4);	//缩减vector空间

	m_block.push_back(Square(x, y));
	m_block.push_back(Square(x-1, y));
	m_block.push_back(Square(x, y-1));
	m_block.push_back(Square(x+1, y-1));
}

/*	反Z方块类	*/
COpposeZ::COpposeZ()
{
	int x = g_pFrm->GetWidth()/2 - 1;
	int y = g_pFrm->GetHeight() - 1;

	m_ptRotate = Square(x, y);
	m_block.reserve(4);	//缩减vector空间

	m_block.push_back(Square(x, y));
	m_block.push_back(Square(x+1, y));
	m_block.push_back(Square(x, y-1));
	m_block.push_back(Square(x-1, y-1));
}

/*	长条方块类	*/
CBar::CBar()
{
	int x = g_pFrm->GetWidth()/2 - 1;
	int y = g_pFrm->GetHeight() - 3;

	m_ptRotate = Square(x, y);
	m_block.reserve(4);	//缩减vector空间

	m_block.push_back(Square(x, y));
	m_block.push_back(Square(x, y-1));
	m_block.push_back(Square(x, y+1));
	m_block.push_back(Square(x, y+2));
}

/*	田字方块类	*/
CTian::CTian()
{
	int x = g_pFrm->GetWidth()/2;
	int y = g_pFrm->GetHeight() - 1;

	m_ptRotate = Square(x, y);
	m_block.reserve(4);	//缩减vector空间

	m_block.push_back(Square(x, y));
	m_block.push_back(Square(x-1, y));
	m_block.push_back(Square(x-1, y-1));
	m_block.push_back(Square(x, y-1));
}

/*	T字方块类	*/
CT::CT()
{
	int x = g_pFrm->GetWidth()/2 - 1;
	int y = g_pFrm->GetHeight() - 1;

	m_ptRotate = Square(x, y);
	m_block.reserve(4);	//缩减vector空间

	m_block.push_back(Square(x, y));
	m_block.push_back(Square(x-1, y));
	m_block.push_back(Square(x+1, y));
	m_block.push_back(Square(x, y-1));
}

//重写长条方块的旋转方法
void CBar::Rotate()
{
	Square_vec tmpvec;
	tmpvec.reserve(m_block.size());

	//如果是竖的则变成横的，反之亦然
	if (m_block[0].y - m_block[1].y != 0)
	{
		tmpvec.push_back(m_ptRotate);
		tmpvec.push_back(Square(m_ptRotate.x-2, m_ptRotate.y));
		tmpvec.push_back(Square(m_ptRotate.x-1, m_ptRotate.y));
		tmpvec.push_back(Square(m_ptRotate.x+1, m_ptRotate.y));
	}
	else
	{
		tmpvec.push_back(m_ptRotate);
		tmpvec.push_back(Square(m_ptRotate.x, m_ptRotate.y+2));
		tmpvec.push_back(Square(m_ptRotate.x, m_ptRotate.y+1));
		tmpvec.push_back(Square(m_ptRotate.x, m_ptRotate.y-1));
	}

	m_block.swap(tmpvec);

	//判断旋转后的方块是否会发生越界
	if (GetBlockBound(LEFT) < 0 || GetBlockBound(RIGHT) > g_pFrm->GetWidth()-1 ||
		GetBlockBound(DOWN) < 0)
	{
		m_block.swap(tmpvec);
		return;
	}

	//判断旋转后的方块是否会和frame内的方块发生重叠
	if (IsCrashed(NO))
	{
		m_block.swap(tmpvec);
		return;
	}
}

//重写田字方块的旋转方法
void CTian::Rotate()
{
	return;
}