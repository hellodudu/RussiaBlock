#include "Frame.h"

//Ĭ�Ϲ����10 ��20�Ŀ��
CFrame::CFrame():width(10), height(20), level(0), slashTime(0)
{
	container.reserve(10);
	for (int i=0; i!=10; ++i)
	{
		container.push_back(vector<bool>(20, false));
	}
}

//�����Ϊw ��Ϊh�Ŀ��
CFrame::CFrame(const unsigned int w, const unsigned int h)
	:width(w), height(h), level(0), slashTime(0)
{
	container.reserve(width);
	for (int i=0; i!=width; ++i)
	{
		container.push_back(vector<bool>(height, false));
	}
}


//���ÿ�ܸ߶�
void CFrame::SetHeight(const unsigned int he)
{
	height = he;
}

//���ÿ�ܿ��
void CFrame::SetWidth(const unsigned int wd)
{
	width = wd;
}

//��ÿ�ܸ߶�
inline int CFrame::GetHeight() const
{
	return height;
}

//��ÿ�ܿ��
inline int CFrame::GetWidth() const
{
	return width;
}

//��ȡ�������ѱ�������������ߵ�һ������
inline Square CFrame::GetTopSquare() const
{
	return topSquare;
}

//��container[x][y]״̬��Ϊtrue
void CFrame::SetFilled(const int x, const int y)
{
	if (!container[x][y])
	{
		container[x][y] = true;
		if (y > topSquare.y)
			topSquare = Square(x, y);
	}
}

//����������Ƿ��п����������У����������SlashLine��������
bool CFrame::Check()
{
	bool flag = false;
	for (int y=0; y<=topSquare.y; ++y)
	{
		for (int x=0; x!=width; ++x)
		{
			if (!container[x][y])
			{
				flag = false;
				break;
			}
			else
			{
				flag = true;
			}
		}
		if (flag)
		{
			SlashLine(y);
			return true;
		}
	}
	return false;
}

//������lineNum��
void CFrame::SlashLine(const int lineNum)
{
	for (int x=0; x!=width; ++x)
	{
		container[x][lineNum] = false;
	}
	Fall(lineNum);
	--topSquare.y;
	++slashTime;
	LevelUp();
}

//���к�����ķ�������
void CFrame::Fall(const int lineNum)
{
	for (int x=0; x!=width; ++x)
	{
		int y = topSquare.y;
		for (; y!=lineNum && container[x][y]!=true; --y)
		{
		}
		if (y == lineNum)
		{
			continue;
		}
		else
		{
			for (int j=lineNum; j<=y; ++j)
			{
				container[x][j] = container[x][j+1];
			}
		}
	}
	Check();	//�������Ҫ�ٴμ���Ƿ��п�����������
}

//��ʼ��
void CFrame::Initial()
{
	for (int x=0; x!=container.size(); ++x)
	{
		for (int y=0; y!=container[x].size(); ++y)
		{
			container[x][y] = false;
		}
	}
	topSquare = Square(0, 0);
	level = 0;
	slashTime = 0;

	srand((unsigned)time(NULL));
}

//��Ϸ�Ƿ����
inline bool CFrame::IsOver()
{
	return topSquare.y >= height-1;
}

//����
bool CFrame::LevelUp()
{
	//level up!
	//��ߵȼ�Ϊ9��
	if (level == 9)
	{
		return false;
	}

	//��10����һ��
	if (slashTime / 10 > 0)
	{
		++level;
		slashTime = 0;
		return true;
	}
	return false;
}

//��ȡ�ȼ�
inline const int CFrame::GetLevel()
{
	return level;
}

//��ȡ���д���
inline const int CFrame::GetSlashTime()
{
	return slashTime;
}