#include "Block.h"
#include "Frame.h"

//��ʼ����̬��Ա
CFrame* CBlock::g_pFrm = NULL;

//CBlock��̬��Ա����ȡCFrameָ��
CFrame* CBlock::GetFramePt(CFrame* pfm)
{
	return g_pFrm = pfm;
}

//�ж��Ƿ�m_block���frame�ص�
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

//���������ƶ�
void CBlock::MoveLeft()
{
	if (GetBlockBound(LEFT) <= 0)
	{
		return;
	}
	//���ж��Ƿ��ײ��
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

//���������ƶ�
void CBlock::MoveRight()
{
	if (GetBlockBound(RIGHT) >= g_pFrm->GetWidth() - 1)
	{
		return;
	}
	//���ж��Ƿ��ײ��
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

//���������ƶ�
void CBlock::MoveUp()
{
	cout << "CBlock::MoveUp" << endl;
}

//���������ƶ�
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
	//���ж��Ƿ��ײ��
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

/*	����L�ͷ��飬����Z�ͷ����T�ͷ��鶼���û����Rotate����ת��
˼·���ǽ������һ��3X3�ľ����У�Ȼ����ת��������ˣ�����������תʱ��ת�㲻��	*/
void CBlock::Rotate()
{
	//����ϵ�ʱ������ת����Ϊ���ܻᳬ������
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

	//��ת3x3����
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

	//�ж���ת��ķ����Ƿ�ᷢ��Խ��
	/*
	for (int i=0; i!=tmpvec.size(); ++i)
	{
		if (tmpvec[i].x <= 0 || tmpvec[i].x >= g_pFrm->GetWidth() || 
			tmpvec[i].y <= 0 || tmpvec[i].y >= g_pFrm->GetHeight())
			return;
	}
	*/
	m_block.swap(tmpvec);
	//�ж���ת��ķ����Ƿ�ᷢ��Խ��
	if (GetBlockBound(LEFT) < 0 || GetBlockBound(RIGHT) > g_pFrm->GetWidth()-1 ||
		GetBlockBound(DOWN) < 0)
	{
		m_block.swap(tmpvec);
		return;
	}

	//�ж���ת��ķ����Ƿ���frame�ڵķ��鷢���ص�
	if (IsCrashed(NO))
	{
		m_block.swap(tmpvec);
		return;
	}

	cout << "CBlock::Rotate" << endl;
}

/*	��ȡ����ı߽�ֵ������ȷ��Ҫ��ȡ�߽�ķ���	*/
int CBlock::GetBlockBound(const POSITION pos) const
{
	
	vector<int> ivec;
	ivec.reserve(m_block.size());
	switch(pos)
	{
		//��ȡ������ϱ߽磬��m_block��y����ֵ����
		case UP:
			for (int i=0; i!=m_block.size(); ++i)
			{
				ivec.push_back(m_block[i].y);
			}
			return *max_element(ivec.begin(), ivec.end());
			break;
		//��ȡ������±߽�
		case DOWN:
			for (int i=0; i!=m_block.size(); ++i)
			{
				ivec.push_back(m_block[i].y);
			}
			return *min_element(ivec.begin(), ivec.end());
			break;
		//��ȡ�������߽�
		case LEFT:
			for (int i=0; i!=m_block.size(); ++i)
			{
				ivec.push_back(m_block[i].x);
			}
			return *min_element(ivec.begin(), ivec.end());
			break;
		//��ȡ������ұ߽�
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

//����m_block
inline const CBlock::Square_vec& CBlock::GetBlock() const
{
	return m_block;
}

//������ת��
inline const Square& CBlock::GetRotatePt() const
{
	return m_ptRotate;
}

//�����������
CBlock* CBlock::CreateObject()
{
	
	double n = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
	int num = static_cast<int>(n * CBlock::NUMBERS);	//���ַ���

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

/*	��L������	*/
COpposeL::COpposeL()
{

	/*	���䴫�ݽ����Ĳ�����Ϊ�÷������ת�������з���Ĺ��죬
	�����ܹ�ͳһ������״�Ĺ��캯����������Ϊ������״�ķ�������
	ת����Ψһ�ģ�����ת��ʱ��ͼ�����Ÿõ������ת	*/
	int x = g_pFrm->GetWidth()/2 - 1;
	int y = g_pFrm->GetHeight() - 2;

	m_ptRotate = Square(x, y);
	m_block.reserve(4);	//����vector�ռ�
	m_block.push_back(Square(x, y-1));
	m_block.push_back(Square(x+1, y-1));
	m_block.push_back(Square(x+1, y));
	m_block.push_back(Square(x+1, y+1));
	
}

/*	��L������	*/
CObverseL::CObverseL()
{
	int x = g_pFrm->GetWidth()/2 - 1;
	int y = g_pFrm->GetHeight() - 2;

	m_ptRotate = Square(x, y);
	m_block.reserve(4);	//����vector�ռ�
	m_block.push_back(Square(x, y-1));
	m_block.push_back(Square(x-1, y-1));
	m_block.push_back(Square(x-1, y));
	m_block.push_back(Square(x-1, y+1));
}

/*	��Z������	*/
CObverseZ::CObverseZ()
{
	int x = g_pFrm->GetWidth()/2 - 1;
	int y = g_pFrm->GetHeight() - 1;

	m_ptRotate = Square(x, y);
	m_block.reserve(4);	//����vector�ռ�

	m_block.push_back(Square(x, y));
	m_block.push_back(Square(x-1, y));
	m_block.push_back(Square(x, y-1));
	m_block.push_back(Square(x+1, y-1));
}

/*	��Z������	*/
COpposeZ::COpposeZ()
{
	int x = g_pFrm->GetWidth()/2 - 1;
	int y = g_pFrm->GetHeight() - 1;

	m_ptRotate = Square(x, y);
	m_block.reserve(4);	//����vector�ռ�

	m_block.push_back(Square(x, y));
	m_block.push_back(Square(x+1, y));
	m_block.push_back(Square(x, y-1));
	m_block.push_back(Square(x-1, y-1));
}

/*	����������	*/
CBar::CBar()
{
	int x = g_pFrm->GetWidth()/2 - 1;
	int y = g_pFrm->GetHeight() - 3;

	m_ptRotate = Square(x, y);
	m_block.reserve(4);	//����vector�ռ�

	m_block.push_back(Square(x, y));
	m_block.push_back(Square(x, y-1));
	m_block.push_back(Square(x, y+1));
	m_block.push_back(Square(x, y+2));
}

/*	���ַ�����	*/
CTian::CTian()
{
	int x = g_pFrm->GetWidth()/2;
	int y = g_pFrm->GetHeight() - 1;

	m_ptRotate = Square(x, y);
	m_block.reserve(4);	//����vector�ռ�

	m_block.push_back(Square(x, y));
	m_block.push_back(Square(x-1, y));
	m_block.push_back(Square(x-1, y-1));
	m_block.push_back(Square(x, y-1));
}

/*	T�ַ�����	*/
CT::CT()
{
	int x = g_pFrm->GetWidth()/2 - 1;
	int y = g_pFrm->GetHeight() - 1;

	m_ptRotate = Square(x, y);
	m_block.reserve(4);	//����vector�ռ�

	m_block.push_back(Square(x, y));
	m_block.push_back(Square(x-1, y));
	m_block.push_back(Square(x+1, y));
	m_block.push_back(Square(x, y-1));
}

//��д�����������ת����
void CBar::Rotate()
{
	Square_vec tmpvec;
	tmpvec.reserve(m_block.size());

	//������������ɺ�ģ���֮��Ȼ
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

	//�ж���ת��ķ����Ƿ�ᷢ��Խ��
	if (GetBlockBound(LEFT) < 0 || GetBlockBound(RIGHT) > g_pFrm->GetWidth()-1 ||
		GetBlockBound(DOWN) < 0)
	{
		m_block.swap(tmpvec);
		return;
	}

	//�ж���ת��ķ����Ƿ���frame�ڵķ��鷢���ص�
	if (IsCrashed(NO))
	{
		m_block.swap(tmpvec);
		return;
	}
}

//��д���ַ������ת����
void CTian::Rotate()
{
	return;
}