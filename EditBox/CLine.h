#pragma once
#ifndef UNICODE
#define UNICODE
#endif // !UNICODE
#include<tchar.h>
#include<string>
#include<iostream>
#define BLOCK_SIZE	5
struct Position
{
	int LineNumber;		//所在行号
	int Sequence;		//所在序号
};
//每次分配的数据块
class Line_iterator;			//行迭代器
struct DataBlock
{
	DataBlock():pNextBlock(NULL){ }
	DataBlock* pNextBlock;
	TCHAR	   Strings[BLOCK_SIZE];
};
class CLine
{
	friend class Line_iterator;
	friend class CText;
public:
	CLine(int LNum, CLine* pNext = NULL);
	CLine(CLine& L);
	CLine(CLine&& L);
	~CLine();
	void CreateLine(std::wstring& String);							//创建行
	void ClearLine();												//清空行
	void ShowLineData();											//显示当前行字符串
	Line_iterator DeleteLine(int first, int last);					//删除当前行[first,end]内的字符串		字符从1开始返回删除后的第一位
	bool BackSpace(Position position);								//退格键  删除Position 位置上的字符  
	Line_iterator InsertStrings(int start,std::wstring String);		//在start "后面" 加入字符串
//	void WriteLine();												//将本行写入文本文件
	Line_iterator begin();											//返回指向行首字符的迭代器
	Line_iterator end();											//返回指向行尾字符的迭代器
	std::wstring  TransformToWString(int first, int last);			//以wstringf的形式返回[first,last]中的字符串
	size_t size()const;												//返回本行字符串数量
	CLine& operator=(CLine Line);
	void SetLineNumber(int Number);									//设置/更改行号
private:
	CLine*		pNextLine;											//下一行
	DataBlock*	pLineHead;											//行首
	int			nLineNumber;										//当前行号
	int			nDataSize;											//当前行字符数量
	int			nBlocks;											//堆块个数
	bool		bBlankLine;											//是否为空行
	void	DeleteSpareBlocks(DataBlock* p);
	void    swap(CLine& Line, bool SWAP_Next_Line_DATA = false);
};

class Line_iterator
{
	friend class CLine;
public:
	Line_iterator(CLine& theLine, int index = 1);
	Line_iterator(const Line_iterator& m);
	Line_iterator& operator=(const Line_iterator& m);
	~Line_iterator();
	bool operator==(const Line_iterator& m);
	bool operator!=(const Line_iterator& m);
//operators
	Line_iterator& operator++();
	Line_iterator operator++(int);
	Line_iterator& operator--();
	Line_iterator operator--(int);
	Line_iterator& operator+(int n);
	Line_iterator& operator-(int n);
	TCHAR&	operator*();
private:
	CLine *		pLine;			//当前行指针			
	TCHAR *		pWChar;			//指向CLine中的一个字符
	DataBlock*	pBlock;			//当前指向字符所在数据块
	int			nIndex;			//当前指向的字符在行中的位置(注：不是下标，从1开始)	
};
//得到两迭代器[first,last]之间的字符数
size_t operator-(Line_iterator last, Line_iterator first);		
//同一行中，将[first,last]的字符从start开始拷贝到
Line_iterator copy(Line_iterator start, Line_iterator first, Line_iterator	last);