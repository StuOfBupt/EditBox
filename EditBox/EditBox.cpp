//EditBox.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "EditBox.h"
#include <assert.h>

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EDITBOX, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EDITBOX));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EDITBOX));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EDITBOX);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle,
	   WS_CAPTION |
	   WS_SYSMENU |
	   WS_MAXIMIZE |
	   WS_HSCROLL |
	   WS_CAPTION |
	   WS_MINIMIZEBOX |
	   WS_VSCROLL,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_SHOW);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 字符相关信息
	static UINT cxChar, cyChar;			// 字符宽高(像素)
	static HFONT hFont;							// 字体
	static TEXTMETRIC tm;							// 字符信息

	// 显示区相关信息
	static int cxClient, cyClient;					// 显示区宽高(像素)
	static int cxPageCol, cyPageRow;				// 页面行列数
	static int cxWindowPos, cyWindowPos;
	static int cyCurRow;

	// 光标相关信息
	static int cxCaretWidth, cyCaretHeight;			// 光标大小
	static int cxCaretPos, cyCaretPos;	// 光标位置(像素)

	static POINT pStart, pEnd;

    switch (message)
    {
	case WM_CREATE:
		{
			HDC hdc = GetDC(hWnd);

			// 初始化字体相关信息
			hFont = CreateFont(
				16,		// 高度
				0,		// 宽度
				0,		// 水平
				0,		// 倾斜
				400,	// 粗度
				0,		// 不倾斜
				0,		// 无下划线
				0,		// 无中划线
				GB2312_CHARSET,					// 字符集
				OUT_DEFAULT_PRECIS,				// 输出精度
				CLIP_DEFAULT_PRECIS,			// 裁剪精度
				DEFAULT_QUALITY,				// 输出质量
				DEFAULT_PITCH | FF_DONTCARE,	// 间距
				TEXT("New Curier")				// 名称
			);
			// ...
			SelectObject(hdc, hFont);

			GetTextMetrics(hdc, &tm);
			cxChar = tm.tmAveCharWidth;
			cyChar = tm.tmHeight;
		}
		break;
	case WM_SHOWWINDOW:
		{
			RECT rc;
			GetClientRect(hWnd, &rc); 
			cxClient = rc.right - rc.left;
			cyClient = rc.bottom - rc.top;

			SCROLLINFO sInfo;
			sInfo.cbSize = sizeof(SCROLLINFO);
			sInfo.fMask = SIF_ALL;
			sInfo.nMax = 0;
			sInfo.nMin = 0;
			sInfo.nPage = cxClient / (cxChar << 1);
			sInfo.nPos = 0;
			sInfo.nTrackPos = 0;
			SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
			ShowScrollBar(hWnd, SB_HORZ, TRUE);

			sInfo.cbSize = sizeof(SCROLLINFO);
			sInfo.fMask = SIF_ALL;
			sInfo.nMax = 0;
			sInfo.nMin = 0;
			sInfo.nPage = cyClient / cyChar;
			sInfo.nPos = 0;
			sInfo.nTrackPos = 0;
			SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
			ShowScrollBar(hWnd, SB_VERT, TRUE);

			cxCaretWidth = 1;
			cyCaretHeight = cyChar;
			cxCaretPos = 0;
			cyCaretPos = 0;
			CreateCaret(hWnd, NULL, cxCaretWidth, cyCaretHeight);
			SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyCaretPos);
			ShowCaret(hWnd);

			cxWindowPos = 0;
			cyWindowPos = 0;
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_SIZE:
		{

		}
		break;
	case WM_SETFOCUS:
		{
			CreateCaret(hWnd, NULL, cxCaretWidth, cyCaretHeight);
			SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
			ShowCaret(hWnd);
		}
		break;
	case WM_KILLFOCUS:
		{
			DestroyCaret();
		}
		break;
	case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_HOME:
					{
						cxWindowPos = 0;
						cxCaretPos = 0;
						SendMessage(hWnd, WM_HSCROLL, SB_LEFT, NULL);
					}
					break;
				case VK_END:
					{
						if (cxCaretPos 
							= UserMessageProc(cxCaretPos, cyCaretPos, UM_END, (cyChar << 16 | cxChar), NULL) != UR_INVALID)
						{
							SCROLLINFO sInfo;
							sInfo.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
							GetScrollInfo(hWnd, SB_HORZ, &sInfo);
							if (cxCaretPos > cxWindowPos + sInfo.nPage * (cxChar << 1))
							{	// 向右跳页
								int nOffset 
									= min(cxCaretPos - cxWindowPos - (sInfo.nPage >> 1) * (cxChar << 1),
										sInfo.nMax * (cxChar << 1) - cxWindowPos - sInfo.nPage * (cxChar << 1));
								cxWindowPos = cxWindowPos + nOffset;
								sInfo.nPos = sInfo.nPos + (nOffset / (cxChar << 1));
								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);
							}
							else
							{	// 未水平跳页

							}
						}
						else
						{	// 异常
							assert(!(cxCaretPos == UR_INVALID));
						}
					}
					break;
				case VK_UP:
					{
						if ((cxCaretPos 
							= UserMessageProc(cxCaretPos, cyCaretPos, UM_UP, (cyChar << 16 | cxChar), NULL)) != UR_INVALID)
						{
							if (cxCaretPos < cxWindowPos)
							{	// 向左跳页
								SCROLLINFO sInfo;
								sInfo.fMask = SIF_POS | SIF_RANGE;
								GetScrollInfo(hWnd, SB_HORZ, &sInfo);

								int nOffset = min(cxWindowPos, (cxWindowPos - cxCaretPos) + (sInfo.nPage >> 1) * (cxChar << 1));
								cxWindowPos = cxWindowPos - nOffset;
								sInfo.nPos = sInfo.nPos - (nOffset / (cxChar << 1));
								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);
							}
							else
							{	// 未水平跳页
							}
							if (cyCaretPos == cyWindowPos)
							{
								SendMessage(hWnd, WM_HSCROLL, SB_LINEUP, NULL);
							}
							cyCaretPos = cyCaretPos - cyChar;
						}
						else
						{	// 光标处于顶层
							// assert(!(cx == UM_INVALID));
						}
					}
					break;
				case VK_DOWN:
					{
						if ((cxCaretPos 
							= UserMessageProc(cxCaretPos, cyCaretPos, UM_DOWN, (cyChar << 16 | cxChar), NULL)) != UR_INVALID)
						{
							SCROLLINFO sInfo;

							if (cxCaretPos < cxWindowPos)
							{	// 向左跳页
								sInfo.fMask = SIF_POS | SIF_PAGE;
								GetScrollInfo(hWnd, SB_HORZ, &sInfo);

								int nOffset = min(cxWindowPos, (cxWindowPos - cxCaretPos) + (sInfo.nPage >> 1) * (cxChar << 1));
								cxWindowPos = cxWindowPos - nOffset;
								sInfo.nPos = sInfo.nPos - (nOffset / (cxChar << 1));
								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);
							}
							else
							{	// 未水平跳页
							}

							sInfo.fMask = SIF_RANGE;
							GetScrollInfo(hWnd, SB_VERT, &sInfo);
							if (cyCaretPos == cyWindowPos + sInfo.nPage * cyChar)
							{
								SendMessage(hWnd, WM_HSCROLL, SB_LINEDOWN, NULL);
							}
							cyCaretPos = cyCaretPos + cyChar;
						}
						else
						{	// 光标处于底层
							// assert(!(cx == UM_INVALID));
						}
					}
					break;
				case VK_RIGHT:
					{
						BOOL isZh;
						if ((isZh 
							= LOWORD(UserMessageProc(cxCaretPos, cyCaretPos, UM_LEFT, (cyChar << 16 | cxChar), NULL))) != UR_INVALID)
						{	// 同行
							SCROLLINFO sInfo;

							sInfo.fMask = SIF_RANGE;
							GetScrollInfo(hWnd, SB_HORZ, &sInfo);
							if (cxCaretPos + cxChar + isZh > cxWindowPos + (sInfo.nPage * (cxChar << 1)))
							{	// 越界
								sInfo.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
								GetScrollInfo(hWnd, SB_HORZ, &sInfo);

								if (sInfo.nPos == sInfo.nMax)
									sInfo.nMax += 1;
								int nOffset = min(sInfo.nMax * (cxChar << 1) - cxCaretPos, (cxChar << 1) * (sInfo.nPage >> 1));
								cxWindowPos = cxWindowPos + nOffset;
								sInfo.nPos = sInfo.nPos + nOffset / (cxChar << 1);

								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);
							}
							else
							{
								cxCaretPos = cxCaretPos + cxChar + isZh;
							}
						}
						else
						{	// HOME + DOWN
							SendMessage(hWnd, WM_KEYDOWN, VK_HOME, NULL);
							SendMessage(hWnd, WM_KEYDOWN, VK_DOWN, NULL);
						}
					}
					break;
				case VK_LEFT:
					{
						BOOL isZh;
						if ((isZh = 
							LOWORD(UserMessageProc(cxCaretPos, cyCaretPos, UM_RIGHT, (cyChar << 16 | cxChar), NULL))) != UR_INVALID)
						{
							cxCaretPos = cxCaretPos - cxChar - isZh;
							if (cxCaretPos < cxWindowPos)
							{
								SCROLLINFO sInfo;
								sInfo.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
								GetScrollInfo(hWnd, SB_HORZ, &sInfo);

								int nOffset = min(cxCaretPos - cxWindowPos, (sInfo.nPage >> 1) * (cxChar << 1));
								cxWindowPos = cxWindowPos - nOffset;
								sInfo.nPos = sInfo.nPos - (nOffset / (cxChar << 1));

								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);
							}
						}
						else
						{	// 提行
							SendMessage(hWnd, WM_KEYDOWN, VK_UP, NULL);
						}
					}
					break;
				case VK_DELETE:
					{	// 待更改
						LRESULT result;
						int nOffset = 0;
						if ((nOffset 
							= LOWORD(UserMessageProc(cxCaretPos, cyCaretPos, UM_DELETE, (cyChar << 16 | cxChar), NULL))) != UR_VALID)
						{
							if (nOffset != 0)
							{	// 删除换行符
								SCROLLINFO sInfo;
								sInfo.fMask = SIF_RANGE;
								GetScrollInfo(hWnd, SB_HORZ, &sInfo);
								// 调整滚动条范围
								sInfo.nMax =
									max(sInfo.nMax, (cxCaretPos + nOffset) / (cxChar << 1) + (cxCaretPos + nOffset) % (cxChar << 1));
								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);

								GetScrollInfo(hWnd, SB_VERT, &sInfo);
								sInfo.nMax = sInfo.nMax - 1;
								SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_VERT, TRUE);
							}
						}
						else
						{	// 删除后一个字符
						}
					}
					break;
				default:
					break;
			}
		}
		break;
	case WM_CHAR:
		{
			//for (int i = 0; i < (int)LOWORD(lParam); ++i)
			//{
				switch (wParam)
				{
				case '\b':
					{
						cxCaretPos = min(pStart.x, pEnd.x);
						cyCaretPos = min(pStart.y, pEnd.y);
						SendMessage(hWnd, WM_KEYDOWN, VK_LEFT, NULL);
						SendMessage(hWnd, WM_KEYDOWN, VK_DELETE, NULL);
					}
					break;
				case '\t':
					{
						do
						{
							SendMessage(hWnd, WM_CHAR, ' ', NULL);
						} while (cxCaretPos % (cxChar << 4) != 0);
					}
					break;
				case '\n':
					{
					}
					break;
				case '\r':
					{
						if ((UserMessageProc(cxCaretPos, cyCaretPos, UM_RETURN, (cyChar << 16 | cxChar), NULL)) != UR_INVALID)
						{
							SendMessage(hWnd, WM_KEYDOWN, VK_RIGHT, NULL);
						}
					}
					break;
				default:
					{
						if ((UserMessageProc(cxCaretPos, cyCaretPos, UM_CHAR, (cyChar << 16 | cxChar), NULL)) != UR_INVALID)
						{
							SendMessage(hWnd, WM_KEYDOWN, VK_RIGHT, NULL);
						}
					}
					break;
				}
			//}
		}
		break;
	case WM_VSCROLL:
		{
			SCROLLINFO	sInfo;
			sInfo.fMask = SIF_ALL;
			GetScrollInfo(hWnd, SB_VERT, &sInfo);

			int iVertPos = sInfo.nPos;
			switch (LOWORD(wParam))
			{
			case SB_LINEUP:
				{
					sInfo.nPos = min(sInfo.nPos - 1, sInfo.nMin);
				}
				break;
			case SB_LINEDOWN:
				{
					sInfo.nPos = max(sInfo.nPos + 1, sInfo.nMax);
				}
				break;
			case SB_PAGEUP:
				{
					sInfo.nPos = min(sInfo.nPos - sInfo.nPage, sInfo.nMin);
				}
				break;
			case SB_PAGEDOWN:
				{
					sInfo.nPos = max(sInfo.nPos + sInfo.nPage, sInfo.nMax);
				}
				break;
			case SB_TOP:
				{
					sInfo.nPos = sInfo.nMin;
				}
				break;
			case SB_BOTTOM:
				{
					sInfo.nPos = sInfo.nMax;
				}
				break;
			case SB_THUMBTRACK:
				{
					sInfo.nPos = HIWORD(wParam);
				}
			default:
				break;
			}
			cyWindowPos = cyWindowPos + (sInfo.nPos - iVertPos) * cyChar;

			sInfo.fMask = SIF_POS;
			SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
			ShowScrollBar(hWnd, SB_VERT, TRUE);

			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		}
		break;
	case WM_HSCROLL:
		{
			SCROLLINFO sInfo;
			sInfo.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
			GetScrollInfo(hWnd, SB_HORZ, &sInfo);

			int iVertPos = sInfo.nPos;
			switch (LOWORD(wParam))
			{
			case SB_LINELEFT:
				{
					sInfo.nPos = max(sInfo.nPos - 1, sInfo.nMin);
				}
				break;
			case SB_LINERIGHT:
				{
					sInfo.nPos = min(sInfo.nPos + 1, sInfo.nMax);
				}
				break;
			case SB_PAGELEFT:
				{
					sInfo.nPos = max(sInfo.nPos - sInfo.nPage, sInfo.nMin);
				}
				break;
			case SB_PAGERIGHT:
				{
					sInfo.nPos = min(sInfo.nPos + sInfo.nPage, sInfo.nMax);
				}
				break;
			case SB_LEFT:
				{
					sInfo.nPos = sInfo.nMin;
				}
				break;
			case SB_RIGHT:
				{
					sInfo.nPos = sInfo.nMax;
				}
				break;
			case SB_THUMBTRACK:
				{
					sInfo.nPos = HIWORD(wParam);
				}
				break;
			default:
				break;
			}
			cxWindowPos = cxWindowPos + (sInfo.nPos - iVertPos) * (cxChar << 1);

			sInfo.fMask = SIF_POS;
			SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
			ShowScrollBar(hWnd, SB_HORZ, TRUE);

			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			GetCursorPos(&pStart);
			ScreenToClient(hWnd, &pStart);
			if (pStart.x > (cxChar << 1) * cxPageCol ||
				pStart.y > cyChar * cyPageRow)
			{
				return 0;
			}

			pStart.x += cxWindowPos;
			pStart.y += cyWindowPos;
			pEnd = pStart;

			LRESULT result;
			if ((result = UserMessageProc(pStart.x, pStart.y,
				UM_CURSOR, NULL, NULL)) != UR_INVALID)
			{	// 无效区域 滑动到行尾
				cxCaretPos = LOWORD(result);
				cyCaretPos = HIWORD(result);

				SendMessage(hWnd, WM_KEYDOWN, VK_HOME, NULL);
				SendMessage(hWnd, WM_KEYDOWN, VK_END, NULL);
			}
			else
			{
				cxCaretPos = LOWORD(result);
				cyCaretPos = HIWORD(result);
			}

			SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
		}
		break;
	case WM_LBUTTONUP:
		{
			GetCursorPos(&pEnd);
			ScreenToClient(hWnd, &pEnd);
			pEnd.x += cxWindowPos;
			pEnd.y += cyWindowPos;

			LRESULT result;
			result = 
				UserMessageProc(pStart.x, pStart.y, UM_CHOOSE, (cyChar << 16 | cxChar), (pEnd.y << 32 | pEnd.x));

			cxCaretPos = LOWORD(result);
			cyCaretPos = HIWORD(result);
			SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);

			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		}
		break;
	case WM_NCLBUTTONDBLCLK:
		{
			if (HTCAPTION == wParam)
			{

			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
		}
		break;
	case WM_MOUSEMOVE:
		{
			switch (wParam)
			{
			case MK_LBUTTON:
				{
					POINT p;
					GetCursorPos(&p);
					ScreenToClient(hWnd, &p);

					if (p.y > cyChar * cyPageRow)
					{
						SendMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, NULL);
					}
					else if (p.x > (cxChar << 1) * cxPageCol)
					{
						SendMessage(hWnd, WM_HSCROLL, SB_LINERIGHT, NULL);
					}
					else if (p.y < 0)
					{
						SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, NULL);
					}
					else if (p.x < 0)
					{
						SendMessage(hWnd, WM_HSCROLL, SB_LINELEFT, NULL);
					}
					p.x += cxWindowPos;
					p.y += cyWindowPos;

					LRESULT result;
					result = UserMessageProc(pStart.x, pStart.y, UM_CHOOSE, (cyChar << 16 | cxChar), (p.y << 32 | p.x));

					cxCaretPos = LOWORD(result);
					cyCaretPos = HIWORD(result);
					SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);

					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
				}
				break;
			}
		}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
			SelectObject(hdc, hFont);

			RECT rc = ps.rcPaint;
			LPWSTR szBuffer = nullptr;
			ULONG info = 0;
			for (int row = rc.top; row <= rc.bottom; row += cyChar)
			{
				if ((szBuffer = 
					(LPWSTR)UserMessageProc(rc.left + cxWindowPos, row + cyWindowPos,
						UM_TEXT, (cyChar << 16 | cxChar), (LPARAM)&info)) != nullptr)
				{
					int count = LOBYTE(LOWORD(info));
					int flag = HIBYTE(LOWORD(info));
					int offset = HIWORD(info);
					if (flag == UR_FRONT)
					{
						SetTextColor(hdc, RGB(0xFF, 0xFF, 0xFF));
						SetBkColor(hdc, RGB(0x18, 0x74, 0xCD));
						TextOut(hdc, rc.left, row, szBuffer, count);

						SetTextColor(hdc, RGB(0x00, 0x00, 0x00));
						SetBkColor(hdc, RGB(0xFF, 0xFF, 0xFF));
						TextOut(hdc, rc.left + offset, row, szBuffer + count, wcslen(szBuffer) - count);
					}
					else if (flag == UR_BACK)
					{
						SetTextColor(hdc, RGB(0x00, 0x00, 0x00));
						SetBkColor(hdc, RGB(0xFF, 0xFF, 0xFF));
						TextOut(hdc, rc.left, row, szBuffer, wcslen(szBuffer) - count);

						SetTextColor(hdc, RGB(0xFF, 0xFF, 0xFF));
						SetBkColor(hdc, RGB(0x18, 0x74, 0xCD));
						TextOut(hdc, rc.left + offset, row, szBuffer + wcslen(szBuffer) - count, count);
					}
				}
			}

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		{
			DestroyCaret();
			PostQuitMessage(0);
		}
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
