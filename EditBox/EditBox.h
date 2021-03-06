#pragma once

#include "resource.h"
#include "CText.h"
#include "API.h"

typedef CText* HTEXT;
HTEXT	_stdcall	CreateText(int iCharWidth, int iCharHeight);
BOOL	_stdcall	DestroyText(HTEXT hText);

typedef ULONGLONG FPARAM;	// _INT64
typedef ULONGLONG SPARAM;	// _INT64
typedef ULONGLONG RVALUE;	// _INT64

#define LODWORD(l) ((UINT)(((ULONGLONG)(l)) & 0xffffffff))			// 低双字节
#define HIDWORD(l) ((UINT)(((ULONGLONG)(l) >> 32) & 0xffffffff))	// 高双字节

#define UM_UP		0x00000001
#define UM_DOWN		0x00000002
#define UM_LEFT		0x00000004
#define UM_RIGHT	0x00000008
#define UM_RETURN	0x00000010
#define UM_END		0x00000020
#define UM_CHAR		0x00000040
#define UM_TEXT		0x00000080
#define UM_DELETE	0x00000100
#define UM_CURSOR	0x00000200
#define UM_CHOOSE	0x00000400
#define UM_HOME		0x00000800
#define UM_NEW		0x00001000
#define UM_OPEN		0x00002000
#define UM_SAVE		0x00004000
#define UM_CLOSE	0x00008000
#define UM_ALL		0x00000011
#define UM_COPY		0x00000012
#define UM_PASTE	0x00000014
#define UM_CANCEL	0x00000018
#define UM_FIND		0x00000021

// #define UR_EMPTY	0x00000000
#define UR_ZH		0xf0000000f0000000
#define UR_EN		0xf0000000f0000001
#define UR_FRONT	0x0000000000000000
#define UR_BACK		0x0000000000000001
#define UR_LINETAIL	0xf0000000f0001000
#define UR_LINEHEAD	0xf0000000f0010000
#define UR_NOPATH	0xf0000000f0100000
#define UR_NOTSAVED	0xf0000000f1000000
#define UR_ERROR	0xf0000001f0000000

#define INRANGE(x, l, r) ((x) >= (l) && (x) << (r))

//
//  函数: UserMessageProc(int, int, UINT, WPARAM, LPARAM)
//
//  目的:    处理文本调整的消息，该消息由主窗口程序转发而来

//	参数:	
//	case UP/DOWN/LEFT/RIGHT/RETURN/END/CHAR/DELETE:	
//					x 光标绝对横坐标 y 光标绝对纵坐标 
//					message 消息类别 
//					wParam 高字节是字符高度 低字节是字符宽度
//					lParam NULL

//	case TEXT:
//					x 待输出行文本绝对横坐标 y 待输出行文本绝对纵坐标
//					message 消息类别
//					wParam 高字节是字符高度 低字节是字符宽度
//					lParam 待输出行文本字符数目
//
//	case CHOOSE:
//
//
//	返回值:
//	case UP/DOWN/END:
//					移动后光标的绝对横坐标
//
//	case LEFT/RIGHT:
//					若当前光标位置有字符 返回字符属性 UR_ZH 中文 UR_EN 英文
//					若当前光标位置无字符 返回UR_INVALID
//
//	case RETURN/CHAR:
//					异常返回UR_INVALID 否则 UR_VALID
//
//	case DELETE:
//					若光标位置有字符 返回UR_VALID 
//					否则返回删除行尾换行符后把下一行提上来而增加的画面宽度(单位为像素)
//
//	case TEXT:
//					一个指向待输出行文本的指针
//
//	UM_UP		光标上移
//	UM_DOWN		光标下移
//	UM_LEFT		光标左移
//	UM_RIGHT	光标右移
//	UM_RETURN	回车换行
//	UM_END		END行尾
//	UM_CHAR		插入字符
//	UM_TEXT		显示文本
//	UM_DELETE	删除字符
//	UM_CHOOSE	选中块
//	UM_CURSOR	光标定位
//
RVALUE _stdcall UserMessageProc(HTEXT hText, int x, int y, UINT message, FPARAM fParam, SPARAM sParam);