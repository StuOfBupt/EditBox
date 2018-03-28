#pragma once
#include"CText.h"
//������  ����ı������Ľ���������
class Cursor
{
public:
	Cursor(CText* p,int width,int height);
	void SetWidth(int width);											//���ÿ��
	void SetHeight(int height);											//���ø߶�
	bool isEnBefore(int LineNumber, int x);								//���ǰ�Ƿ�ΪӢ��
	bool isEnAfter(int LineNumber, int x);								//�����Ƿ�ΪӢ��
	bool isLegalCursor(int LineNumber, int x);							//�ж�x���Ĺ������ʾ�����Ƿ�Ϸ�
	int Characters_before_Cursor(int LineNumber, int x);				//���ع��ǰ���ַ�����
	int CharactersProperty_before_Cursor(int LineNumber, int x);		//���ع��ǰ�ַ�����
	int CursorLocation(int LineNumber, int x);							//�ض�λ���x(ʹ֮�Ϸ�)
	Position CursorToPosition(int x, int y);							//���Ϸ������λ��ǰ���ַ�λ��ת�����ı�λ��
private:
	CText * pText;					//���ı�ָ��
	int		nWidth;					//�󶨵�λ�п�
	int		nHeight;				//���ַ��߶�
};

//������Ϣ��
struct Install
{			
	void SetWidth(int width);
	void SetHeight(int height);
	static int Width;						//�ַ����
	static int Height;						//�и�
};
