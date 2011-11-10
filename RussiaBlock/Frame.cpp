#include "Frame.h"

//默认构造宽10 高20的框架
CFrame::CFrame():width(10), height(20), level(0), slashTime(0)
{
	container.reserve(10);
	for (int i=0; i!=10; ++i)
	{
		container.push_back(vector<bool>(20, false));
	}
}

//构造宽为w 高为h的框架
CFrame::CFrame(const unsigned int w, const unsigned int h)
	:width(w), height(h), level(0), slashTime(0)
{
	container.reserve(width);
	for (int i=0; i!=width; ++i)
	{
		container.push_back(vector<bool>(height, false));
	}
}


//设置框架高度
void CFrame::SetHeight(const unsigned int he)
{
	height = he;
}

//设置框架宽度
void CFrame::SetWidth(const unsigned int wd)
{
	width = wd;
}

//获得框架高度
inline int CFrame::GetHeight() const
{
	return height;
}

//获得框架宽度
inline int CFrame::GetWidth() const
{
	return width;
}

//获取容器中已被填充的纵坐标最高的一个方块
inline Square CFrame::GetTopSquare() const
{
	return topSquare;
}

//将container[x][y]状态设为true
void CFrame::SetFilled(const int x, const int y)
{
	if (!container[x][y])
	{
		container[x][y] = true;
		if (y > topSquare.y)
			topSquare = Square(x, y);
	}
}

//检查容器中是否有可以消除的行，如有则调用SlashLine函数消行
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

//消除第lineNum行
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

//消行后上面的方块落下
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
	Check();	//下落后需要再次检查是否有可以消除的行
}

//初始化
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

//游戏是否结束
inline bool CFrame::IsOver()
{
	return topSquare.y >= height-1;
}

//升级
bool CFrame::LevelUp()
{
	//level up!
	//最高等级为9级
	if (level == 9)
	{
		return false;
	}

	//消10次升一级
	if (slashTime / 10 > 0)
	{
		++level;
		slashTime = 0;
		return true;
	}
	return false;
}

//获取等级
inline const int CFrame::GetLevel()
{
	return level;
}

//获取消行次数
inline const int CFrame::GetSlashTime()
{
	return slashTime;
}