#ifndef BLOCK_H_
#define BLOCK_H_

#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>

using namespace std;

/*	小方块	*/
class Square
{
public:
	//小方块在框架内的相对坐标
	int x;
	int y;
public:
	Square():x(0), y(0)
	{
	}
	Square(const int _x, const int _y):x(_x), y(_y)
	{
	}
	Square(const Square& sq)
	{
		if (this == &sq)
			return;
		x = sq.x;
		y = sq.y;
	}
};

class CFrame;
/*	方块类，各种形状方块的基类	*/
class CBlock
{
public:
	typedef vector<Square> Square_vec;
	enum POSITION {UP, DOWN, LEFT, RIGHT, NO};	//方向
	enum OBJECT	{OBL, OPL, OBZ, OPZ, T, TIAN, BAR, NUMBERS};	//方块类型

	static CFrame* GetFramePt(CFrame* pfm);		//获取框架指针
	static CFrame* g_pFrm;					//框架指针

	virtual void MoveLeft(void);		//向左移动
	virtual void MoveRight(void);		//向右移动
	virtual void MoveUp(void);		//向上移动
	virtual bool MoveDown(void);		//向下移动
	virtual void Rotate(void);		//旋转
	virtual int GetBlockBound(const POSITION pos) const;	//获取参数所指方块的边界值
	virtual bool IsCrashed(POSITION pos) const;	//判断向pos方向移动一步是否会在框架内发生碰撞
	virtual const Square_vec& GetBlock(void) const;	//返回m_block
	virtual const Square& GetRotatePt(void) const;	//返回旋转点
	static CBlock* CreateObject();		//随机产生方块

protected:
	/*	每种形状都是由四个小方块构成	*/
	Square_vec m_block;
	Square m_ptRotate;	//旋转点
};

/*	反L方块类	*/
class COpposeL : public CBlock
{
public:
	COpposeL();	//构造函数
};

/*	正L方块类	*/
class CObverseL : public CBlock
{
public:
	CObverseL();
};

/*	正Z方块类	*/
class CObverseZ : public CBlock
{
public:
	CObverseZ();
};

/*	反Z方块类	*/
class COpposeZ : public CBlock
{
public:
	COpposeZ();
};

/*	长条方块类	*/
class CBar : public CBlock
{
public:
	CBar();
	void Rotate(void);	//重写长条方块的旋转方法
};

/*	田字方块类	*/
class CTian : public CBlock
{
public:
	CTian();
	void Rotate(void);	//重写田字方块的旋转方法
};

/*	T字方块类	*/
class CT : public CBlock
{
public:
	CT();
};

#endif