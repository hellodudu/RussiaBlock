#ifndef FRAME_H_
#define FRAME_H_

#include "Block.h"

/*	框架类	*/
class CFrame
{
public:
	
protected:
	int width;	//框架的宽
	int height;	//框架的高
	Square topSquare;	//存储容器中已被填充的纵坐标最高的一个方块
public:
	vector< vector<bool> > container;	//框架内容纳方块的容器
	int level;	//等级
	int slashTime;	//消行的次数
public:
	CFrame();
	CFrame(const unsigned int w, const unsigned int h);

	virtual int GetWidth(void) const;		//获取宽度
	virtual int GetHeight(void) const;		//获取高度
	virtual Square GetTopSquare(void) const;	//获取容器中已被填充的纵坐标最高的一个方块
	virtual void SetWidth(const unsigned int wd);	//设置宽度
	virtual void SetHeight(const unsigned int he);	//设置高度
	virtual void SetFilled(const int x, const int y);	//将container[x][y]状态设为true
	virtual bool Check(void);	//检查容器中是否有可以消除的行
	virtual void SlashLine(const int lineNum);	//消除第lineNum行
	virtual void Fall(const int lineNum);	//消行后上面的方块落下
	virtual void Initial(void);	//初始化
	virtual bool IsOver(void);	//判断游戏是否已经结束
	virtual bool LevelUp(void);	//升级
	virtual const int GetLevel(void);	//获取等级
	virtual const int GetSlashTime(void);	//获取消行次数
};

#endif