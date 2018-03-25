#pragma once
#include"CText.h"
#include"Cursor.h"
#include<Windows.h>
#define UM_UP		0x00000001		//	UM_UP		�������
#define UM_DOWN		0x00000002		//	UM_DOWN		�������
#define UM_LEFT		0x00000004		//	UM_LEFT		�������
#define UM_RIGHT	0x00000008		//	UM_RIGHT	�������
#define UM_RETURN	0x00000010		//	UM_RETURN	�س�����
#define UM_END		0x00000020		//	UM_END		END��β
#define UM_CHAR		0x00000040		//	UM_CHAR		�����ַ�
#define UM_TEXT		0x00000080		//	UM_TEXT		��ʾ�ı�
#define UM_DELETE	0x00000100		//	UM_DELETE	ɾ���ַ�
#define UM_CURSOR	0x00000012		//	���λ��
#define UM_HOME	    0X00000014

#define UR_ZH		0x00000200		
#define UR_EN		0x00000400
#define UR_INVALID	0xFFFFFFFF
#define UR_VALID	0x00000000
#define UR_LINEHEAD 0x00000003
#define UR_SUCCESS	0x00000005
#define UR_ERROR	0x00000009
#define UR_LINETALL	0x00000007
//����ֵ
//	UP/DOWN/END:
//					�ƶ�����ľ��Ժ�����
//
//	case LEFT/RIGHT:
//					����ǰ���λ�����ַ� �����ַ����� UR_ZH ���� UR_EN Ӣ��
//					����ǰ���λ�����ַ� ����UR_INVALID
//
//	case RETURN/CHAR:
//					�쳣����UR_INVALID ���� UR_VALID
//
//	case DELETE:
//					�����λ�����ַ� ����UR_VALID 
//					���򷵻�ɾ����β���з������һ�������������ӵĻ�����(��λΪ����)
//
//	case TEXT:
//					һ��ָ���������ı���ָ��
LRESULT WINAPI UserMessageProc(int x, int y, UINT message, WPARAM wParam, LPARAM lParam);
Cursor*  Initialize_Cursor(CText* p,int Width,int Height);
void Free_Cursor(Cursor*& p);
CText* pText = NULL;
Cursor* pCursor = NULL;