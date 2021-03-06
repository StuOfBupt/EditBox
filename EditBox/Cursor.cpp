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
判断窗口坐标x前的字符是否为英文类型
*/
bool Cursor::isEnBefore(int LineNumber, int x)
{
	if (CharactersProperty_before_Cursor(LineNumber, x) == -1)
		return true;
	else
		return false;
}

/*
判断窗口坐标x后的字符是否为英文类型
*/
bool Cursor::isEnAfter(int LineNumber, int x)
{
	if (!isLegalCursor(LineNumber, x))	//检查光标合法性
		throw std::invalid_argument("invalid Cursor 'x'");
	int Size = Characters_before_Cursor(LineNumber, x);
	if (Size == pText->Line_Size(LineNumber))
		throw std::invalid_argument("光标x后无字符");
	CLine* p = pText->GetLinePointer(LineNumber);
	Line_iterator iterator(*p, Size + 1);
	short int m = *iterator;
	if (WORD(m >> 8) > 0)
		return false;
	else
		return true;
}
//光标是否夹在汉字中间
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

/*返回光标前的字符数量*/
int Cursor::Characters_before_Cursor(int LineNumber, int x)
{
	if (!isLegalCursor(LineNumber, x))						//检查光标合法性
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
返回光标前的字符属性(空、中、英)
0---光标处于行首 无字符
1---中文字符
-1--英文字符
*/
int Cursor::CharactersProperty_before_Cursor(int LineNumber, int x)
{
	if (!isLegalCursor(LineNumber, x))		//检查光标合法性
		throw std::invalid_argument("invalid Cursor 'x'");
	if (x == 0)
		return 0;
	int Size = Characters_before_Cursor(LineNumber, x);		//光标前字符数量
	CLine* p = pText->GetLinePointer(LineNumber);
	Line_iterator iterator(*p, Size);
	int m = *iterator;
	if (WORD(m >> 8) > 0)
		return 1;
	else
		return -1;
}

/*
鼠标点击时对光标重定位
使之返回一个合法的光标位置
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
返回光标前字符的位置
eg. 3 abcde|fd
return (3,5)
*/
Position Cursor::CursorToPosition(int x, int y)
{
	int LineNumber = y / nHeight + 1;
	//光标合法性检测
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
