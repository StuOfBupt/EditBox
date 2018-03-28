#include "Cursor.h"
#include<Windows.h>
int Install::Width = 10;
int Install::Height = 10;
Cursor::Cursor(CText * p, int width, int height)
{
	pText = p;
	nWidth = width;
	nHeight = height;
}

void Cursor::SetWidth(int width)
{
	nWidth = width;
}

void Cursor::SetHeight(int height)
{
	nHeight = height;
}
/*
�жϴ�������xǰ���ַ��Ƿ�ΪӢ������
*/
bool Cursor::isEnBefore(int LineNumber, int x)
{
	if (CharactersProperty_before_Cursor(LineNumber, x) == -1)
		return true;
	else
		return false;
}

/*
�жϴ�������x����ַ��Ƿ�ΪӢ������
*/
bool Cursor::isEnAfter(int LineNumber, int x)
{
	if (!isLegalCursor(LineNumber, x))	//�����Ϸ���
		throw std::invalid_argument("invalid Cursor 'x'");
	int Size = Characters_before_Cursor(LineNumber, x);
	if (Size == pText->Line_Size(LineNumber))
		throw std::invalid_argument("���x�����ַ�");
	CLine* p = pText->GetLinePointer(LineNumber);
	Line_iterator iterator(*p, Size + 1);
	short int m = *iterator;
	if (WORD(m >> 8) > 0)
		return false;
	else
		return true;
}
//����Ƿ���ں����м�
bool Cursor::isLegalCursor(int LineNumber, int x)
{
	int Sum = 0;
	CLine* p = pText->GetLinePointer(LineNumber);
	if (x > p->Line_Width(nWidth))
		return false;
	Line_iterator iterator(*p);
	while (Sum < x)
	{
		short int m = *iterator;
		++iterator;
		if (WORD(m >> 8) > 0)
			Sum += nWidth;
		Sum += nWidth;
	}
	if (Sum == x)
		return true;
	else
		return false;
}

/*���ع��ǰ���ַ�����*/
int Cursor::Characters_before_Cursor(int LineNumber, int x)
{
	if (!isLegalCursor(LineNumber, x))						//�����Ϸ���
		throw std::invalid_argument("invalid Cursor 'x'");
	CLine* p = pText->GetLinePointer(LineNumber);
	int Width = 0;
	int n = 0;
	Line_iterator iterator(*p);
	while (Width < x)
	{
		short int m = *iterator;
		if (WORD(m >> 8) > 0)
			Width += nWidth * 2;
		else
			Width += nWidth;
		++iterator;
		++n;
	}
	if (Width != x)
		throw std::invalid_argument("error coordinate 'x'");
	return n;
}

/*
���ع��ǰ���ַ�����(�ա��С�Ӣ)
0---��괦������ ���ַ�
1---�����ַ�
-1--Ӣ���ַ�
*/
int Cursor::CharactersProperty_before_Cursor(int LineNumber, int x)
{
	if (!isLegalCursor(LineNumber, x))		//�����Ϸ���
		throw std::invalid_argument("invalid Cursor 'x'");
	if (x == 0)
		return 0;
	int Size = Characters_before_Cursor(LineNumber, x);		//���ǰ�ַ�����
	CLine* p = pText->GetLinePointer(LineNumber);
	Line_iterator iterator(*p, Size);
	int m = *iterator;
	if (WORD(m >> 8) > 0)
		return 1;
	else
		return -1;
}

/*
�����ʱ�Թ���ض�λ
ʹ֮����һ���Ϸ��Ĺ��λ��
*/
int Cursor::CursorLocation(int LineNumber, int x)
{
	CLine* pLine = pText->GetLinePointer(LineNumber);
	int Length =pText-> Line_Width(LineNumber, nWidth);
	if (x > Length)
		return Length;
	else
	{
		while (true)
		{
			if (isLegalCursor(LineNumber, x))
				return x;
			else
				x--;
		}
	}
	
}
/*
���ع��ǰ�ַ���λ��
eg. 3 abcde|fd
return (3,5)
*/
Position Cursor::CursorToPosition(int x, int y)
{
	int LineNumber = y / nHeight + 1;
	//���Ϸ��Լ��
	if (!isLegalCursor(LineNumber, x))
		throw std::invalid_argument("invalid (x,y)");
	int n = Characters_before_Cursor(LineNumber, x);
	return { LineNumber,n };
}

void Install::SetWidth(int width)
{
	Install::Width = width;
}

void Install::SetHeight(int height)
{
	Install::Height = height;
}
