#pragma once
#ifndef UNICODE
#define UNICODE
#endif // !UNICODE
#include<sstream>
#include<fstream>
#include"CLine.h"
//�ı��洢�ṹ
class CText
{
public:
	CText();
	~CText();
	void CreateText(std::string filename);
	void ClearAll();
	void ShowText()const;
	void DeleteLines(int first, int last);					//ɾ������
	void Delete(Position first, Position last);				//ɾ��ѡ�в���
	void BackSpace(Position position);						//�˸��
private:
	CLine *		pFirstLineHead;		//���׵�ַ
	int			nLineNumbers;		//����
	std::string	FileName;			//�ļ���	
	void UpDataLineNumber(CLine* p,int Start);				//�����к�
};
