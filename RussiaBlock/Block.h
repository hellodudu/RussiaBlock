#ifndef BLOCK_H_
#define BLOCK_H_

#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>

using namespace std;

/*	С����	*/
class Square
{
public:
	//С�����ڿ���ڵ��������
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
/*	�����࣬������״����Ļ���	*/
class CBlock
{
public:
	typedef vector<Square> Square_vec;
	enum POSITION {UP, DOWN, LEFT, RIGHT, NO};	//����
	enum OBJECT	{OBL, OPL, OBZ, OPZ, T, TIAN, BAR, NUMBERS};	//��������

	static CFrame* GetFramePt(CFrame* pfm);		//��ȡ���ָ��
	static CFrame* g_pFrm;					//���ָ��

	virtual void MoveLeft(void);		//�����ƶ�
	virtual void MoveRight(void);		//�����ƶ�
	virtual void MoveUp(void);		//�����ƶ�
	virtual bool MoveDown(void);		//�����ƶ�
	virtual void Rotate(void);		//��ת
	virtual int GetBlockBound(const POSITION pos) const;	//��ȡ������ָ����ı߽�ֵ
	virtual bool IsCrashed(POSITION pos) const;	//�ж���pos�����ƶ�һ���Ƿ���ڿ���ڷ�����ײ
	virtual const Square_vec& GetBlock(void) const;	//����m_block
	virtual const Square& GetRotatePt(void) const;	//������ת��
	static CBlock* CreateObject();		//�����������

protected:
	/*	ÿ����״�������ĸ�С���鹹��	*/
	Square_vec m_block;
	Square m_ptRotate;	//��ת��
};

/*	��L������	*/
class COpposeL : public CBlock
{
public:
	COpposeL();	//���캯��
};

/*	��L������	*/
class CObverseL : public CBlock
{
public:
	CObverseL();
};

/*	��Z������	*/
class CObverseZ : public CBlock
{
public:
	CObverseZ();
};

/*	��Z������	*/
class COpposeZ : public CBlock
{
public:
	COpposeZ();
};

/*	����������	*/
class CBar : public CBlock
{
public:
	CBar();
	void Rotate(void);	//��д�����������ת����
};

/*	���ַ�����	*/
class CTian : public CBlock
{
public:
	CTian();
	void Rotate(void);	//��д���ַ������ת����
};

/*	T�ַ�����	*/
class CT : public CBlock
{
public:
	CT();
};

#endif