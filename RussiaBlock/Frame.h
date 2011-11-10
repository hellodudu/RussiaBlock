#ifndef FRAME_H_
#define FRAME_H_

#include "Block.h"

/*	�����	*/
class CFrame
{
public:
	
protected:
	int width;	//��ܵĿ�
	int height;	//��ܵĸ�
	Square topSquare;	//�洢�������ѱ�������������ߵ�һ������
public:
	vector< vector<bool> > container;	//��������ɷ��������
	int level;	//�ȼ�
	int slashTime;	//���еĴ���
public:
	CFrame();
	CFrame(const unsigned int w, const unsigned int h);

	virtual int GetWidth(void) const;		//��ȡ���
	virtual int GetHeight(void) const;		//��ȡ�߶�
	virtual Square GetTopSquare(void) const;	//��ȡ�������ѱ�������������ߵ�һ������
	virtual void SetWidth(const unsigned int wd);	//���ÿ��
	virtual void SetHeight(const unsigned int he);	//���ø߶�
	virtual void SetFilled(const int x, const int y);	//��container[x][y]״̬��Ϊtrue
	virtual bool Check(void);	//����������Ƿ��п�����������
	virtual void SlashLine(const int lineNum);	//������lineNum��
	virtual void Fall(const int lineNum);	//���к�����ķ�������
	virtual void Initial(void);	//��ʼ��
	virtual bool IsOver(void);	//�ж���Ϸ�Ƿ��Ѿ�����
	virtual bool LevelUp(void);	//����
	virtual const int GetLevel(void);	//��ȡ�ȼ�
	virtual const int GetSlashTime(void);	//��ȡ���д���
};

#endif