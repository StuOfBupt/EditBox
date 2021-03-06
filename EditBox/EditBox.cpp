//EditBox.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "EditBox.h"

#pragma comment(lib, "Shell32.lib")

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HTEXT hText;									// 文本对象

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Find(HWND, UINT, WPARAM, LPARAM);

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
	   WS_CAPTION |		// 标题栏
	   WS_SYSMENU |		// 窗口调整按钮
	   WS_MAXIMIZE |	// 初始最大化
	   WS_HSCROLL |		// 水平滚动条
	   WS_MINIMIZEBOX |	// 最小化按钮
	   WS_VSCROLL		// 竖直滚动条
	   ,		
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
//	WM_CREATE			- 创建窗口时初始化
//	WM_SHOWWINDOW		- 清空窗口（文本也被清理）
//  WM_COMMAND			- 处理应用程序菜单
//	WN_CLOSE			- 处理应用程序关闭时的资源释放
//	WM_SIZE				- 用户调整大小时进行重绘处理
//	WM_SETFOCOUS		- 用户重新单击应用程序
//	WM_KILLFOCUS		- 用户放弃应用程序
//	WM_KEYDOWN			- 处理虚拟按键
//	WM_CHAR				- 处理字符按键
//	WM_VSCROLL			- 处理用户拖动竖直滚动条
//	WM_HSCROLL			- 处理用户拖动水平滚动条
//	WM_LBUTTONDOWM		- 处理用户单击鼠标左键
//	WM_NCLBUTTONDBLCLK	- 处理用户双击鼠标左键击中标题栏
//	WM_LBUTTONDBLCLK	- 处理用户双击鼠标左键击中显示区
//	WM_MOUSEMOVE		- 处理用户滑动鼠标
//  WM_PAINT			- 绘制主窗口
//  WM_DESTROY			- 发送退出消息并返回
//	
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 字符相关信息
	static UINT			cxChar, cyChar;			// 字符宽高(像素)
	static HFONT		hFont;					// 字体
	static TEXTMETRIC	tm;						// 字符信息

	// 显示区相关信息
	static int cxClient, cyClient;				// 显示区宽高(像素)
	static int cxPageCol, cyPageRow;			// 页面行列数
	static int cxWindowPos, cyWindowPos;		// 显示区位置（像素）

	// 光标相关信息
	static int cxCaretWidth, cyCaretHeight;		// 光标大小
	static int cxCaretPos, cyCaretPos;			// 光标位置(像素)

	// 选中
	static POINT pStart, pEnd;					// 选段起点和终点

    switch (message)
    {
	case WM_CREATE:	// 创建窗口
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

			// 设置字符像素大小（英文）
			GetTextMetrics(hdc, &tm);
			cxChar = tm.tmAveCharWidth;
			cyChar = tm.tmHeight;
			// ...

			// 创建文本对象
			hText = CreateText(cxChar, cyChar);
			// ...
		}
		break;
	case WM_SHOWWINDOW:	// 显示窗口
		{
			RECT rc;
			GetClientRect(hWnd, &rc); 
			cxClient = rc.right - rc.left;
			cyClient = rc.bottom - rc.top;
			// 设置页面栅格
			cxPageCol = cxClient / (cxChar << 1);
			cyPageRow = cyClient / cyChar;
			// 设置显示区像素大小
			cxClient = cxPageCol * (cxChar << 1);
			cyClient = cyPageRow * cyChar;
			// 设置显示区坐标
			cxWindowPos = 0;
			cyWindowPos = 0;
			// ...

			// 设置滚动条
			SCROLLINFO sInfo;
			sInfo.cbSize = sizeof(SCROLLINFO);
			sInfo.fMask = SIF_ALL;
			sInfo.nMax = 0;
			sInfo.nMin = 0;
			sInfo.nPage = cxPageCol;
			sInfo.nPos = 0;
			sInfo.nTrackPos = 0;
			SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
			ShowScrollBar(hWnd, SB_HORZ, TRUE);

			sInfo.cbSize = sizeof(SCROLLINFO);
			sInfo.fMask = SIF_ALL;
			sInfo.nMax = 0;
			sInfo.nMin = 0;
			sInfo.nPage = cyPageRow;
			sInfo.nPos = 0;
			sInfo.nTrackPos = 0;
			SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
			ShowScrollBar(hWnd, SB_VERT, TRUE);
			// ...

			// 设置光标大小
			cxCaretWidth = 1;
			cyCaretHeight = cyChar;
			// 设置光标位置
			cxCaretPos = 0;
			cyCaretPos = 0;
			// 创建光标
			CreateCaret(hWnd, NULL, cxCaretWidth, cyCaretHeight);
			// 重绘光标
			SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
			ShowCaret(hWnd);
			// ...

			// 更新选段
			pStart.x = cxCaretPos;
			pStart.y = cyCaretPos;
			pEnd.x = pStart.x;
			pEnd.y = pStart.y;
			// ...
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
			case IDM_NEW:
				{	// 新建文件
					RVALUE result;
					if ((result =
						UserMessageProc(hText, 0, 0, UM_NEW, NULL, NULL)) != UR_ERROR)
					{
						if (result == UR_NOTSAVED)
						{	// 未保存
							// 保存当前文件
							SendMessage(hWnd, WM_COMMAND, IDM_SAVE, NULL);
							if ((result =	// 再次校验
								UserMessageProc(hText, 0, 0, UM_NEW, NULL, NULL)) != UR_ERROR)
							{
								// 重绘显示区(初始化窗口)
								ShowWindow(hWnd, SW_SHOW);
							}
							else
							{	// 发生异常
								assert(!(result == UR_ERROR));
							}
						}
						else
						{	// 已保存
							// 重绘显示区(初始化窗口)
							ShowWindow(hWnd, SW_SHOW);
						}
					}
					else
					{	// 发生异常
						assert(!(result == UR_ERROR));
					}
				}
				break;
			case IDM_OPEN:
				{	// 打开文件
					// 弹出对话框
					TCHAR szFile[MAX_PATH] = {0};	// 文件名记录
					OPENFILENAME ofn = { 0 };
					ofn.lStructSize = sizeof(OPENFILENAME);
					ofn.hwndOwner = hWnd;
					ofn.lpstrTitle = TEXT("打开文件");
					ofn.lpstrFile = szFile;
					ofn.lpstrFilter = TEXT("txt文本文件(*.txt)\0");
					ofn.nMaxFile = MAX_PATH;
					ofn.nFilterIndex = 1;
					ofn.lpstrInitialDir = TEXT("./");	// 初始化路径
					ofn.Flags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;

					if (GetOpenFileName(&ofn) && lstrcmp(szTitle, TEXT("")) == 0)
					{	// 选中文件
						RVALUE result;
						if ((result =
							UserMessageProc(hText, 0, 0, UM_OPEN, NULL, (SPARAM)szFile)) != UR_ERROR)
						{
							if (result == UR_NOTSAVED)
							{	// 未保存
								// 保存
								SendMessage(hWnd, WM_COMMAND, IDM_SAVE, NULL);
								if ((result =
									UserMessageProc(hText, 0, 0, UM_OPEN, NULL, (SPARAM)szFile)) != UR_ERROR)
								{
									// 重绘滚动条
									SCROLLINFO sInfo;
									sInfo.fMask = SIF_RANGE | SIF_POS;
									GetScrollInfo(hWnd, SB_HORZ, &sInfo);
									sInfo.nMin = 0;
									sInfo.nMax = max(((int)LODWORD(result) - cxClient) / (cxChar << 1), sInfo.nMin);
									sInfo.nPos = 0;
									SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
									ShowScrollBar(hWnd, SB_HORZ, TRUE);

									GetScrollInfo(hWnd, SB_VERT, &sInfo);
									sInfo.nMin = 0;
									sInfo.nMax = max(((int)HIDWORD(result) - cyClient) / cyChar, sInfo.nMin);
									sInfo.nPos = 0;
									SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
									ShowScrollBar(hWnd, SB_VERT, TRUE);
									// ...

									// 设置显示区位置
									cxWindowPos = 0;
									cyWindowPos = 0;
									// ...

									// 重绘光标
									cxCaretPos = 0;
									cyCaretPos = 0;
									SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
									ShowCaret(hWnd);
									// ...

									// 重置选段
									pStart.x = cxCaretPos;
									pStart.y = cyCaretPos;
									pEnd.x = pStart.x;
									pEnd.y = pStart.y;
									// ...

									// 重绘显示区
									InvalidateRect(hWnd, NULL, TRUE);
									UpdateWindow(hWnd);
									// ...
								}
								else
								{	// 发生异常
									assert(!(result == UR_ERROR));
								}
							}
							else
							{
								// 重绘显示区（初始化）
								ShowWindow(hWnd, SW_SHOW);
							}
						}
						else
						{	// 发生异常
							assert(!(result == UR_ERROR));
						}
					}
				}
				break;
			case IDM_SAVE:
				{	// 保存文件
					RVALUE result;
					if ((result =
						UserMessageProc(hText, 0, 0, UM_SAVE, NULL, NULL)) != UR_ERROR)
					{
						if (result == UR_NOPATH)
						{	// 未设置保存路径
							// 另存为
							SendMessage(hWnd, WM_COMMAND, IDM_SAVEAS, NULL);
						}
						else
						{
							// 无操作
						}
					}
					else
					{	// 发生异常
						assert(!(result == UR_ERROR));
					}
				}
				break;
			case IDM_SAVEAS:
				{	// 另存为
					TCHAR szFile[MAX_PATH] = {0};	// 路径记录
					OPENFILENAME ofn;
					ofn.lStructSize = sizeof(OPENFILENAME);
					ofn.hwndOwner = hWnd;
					ofn.lpstrFilter = TEXT("txt文件(*.txt)\0");
					ofn.lpstrInitialDir = TEXT("./");
					ofn.lpstrTitle = TEXT("另存为...");
					ofn.lpstrDefExt = TEXT("txt文件(*.txt)\0");
					ofn.lpstrFile = szFile;
					ofn.nMaxFile = MAX_PATH;
					ofn.nFilterIndex = 1;
					ofn.Flags = OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;

					if (GetSaveFileName(&ofn) && lstrcmp(szFile, TEXT("")) == 0)
					{
						RVALUE result;
						if ((result =
							UserMessageProc(hText, 0, 0, UM_SAVE, NULL, (SPARAM)szFile)) != UR_ERROR)
						{
							// 无界面操作
						}
						else
						{	// 发生异常
							assert(!(result == UR_ERROR));
						}
					}
				}
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
				SendMessage(hWnd, WM_CLOSE, NULL, NULL);
                break;
			case IDM_FIND:
				{
					DialogBox(hInst, MAKEINTRESOURCE(IDD_FINDDIALOG), hWnd, Find);
				}
				break;
			case IDM_CUT:
				break;
			case IDM_COPY:
				break;
			case IDM_PASTE:
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_CLOSE:
		{
			RVALUE result;
			if ((result =
				UserMessageProc(hText, 0, 0, UM_CLOSE, NULL, NULL)) != UR_ERROR)
			{
				if (result == UR_NOTSAVED)
				{	// 未保存
					// 保存
					SendMessage(hWnd, WM_COMMAND, IDM_SAVE, NULL);
				}
				{
					// 无操作
				}
			}
			else
			{	// 发生异常
				assert(!(result == UR_ERROR));
			}
			// 销毁文本对象
			DestroyText(hText);
			// ...

			DestroyWindow(hWnd);
		}
		break;
	case WM_SIZE:
		{
			// 永不被调用
		}
		break;
	case WM_SETFOCUS:	// 获得焦点
		{
			// 新建光标
			CreateCaret(hWnd, NULL, cxCaretWidth, cyCaretHeight);
			// 重绘光标
			SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
			ShowCaret(hWnd);
			// ...
		}
		break;
	case WM_KILLFOCUS:	// 释放焦点
		{
			// 隐藏光标
			HideCaret(hWnd);
			DestroyCaret();
			// ...
		}
		break;
	case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_HOME:
					{	// HOME键
						RVALUE result;
						if ((result =
							UserMessageProc(hText, cxCaretPos, cyCaretPos, UM_HOME, NULL, NULL)) != UR_ERROR)
						{
							// 设置光标位置
							cxCaretPos = (int)LODWORD(result);
							cyCaretPos = (int)HIDWORD(result);
							// ...

							// 滑动显示区
							cxWindowPos = cxCaretPos;
							// cyWindowPos
							// ...

							// 重绘光标
							SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
							ShowCaret(hWnd);
							// ...

							// 滑动水平滚动条
							SendMessage(hWnd, WM_HSCROLL, SB_LEFT, NULL);
							// ...

							// 更新选段
							pStart.x = cxCaretPos;
							pStart.y = cyCaretPos;
							pEnd.x = pStart.x;
							pEnd.y = pStart.y;
							// ...
						}
						else
						{	// 发生异常
							assert(!(result == UR_ERROR));
						}
					}
					break;
				case VK_END:
					{	// END键
						RVALUE result;
						if ((result = 
							UserMessageProc(hText, cxCaretPos, cyCaretPos, UM_END, NULL, NULL)) != UR_ERROR)
						{
							// 设置光标位置
							cxCaretPos = (int)LODWORD(result);
							cyCaretPos = (int)HIDWORD(result);
							// ...

							// 重绘滚动条
							SCROLLINFO sInfo;
							sInfo.fMask = SIF_POS | SIF_RANGE;
							GetScrollInfo(hWnd, SB_HORZ, &sInfo);
							if (cxCaretPos > cxWindowPos + cxPageCol * (cxChar << 1))
							{	// 向右跳页
								int nOffset 
									= min(cxCaretPos - cxWindowPos - (cxPageCol >> 1) * (cxChar << 1),
										sInfo.nMax * (cxChar << 1) - cxWindowPos - cxPageCol * (cxChar << 1));
								// 滑动显示区
								cxWindowPos = cxWindowPos + nOffset;
								// ...

								// 设置滚动条
								sInfo.nPos = sInfo.nPos + (nOffset / (cxChar << 1));
								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);
							}
							else
							{	// 未水平跳页
								// 无操作
							}
							// ...

							// 重绘光标
							SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
							ShowCaret(hWnd);
							// ...

							// 更新选段
							pStart.x = cxCaretPos;
							pStart.y = cyCaretPos;
							pEnd.x = pStart.x;
							pEnd.y = pStart.y;
							// ...
						}
						else
						{	// 异常
							assert(!(result == UR_ERROR));
						}
					}
					break;
				case VK_UP:
					{
						RVALUE result;
						if ((result = 
							UserMessageProc(hText, cxCaretPos, cyCaretPos, UM_UP, NULL, NULL)) != UR_ERROR)
						{
							// 设置光标位置
							cxCaretPos = (int)LODWORD(result);
							cyCaretPos = (int)HIDWORD(result);
							// ...

							// 向上滑动一行
							if (cyCaretPos < cyWindowPos)
							{
								SendMessage(hWnd, WM_HSCROLL, SB_LINEUP, NULL);
							}
							// ...

							// 检测光标是否在显示区内
							if (!INRANGE(cxCaretPos, cxWindowPos, cxWindowPos + cxPageCol * (cxChar << 1)) ||
								!INRANGE(cyCaretPos, cyWindowPos, cyWindowPos + cyPageRow * cyChar))
							{
								// 确定滑动像素大小
								int xOffset =
									cxCaretPos - cxWindowPos - (cxPageCol >> 1) * (cxChar << 1);
								int yOffset =
									cyCaretPos - cyWindowPos - (cyPageRow >> 1) * cyChar;
								// ...

								// 滑动滚动条
								SCROLLINFO sInfo;
								sInfo.fMask = SIF_RANGE | SIF_POS;
								GetScrollInfo(hWnd, SB_HORZ, &sInfo);

								sInfo.nPos = sInfo.nPos + xOffset / (cxChar << 1);
								if (sInfo.nPos < sInfo.nMin)
									sInfo.nPos = sInfo.nMin;
								if (sInfo.nPos > sInfo.nMax)
									sInfo.nPos = sInfo.nMax;
								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);
								// 更新显示区横坐标
								cxWindowPos = sInfo.nPos * (cxChar << 1);
								// ...

								GetScrollInfo(hWnd, SB_VERT, &sInfo);

								sInfo.nPos = sInfo.nPos + yOffset / cyChar;
								if (sInfo.nPos < sInfo.nMin)
									sInfo.nPos = sInfo.nMin;
								if (sInfo.nPos > sInfo.nMax)
									sInfo.nPos = sInfo.nMax;
								SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_VERT, TRUE);
								// 更新显示区纵坐标
								cyWindowPos = sInfo.nPos * cyChar;
								// ...
							}
							// ...

							// 重绘光标
							SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
							ShowCaret(hWnd);
							// ...

							// 更新选段
							pStart.x = cxCaretPos;
							pStart.y = cyCaretPos;
							pEnd.x = pStart.x;
							pEnd.y = pStart.y;
							// ...
						}
						else
						{
							assert(!(result == UR_ERROR));
						}
					}
					break;
				case VK_DOWN:
					{
						RVALUE result;
						if ((result = 
							UserMessageProc(hText, cxCaretPos, cyCaretPos, UM_DOWN, NULL, NULL)) != UR_ERROR)
						{
							// 设置光标
							cxCaretPos = (int)LODWORD(result);
							cyCaretPos = (int)HIDWORD(result);
							// ...

							// 向下滑动一行
							if (cyCaretPos > cyWindowPos + cyPageRow * cyChar)
							{
								SendMessage(hWnd, WM_HSCROLL, SB_LINEDOWN, NULL);
							}
							// ...

							// 检测光标是否在显示区内
							if (!INRANGE(cxCaretPos, cxWindowPos, cxWindowPos + cxPageCol * (cxChar << 1)) ||
								!INRANGE(cyCaretPos, cyWindowPos, cyWindowPos + cyPageRow * cyChar))
							{
								// 确定滑动像素大小
								int xOffset =
									cxCaretPos - cxWindowPos - (cxPageCol >> 1) * (cxChar << 1);
								int yOffset =
									cyCaretPos - cyWindowPos - (cyPageRow >> 1) * cyChar;
								// ...

								// 滑动滚动条
								SCROLLINFO sInfo;
								sInfo.fMask = SIF_RANGE | SIF_POS;
								GetScrollInfo(hWnd, SB_HORZ, &sInfo);

								sInfo.nPos = sInfo.nPos + xOffset / (cxChar << 1);
								if (sInfo.nPos < sInfo.nMin)
									sInfo.nPos = sInfo.nMin;
								if (sInfo.nPos > sInfo.nMax)
									sInfo.nPos = sInfo.nMax;
								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);
								// 更新显示区横坐标
								cxWindowPos = sInfo.nPos * (cxChar << 1);
								// ...

								GetScrollInfo(hWnd, SB_VERT, &sInfo);

								sInfo.nPos = sInfo.nPos + yOffset / cyChar;
								if (sInfo.nPos < sInfo.nMin)
									sInfo.nPos = sInfo.nMin;
								if (sInfo.nPos > sInfo.nMax)
									sInfo.nPos = sInfo.nMax;
								SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_VERT, TRUE);
								// 更新显示区纵坐标
								cyWindowPos = sInfo.nPos * cyChar;
								// ...
							}
							// ...

							// 重绘光标
							SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
							ShowCaret(hWnd);
							// ...

							// 更新选段
							pStart.x = cxCaretPos;
							pStart.y = cyCaretPos;
							pEnd.x = pStart.x;
							pEnd.y = pStart.y;
							// ...
						}
						else
						{	// 光标处于底层
							assert(!(result == UR_ERROR));
						}
					}
					break;
				case VK_RIGHT:
					{
						RVALUE result;
						if ((result =
							UserMessageProc(hText, cxCaretPos, cyCaretPos, UM_RIGHT, NULL, NULL)) != UR_ERROR)
						{	// 同行
							if (result == UR_LINETAIL)
							{	// HOME + DOWN
								SendMessage(hWnd, WM_KEYDOWN, VK_HOME, NULL);
								SendMessage(hWnd, WM_KEYDOWN, VK_DOWN, NULL);
							}
							else
							{
								BOOL isZh = (BOOL)LODWORD(result);

								// 移动光标
								cxCaretPos = cxCaretPos + cxChar + isZh;
								// ...

								// 检测光标是否在显示区内
								if (!INRANGE(cxCaretPos, cxWindowPos, cxWindowPos + cxPageCol * (cxChar << 1)) ||
									!INRANGE(cyCaretPos, cyWindowPos, cyWindowPos + cyPageRow * cyChar))
								{
									// 确定滑动像素大小
									int xOffset =
										cxCaretPos - cxWindowPos - (cxPageCol >> 1) * (cxChar << 1);
									int yOffset =
										cyCaretPos - cyWindowPos - (cyPageRow >> 1) * cyChar;
									// ...

									// 滑动滚动条
									SCROLLINFO sInfo;
									sInfo.fMask = SIF_RANGE | SIF_POS;
									GetScrollInfo(hWnd, SB_HORZ, &sInfo);

									sInfo.nPos = sInfo.nPos + xOffset / (cxChar << 1);
									if (sInfo.nPos < sInfo.nMin)
										sInfo.nPos = sInfo.nMin;
									if (sInfo.nPos > sInfo.nMax)
										sInfo.nPos = sInfo.nMax;
									SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
									ShowScrollBar(hWnd, SB_HORZ, TRUE);
									// 更新显示区横坐标
									cxWindowPos = sInfo.nPos * (cxChar << 1);
									// ...

									GetScrollInfo(hWnd, SB_VERT, &sInfo);

									sInfo.nPos = sInfo.nPos + yOffset / cyChar;
									if (sInfo.nPos < sInfo.nMin)
										sInfo.nPos = sInfo.nMin;
									if (sInfo.nPos > sInfo.nMax)
										sInfo.nPos = sInfo.nMax;
									SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
									ShowScrollBar(hWnd, SB_VERT, TRUE);
									// 更新显示区纵坐标
									cyWindowPos = sInfo.nPos * cyChar;
									// ...
								}
								// ...

								// 重绘光标
								SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
								ShowCaret(hWnd);
								// ...

								// 更新选段
								pStart.x = cxCaretPos;
								pStart.y = cyCaretPos;
								pEnd.x = pStart.x;
								pEnd.y = pStart.y;
								// ...
							}
						}
						else
						{	
							assert(!(result == UR_ERROR));
						}
					}
					break;
				case VK_LEFT:
					{
						RVALUE result;
						if ((result =
							UserMessageProc(hText, cxCaretPos, cyCaretPos, UM_LEFT, NULL, NULL)) != UR_ERROR)
						{
							if (result == UR_LINEHEAD)
							{	// 提行
								SendMessage(hWnd, WM_KEYDOWN, VK_UP, NULL);
								SendMessage(hWnd, WM_KEYDOWN, VK_END, NULL);
							}
							else
							{
								BOOL isZh = (BOOL)LODWORD(result);

								// 移动光标
								cxCaretPos = cxCaretPos - cxChar - isZh;
								// ...

								// 检测光标是否在显示区内
								if (!INRANGE(cxCaretPos, cxWindowPos, cxWindowPos + cxPageCol * (cxChar << 1)) ||
									!INRANGE(cyCaretPos, cyWindowPos, cyWindowPos + cyPageRow * cyChar))
								{
									// 确定滑动像素大小
									int xOffset =
										cxCaretPos - cxWindowPos - (cxPageCol >> 1) * (cxChar << 1);
									int yOffset =
										cyCaretPos - cyWindowPos - (cyPageRow >> 1) * cyChar;
									// ...

									// 滑动滚动条
									SCROLLINFO sInfo;
									sInfo.fMask = SIF_RANGE | SIF_POS;
									GetScrollInfo(hWnd, SB_HORZ, &sInfo);

									sInfo.nPos = sInfo.nPos + xOffset / (cxChar << 1);
									if (sInfo.nPos < sInfo.nMin)
										sInfo.nPos = sInfo.nMin;
									if (sInfo.nPos > sInfo.nMax)
										sInfo.nPos = sInfo.nMax;
									SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
									ShowScrollBar(hWnd, SB_HORZ, TRUE);
									// 更新显示区横坐标
									cxWindowPos = sInfo.nPos * (cxChar << 1);
									// ...

									GetScrollInfo(hWnd, SB_VERT, &sInfo);

									sInfo.nPos = sInfo.nPos + yOffset / cyChar;
									if (sInfo.nPos < sInfo.nMin)
										sInfo.nPos = sInfo.nMin;
									if (sInfo.nPos > sInfo.nMax)
										sInfo.nPos = sInfo.nMax;
									SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
									ShowScrollBar(hWnd, SB_VERT, TRUE);
									// 更新显示区纵坐标
									cyWindowPos = sInfo.nPos * cyChar;
									// ...
								}
								// ...

								// 重绘光标
								SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
								ShowCaret(hWnd);
								// ...

								// 更新选段
								pStart.x = cxCaretPos;
								pStart.y = cyCaretPos;
								pEnd.x = pStart.x;
								pEnd.y = pStart.y;
								// ...
							}
						}
						else
						{	// 发生异常
							assert(!(result == UR_ERROR));
						}
					}
					break;
				case VK_DELETE:
					{
						RVALUE result;
						if ((result =
							UserMessageProc(hText, pStart.x, pStart.y, UM_DELETE, NULL, (SPARAM)(pEnd.y << 32 | pEnd.x))) != UR_ERROR)
						{
							// 获取文本区域大小偏移量
							int xOffset = (int)LODWORD(result);
							int yOffset = (int)HIDWORD(result);
							// ...

							// 设置显示区大小
							SCROLLINFO sInfo;
							sInfo.fMask = SIF_RANGE;
							if (xOffset != 0)
							{
								GetScrollInfo(hWnd, SB_HORZ, &sInfo);
								// 调整滚动条范围
								sInfo.nMin = 0;
								sInfo.nMax = max(sInfo.nMax + (xOffset / (cxChar << 1)), 0);
								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);
							}
							if (yOffset != 0)
							{
								GetScrollInfo(hWnd, SB_VERT, &sInfo);
								// 调整滚动条范围
								sInfo.nMin = 0;
								sInfo.nMax = max(sInfo.nMax + (yOffset / cyChar), 0);
								SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_VERT, TRUE);
							}

							// 检测光标是否在显示区内
							if (!INRANGE(cxCaretPos, cxWindowPos, cxWindowPos + cxPageCol * (cxChar << 1)) ||
								!INRANGE(cyCaretPos, cyWindowPos, cyWindowPos + cyPageRow * cyChar))
							{
								// 确定滑动像素大小
								xOffset =
									cxCaretPos - cxWindowPos - (cxPageCol >> 1) * (cxChar << 1);
								yOffset =
									cyCaretPos - cyWindowPos - (cyPageRow >> 1) * cyChar;
								// ...

								// 滑动滚动条
								SCROLLINFO sInfo;
								sInfo.fMask = SIF_RANGE | SIF_POS;
								GetScrollInfo(hWnd, SB_HORZ, &sInfo);

								sInfo.nPos = sInfo.nPos + xOffset / (cxChar << 1);
								if (sInfo.nPos < sInfo.nMin)
									sInfo.nPos = sInfo.nMin;
								if (sInfo.nPos > sInfo.nMax)
									sInfo.nPos = sInfo.nMax;
								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);
								// 更新显示区横坐标
								cxWindowPos = sInfo.nPos * (cxChar << 1);
								// ...

								GetScrollInfo(hWnd, SB_VERT, &sInfo);

								sInfo.nPos = sInfo.nPos + yOffset / cyChar;
								if (sInfo.nPos < sInfo.nMin)
									sInfo.nPos = sInfo.nMin;
								if (sInfo.nPos > sInfo.nMax)
									sInfo.nPos = sInfo.nMax;
								SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_VERT, TRUE);
								// 更新显示区纵坐标
								cyWindowPos = sInfo.nPos * cyChar;
								// ...
							}
							// ...

							// 显示区位置保持不变
							// ...

							// 光标位置保持不变 必定与pStart相同
							// ...

							// 重置选段
							pStart.x = cxCaretPos;
							pStart.y = cyCaretPos;
							pEnd.x = pStart.x;
							pEnd.y = pStart.y;
							// ...
						}
						else
						{	// 发生异常
							assert(!(result == UR_ERROR));
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
						// 调整光标 -> 挪至上侧
						if (pEnd.y < pStart.y | 
							(pEnd.y == pEnd.y && pEnd.x < pStart.x))
						{
							std::swap(pStart.x, pEnd.x);
							std::swap(pStart.y, pEnd.y);
						}
						cxCaretPos = pStart.x;
						cyCaretPos = pStart.y;
						// ...

						// 判断选段是否为空
						if (pStart.x == pEnd.x &&
							pStart.y == pEnd.y)
						{	// 光标左移
							POINT temp = pEnd;
							SendMessage(hWnd, WM_KEYDOWN, VK_LEFT, NULL);
							// ...

							// 重置选段
							pEnd.x = temp.x;
							pEnd.y = temp.y;
							// ...
						}
						// ...

						// 若光标未在显示区 滑动显示区 -> 将光标显示在中部
						// CatchCaret();
						if (!INRANGE(cxCaretPos, cxWindowPos, cxWindowPos + cxPageCol * (cxChar << 1)) ||
							!INRANGE(cyCaretPos, cyWindowPos, cyWindowPos + cyPageRow * cyChar))
						{
							// 确定滑动像素大小
							int xOffset =
								cxCaretPos - cxWindowPos - (cxPageCol >> 1) * (cxChar << 1);
							int yOffset =
								cyCaretPos - cyWindowPos - (cyPageRow >> 1) * cyChar;
							// ...

							// 滑动滚动条
							SCROLLINFO sInfo;
							sInfo.fMask = SIF_RANGE | SIF_POS;
							GetScrollInfo(hWnd, SB_HORZ, &sInfo);

							sInfo.nPos = sInfo.nPos + xOffset / (cxChar << 1);
							if (sInfo.nPos < sInfo.nMin)
								sInfo.nPos = sInfo.nMin;
							if (sInfo.nPos > sInfo.nMax)
								sInfo.nPos = sInfo.nMax;
							SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
							ShowScrollBar(hWnd, SB_HORZ, TRUE);
							cxWindowPos = sInfo.nPos * (cxChar << 1);

							GetScrollInfo(hWnd, SB_VERT, &sInfo);

							sInfo.nPos = sInfo.nPos + yOffset / cyChar;
							if (sInfo.nPos < sInfo.nMin)
								sInfo.nPos = sInfo.nMin;
							if (sInfo.nPos > sInfo.nMax)
								sInfo.nPos = sInfo.nMax;
							SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
							ShowScrollBar(hWnd, SB_VERT, TRUE);
							cyWindowPos = sInfo.nPos * cyChar;
							// ...
						}
						// ...

						// 重绘光标
						SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
						ShowCaret(hWnd);
						// ...

						// 执行删除
						SendMessage(hWnd, WM_KEYDOWN, VK_DELETE, NULL);
						// ...

						// 重置选段
						pEnd.x = pStart.x;
						pEnd.y = pStart.y;
						// ...
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
						RVALUE result;
						if (result = 
							(UserMessageProc(hText, cxCaretPos, cyCaretPos, UM_RETURN, NULL, NULL)) != UR_ERROR)
						{
							// 获取显示区位置偏移量
							int xOffset = (int)LODWORD(result);
							int yOffset = (int)HIDWORD(result);
							// ...

							// 调整显示区大小
							SCROLLINFO sInfo;
							sInfo.fMask = SIF_RANGE;
							if (xOffset != 0)
							{
								GetScrollInfo(hWnd, SB_HORZ, &sInfo);
								// 调整滚动条范围
								sInfo.nMax = max(sInfo.nMax + xOffset / (cxChar << 1), 0);
								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);
							}
							if (yOffset != 0)
							{
								GetScrollInfo(hWnd, SB_HORZ, &sInfo);
								// 调整滚动条范围
								sInfo.nMax = max(sInfo.nMax + yOffset / cyChar, 0);
								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);
							}
							// ...

							// 移动光标
							SendMessage(hWnd, WM_KEYDOWN, VK_RIGHT, NULL);
							// ...
						}
						else
						{	// 发生异常
							assert(!(result == UR_ERROR));
						}
					}
					break;
				default:
					{
						RVALUE result;
						// 非字符处理（控制命令/快捷键）
						if (TCHAR(wParam) == TEXT('A') || TCHAR(wParam) == TEXT('a'))
						{
							if (GetKeyState(VK_CONTROL) < 0)
							{	// Ctrl + A 全选
								if ((result =
									UserMessageProc(hText, cxCaretPos, cyCaretPos, UM_ALL, NULL, NULL)) != UR_ERROR)
								{
									// 设置光标位置
									cxCaretPos = (int)LODWORD(result);
									cyCaretPos = (int)HIDWORD(result);
									// ...

									// 若光标未在显示区 滑动显示区 -> 将光标显示在中部
									// CatchCaret();
									if (!INRANGE(cxCaretPos, cxWindowPos, cxWindowPos + cxPageCol * (cxChar << 1)) ||
										!INRANGE(cyCaretPos, cyWindowPos, cyWindowPos + cyPageRow * cyChar))
									{
										// 确定滑动像素大小
										int xOffset =
											cxCaretPos - cxWindowPos - (cxPageCol >> 1) * (cxChar << 1);
										int yOffset =
											cyCaretPos - cyWindowPos - (cyPageRow >> 1) * cyChar;
										// ...

										// 滑动滚动条
										SCROLLINFO sInfo;
										sInfo.fMask = SIF_RANGE | SIF_POS;
										GetScrollInfo(hWnd, SB_HORZ, &sInfo);

										sInfo.nPos = sInfo.nPos + xOffset / (cxChar << 1);
										if (sInfo.nPos < sInfo.nMin)
											sInfo.nPos = sInfo.nMin;
										if (sInfo.nPos > sInfo.nMax)
											sInfo.nPos = sInfo.nMax;
										SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
										ShowScrollBar(hWnd, SB_HORZ, TRUE);
										// 更新显示区横坐标
										cxWindowPos = sInfo.nPos * (cxChar << 1);
										// ...

										GetScrollInfo(hWnd, SB_VERT, &sInfo);

										sInfo.nPos = sInfo.nPos + yOffset / cyChar;
										if (sInfo.nPos < sInfo.nMin)
											sInfo.nPos = sInfo.nMin;
										if (sInfo.nPos > sInfo.nMax)
											sInfo.nPos = sInfo.nMax;
										SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
										ShowScrollBar(hWnd, SB_VERT, TRUE);
										// 更新显示区纵坐标
										cyWindowPos = sInfo.nPos * cyChar;
										// ...
									}
									// ...

									// 重绘光标
									SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
									ShowCaret(hWnd);
									// ...

									// 重置选段
									pStart.x = 0;
									pStart.y = 0;
									pEnd.x = cxCaretPos;
									pEnd.y = cyCaretPos;
									// ...
								}
								else
								{
									assert(!(result == UR_ERROR));
								}
								break;	// 跳出 不做字符处理
							}
						}
						else if (TCHAR(wParam) == TEXT('C') || TCHAR(wParam) == TEXT('c'))
						{
							if (GetKeyState(VK_CONTROL) < 0)
							{	// Ctrl + C 拷贝
								if (OpenClipboard(hWnd))
								{	// 打开剪切板
									if (EmptyClipboard())
									{	// 清空剪切板
										LPWSTR lpString = nullptr;
										if ((result =
											UserMessageProc(hText, pStart.x, pStart.y, UM_COPY, (pEnd.y << 32 | pEnd.x), (SPARAM)&lpString)) != UR_ERROR)
										{	// 拷贝到剪切板
											// 字节数目
											int nSize = 2 * LODWORD(result) + 1;
											HGLOBAL hMem = GlobalAlloc(GHND, nSize);
											LPSTR pData = (LPSTR)GlobalLock(hMem);

											// 拷贝
											memcpy_s(pData, nSize, lpString, (nSize - 1) / 2);
											pData[nSize - 1] = '\0';
											// ...

											GlobalUnlock(hMem);
											SetClipboardData(CF_UNICODETEXT, hMem);

											// 关闭剪切板
											CloseClipboard();
											GlobalFree(hMem);
											// ...
										}
										else
										{	// 发生异常
											assert(!(result == UR_ERROR));
										}
									}
								}
								break;	// 跳出 不做字符处理
							}
						}
						else if (TCHAR(wParam) == TEXT('V') || TCHAR(wParam) == TEXT('v'))
						{
							if (GetKeyState(VK_CONTROL) < 0)
							{	// Ctrl + V 粘贴
								if (OpenClipboard(hWnd))
								{	// 从剪切板拷贝
									HANDLE hClipMemory = GetClipboardData(CF_UNICODETEXT);
									// 获取字节数目
									DWORD dwLength = GlobalSize(hClipMemory);
									// ...

									LPBYTE lpClipMemory = (LPBYTE)GlobalLock(hClipMemory);
									LPWSTR lpString = new TCHAR[dwLength / 2 + 1];

									// 拷贝
									memcpy_s(lpString, dwLength / 2, lpClipMemory, dwLength);
									lpString[dwLength / 2] = TEXT('\0');
									// ...

									// 关闭剪切板
									GlobalUnlock(hClipMemory);
									CloseClipboard();
									// ...

									// 重置选段
									pStart.x = cxCaretPos;
									pStart.y = cyCaretPos;
									// ...

									if ((result =
										UserMessageProc(hText, cxCaretPos, cyCaretPos, UM_PASTE, (FPARAM)&pEnd, (SPARAM)lpString)) != UR_ERROR)
									{
										// 获取显示区大小偏移量
										int xOffset = (int)LODWORD(result);
										int yOffset = (int)HIDWORD(result);
										// ...

										// 调整显示区大小
										SCROLLINFO sInfo;
										sInfo.fMask = SIF_RANGE;
										if (xOffset != 0)
										{
											GetScrollInfo(hWnd, SB_HORZ, &sInfo);
											// 调整滚动条范围
											sInfo.nMax = max(sInfo.nMax - (xOffset / (cxChar << 1)), 0);
											SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
											ShowScrollBar(hWnd, SB_HORZ, TRUE);
										}
										if (yOffset != 0)
										{
											GetScrollInfo(hWnd, SB_HORZ, &sInfo);
											// 调整滚动条范围
											sInfo.nMax = max(sInfo.nMax - (yOffset / cyChar), 0);
											SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
											ShowScrollBar(hWnd, SB_HORZ, TRUE);
										}
										// ...

										// 设置光标位置
										cxCaretPos = pEnd.x;
										cyCaretPos = pEnd.y;
										// ...

										// 检测光标是否在显示区内
										if (!INRANGE(cxCaretPos, cxWindowPos, cxWindowPos + cxPageCol * (cxChar << 1)) ||
											!INRANGE(cyCaretPos, cyWindowPos, cyWindowPos + cyPageRow * cyChar))
										{
											// 确定滑动像素大小
											xOffset =
												cxCaretPos - cxWindowPos - (cxPageCol >> 1) * (cxChar << 1);
											yOffset =
												cyCaretPos - cyWindowPos - (cyPageRow >> 1) * cyChar;
											// ...

											// 滑动滚动条
											SCROLLINFO sInfo;
											sInfo.fMask = SIF_RANGE | SIF_POS;
											GetScrollInfo(hWnd, SB_HORZ, &sInfo);

											sInfo.nPos = sInfo.nPos + xOffset / (cxChar << 1);
											if (sInfo.nPos < sInfo.nMin)
												sInfo.nPos = sInfo.nMin;
											if (sInfo.nPos > sInfo.nMax)
												sInfo.nPos = sInfo.nMax;
											SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
											ShowScrollBar(hWnd, SB_HORZ, TRUE);
											// 更新显示区横坐标
											cxWindowPos = sInfo.nPos * (cxChar << 1);
											// ...

											GetScrollInfo(hWnd, SB_VERT, &sInfo);

											sInfo.nPos = sInfo.nPos + yOffset / cyChar;
											if (sInfo.nPos < sInfo.nMin)
												sInfo.nPos = sInfo.nMin;
											if (sInfo.nPos > sInfo.nMax)
												sInfo.nPos = sInfo.nMax;
											SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
											ShowScrollBar(hWnd, SB_VERT, TRUE);
											// 更新显示区纵坐标
											cyWindowPos = sInfo.nPos * cyChar;
											// ...
										}
										// ...
									}
									else
									{	// 发生异常
										assert(!(result == UR_ERROR));
									}

									delete[] lpString;	// 释放临时内存
								}
								break;	// 跳出 不做字符处理
							}
						}
						else if (TCHAR(wParam) == TEXT('X') || TCHAR(wParam) == TEXT('x'))
						{
							if (GetKeyState(VK_CONTROL) < 0)
							{	// Ctrl + X 剪切
								// 设置Ctrl被按下
								BYTE keyState[256] = { 0 };
								keyState[VK_CONTROL] = -1;
								SetKeyboardState(keyState);
								// ...

								SendMessage(hWnd, WM_CHAR, TEXT('C'), NULL);
								SendMessage(hWnd, WM_CHAR, TEXT('\b'), NULL);

								// 恢复Ctrl按键
								keyState[VK_CONTROL] = 0;
								SetKeyboardState(keyState);
								// ...

								break;	// 跳出 不做字符处理
							}
						}
						else if (TCHAR(wParam) == TEXT('Z') || TCHAR(wParam) == TEXT('z'))
						{
							if (GetKeyState(VK_CONTROL) < 0)
							{	// Ctrl + Z 撤销
								if ((result =
									UserMessageProc(hText, 0, 0, UM_CANCEL, (FPARAM)&pStart, (SPARAM)&pEnd)) != UR_ERROR)
								{
									// 获取显示区大小偏移量
									int xOffset = (int)LODWORD(result);
									int yOffset = (int)HIDWORD(result);
									// ...

									// 调整显示区大小
									SCROLLINFO sInfo;
									sInfo.fMask = SIF_RANGE;
									if (xOffset != 0)
									{
										GetScrollInfo(hWnd, SB_HORZ, &sInfo);
										// 调整滚动条范围
										sInfo.nMax = max(sInfo.nMax - (xOffset / (cxChar << 1)), 0);
										SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
										ShowScrollBar(hWnd, SB_HORZ, TRUE);
									}
									if (yOffset != 0)
									{
										GetScrollInfo(hWnd, SB_HORZ, &sInfo);
										// 调整滚动条范围
										sInfo.nMax = max(sInfo.nMax - (yOffset / cyChar), 0);
										SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
										ShowScrollBar(hWnd, SB_HORZ, TRUE);
									}
									// ...

									// 设置光标位置
									cxCaretPos = pEnd.x;
									cyCaretPos = pEnd.y;
									// ...

									// 检测光标是否在显示区内
									if (!INRANGE(cxCaretPos, cxWindowPos, cxWindowPos + cxPageCol * (cxChar << 1)) ||
										!INRANGE(cyCaretPos, cyWindowPos, cyWindowPos + cyPageRow * cyChar))
									{
										// 确定滑动像素大小
										xOffset =
											cxCaretPos - cxWindowPos - (cxPageCol >> 1) * (cxChar << 1);
										yOffset =
											cyCaretPos - cyWindowPos - (cyPageRow >> 1) * cyChar;
										// ...

										// 滑动滚动条
										SCROLLINFO sInfo;
										sInfo.fMask = SIF_RANGE | SIF_POS;
										GetScrollInfo(hWnd, SB_HORZ, &sInfo);

										sInfo.nPos = sInfo.nPos + xOffset / (cxChar << 1);
										if (sInfo.nPos < sInfo.nMin)
											sInfo.nPos = sInfo.nMin;
										if (sInfo.nPos > sInfo.nMax)
											sInfo.nPos = sInfo.nMax;
										SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
										ShowScrollBar(hWnd, SB_HORZ, TRUE);
										// 更新显示区横坐标
										cxWindowPos = sInfo.nPos * (cxChar << 1);
										// ...

										GetScrollInfo(hWnd, SB_VERT, &sInfo);

										sInfo.nPos = sInfo.nPos + yOffset / cyChar;
										if (sInfo.nPos < sInfo.nMin)
											sInfo.nPos = sInfo.nMin;
										if (sInfo.nPos > sInfo.nMax)
											sInfo.nPos = sInfo.nMax;
										SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
										ShowScrollBar(hWnd, SB_VERT, TRUE);
										// 更新显示区纵坐标
										cyWindowPos = sInfo.nPos * cyChar;
										// ...
									}
									// ...
								}
								break;	// 跳出 不做字符处理
							}
						}
						else if (TCHAR(wParam) == TEXT('F') || TCHAR(wParam) == TEXT('f'))
						{
							if (GetKeyState(VK_CONTROL) < 0)
							{	// Ctrl + F 查找
								SendMessage(hWnd, WM_COMMAND, IDM_FIND, NULL);
								break;	// 跳出 不做字符处理
							}
						}
						else if (TCHAR(wParam) == TEXT('S') || TCHAR(wParam) == TEXT('s'))
						{
							if (GetKeyState(VK_CONTROL) < 0)
							{	// Ctrl + S 保存
								SendMessage(hWnd, WM_COMMAND, IDM_SAVE, NULL);
								break;	// 跳出 不做字符处理
							}
						}
						// ...

						// 字符处理
						if ((result = 
							UserMessageProc(hText, cxCaretPos, cyCaretPos, UM_CHAR, NULL, (SPARAM)wParam)) != UR_ERROR)
						{
							// 获取显示区大小偏移量
							int xOffset = (int)LODWORD(result);
							int yOffset = (int)HIDWORD(result);
							// ...

							// 调整显示区大小
							SCROLLINFO sInfo;
							sInfo.fMask = SIF_RANGE;
							if (xOffset != 0)
							{
								GetScrollInfo(hWnd, SB_HORZ, &sInfo);
								// 调整滚动条范围
								sInfo.nMax = max(sInfo.nMax - (xOffset / (cxChar << 1)), 0);
								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);
							}
							if (yOffset != 0)
							{
								GetScrollInfo(hWnd, SB_HORZ, &sInfo);
								// 调整滚动条范围
								sInfo.nMax = max(sInfo.nMax - (yOffset / cyChar), 0);
								SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
								ShowScrollBar(hWnd, SB_HORZ, TRUE);
							}
							// ...

							// 移动光标
							SendMessage(hWnd, WM_KEYDOWN, VK_RIGHT, NULL);
							// ...
						}
						else
						{	// 发生异常
							assert(!(result == UR_ERROR));
						}
					}
					break;
				}
			//}
		}
		break;
	case WM_VSCROLL:
		{
			// 获取滚动条信息
			SCROLLINFO	sInfo;
			sInfo.fMask = SIF_ALL;
			GetScrollInfo(hWnd, SB_VERT, &sInfo);
			// ...

			int iVertPos = sInfo.nPos;	// 记录初始位置
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
			// 更新显示区位置纵坐标
			cyWindowPos = cyWindowPos + (sInfo.nPos - iVertPos) * cyChar;
			// ...

			// 更新滚动条
			sInfo.fMask = SIF_POS;
			SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
			ShowScrollBar(hWnd, SB_VERT, TRUE);
			// ...

			// 重绘显示区
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			// ...
		}
		break;
	case WM_HSCROLL:
		{
			// 获取滚动条信息
			SCROLLINFO sInfo;
			sInfo.fMask = SIF_ALL;
			GetScrollInfo(hWnd, SB_HORZ, &sInfo);
			// ...

			int iVertPos = sInfo.nPos;	// 记录初始位置
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
			// 更新显示区位置纵坐标
			cxWindowPos = cxWindowPos + (sInfo.nPos - iVertPos) * (cxChar << 1);
			// ...

			// 更新滚动条
			sInfo.fMask = SIF_POS;
			SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
			ShowScrollBar(hWnd, SB_HORZ, TRUE);
			// ...

			// 重绘显示区
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			// ...
		}
		break;
	case WM_LBUTTONDOWN:
		{
			// 解析鼠标点击位置
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(hWnd, &p);
			if (!INRANGE(p.x, 0, cxPageCol * (cxChar << 1)) ||
				!INRANGE(p.y, 0, cyPageRow * cyChar))
			{	// 未击中显示区
				return 0;
			}
			p.x += cxWindowPos;
			p.y += cyWindowPos;
			// ...

			RVALUE result;
			if ((result = 
				UserMessageProc(hText, p.x, p.y, UM_CURSOR, NULL, NULL)) != UR_ERROR)
			{	
				// 设置光标位置
				cxCaretPos = (int)LODWORD(result);
				cyCaretPos = (int)HIDWORD(result);
				// ...

				// 检测光标是否在显示区内
				if (!INRANGE(cxCaretPos, cxWindowPos, cxWindowPos + cxPageCol * (cxChar << 1)) ||
					!INRANGE(cyCaretPos, cyWindowPos, cyWindowPos + cyPageRow * cyChar))
				{
					// 确定滑动像素大小
					int xOffset =
						cxCaretPos - cxWindowPos - (cxPageCol >> 1) * (cxChar << 1);
					int yOffset =
						cyCaretPos - cyWindowPos - (cyPageRow >> 1) * cyChar;
					// ...

					// 滑动滚动条
					SCROLLINFO sInfo;
					sInfo.fMask = SIF_RANGE | SIF_POS;
					GetScrollInfo(hWnd, SB_HORZ, &sInfo);

					sInfo.nPos = sInfo.nPos + xOffset / (cxChar << 1);
					if (sInfo.nPos < sInfo.nMin)
						sInfo.nPos = sInfo.nMin;
					if (sInfo.nPos > sInfo.nMax)
						sInfo.nPos = sInfo.nMax;
					SetScrollInfo(hWnd, SB_HORZ, &sInfo, FALSE);
					ShowScrollBar(hWnd, SB_HORZ, TRUE);
					// 更新显示区横坐标
					cxWindowPos = sInfo.nPos * (cxChar << 1);
					// ...

					GetScrollInfo(hWnd, SB_VERT, &sInfo);

					sInfo.nPos = sInfo.nPos + yOffset / cyChar;
					if (sInfo.nPos < sInfo.nMin)
						sInfo.nPos = sInfo.nMin;
					if (sInfo.nPos > sInfo.nMax)
						sInfo.nPos = sInfo.nMax;
					SetScrollInfo(hWnd, SB_VERT, &sInfo, FALSE);
					ShowScrollBar(hWnd, SB_VERT, TRUE);
					// 更新显示区纵坐标
					cyWindowPos = sInfo.nPos * cyChar;
					// ...
				}
				// ...

				// 重绘光标
				SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
				ShowCaret(hWnd);
				// ...

				// 更新选段
				pStart.x = cxCaretPos;
				pStart.y = cyCaretPos;
				pEnd.x = pStart.x;
				pEnd.y = pStart.y;
				// ...
			}
			else
			{	// 发生异常
				assert(!(result == UR_ERROR));
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			//*可注释掉该case的内容
			// 解析鼠标位置
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(hWnd, &p);
			p.x += cxWindowPos;
			p.y += cyWindowPos;

			RVALUE result;
			if ((result =
				UserMessageProc(hText, p.x, p.y, UM_CHOOSE, pStart.y << 32 | pStart.x, NULL)) != UR_ERROR)
			{
				// 设置光标位置
				cxCaretPos = (int)LODWORD(result);
				cyCaretPos = (int)HIDWORD(result);
				// ...

				// 重绘光标
				SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
				ShowCaret(hWnd);
				// ...

				// 更新选段
				pEnd.x = cxCaretPos;
				pEnd.y = cyCaretPos;
				// ...

				// 重绘显示区
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
				// ...	
			}
			
			//*/
		}
		break;
	case WM_NCLBUTTONDBLCLK:
		{
			if (HTCAPTION == wParam)
			{	// 禁用双击还原

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
				{	// 按住左键
					POINT p;	// 用户鼠标落点
					GetCursorPos(&p);
					ScreenToClient(hWnd, &p);
					// ...

					if (p.y > cyChar * cyPageRow)
					{	// 下移
						SendMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, NULL);
					}
					else if (p.x > (cxChar << 1) * cxPageCol)
					{	// 右移
						SendMessage(hWnd, WM_HSCROLL, SB_LINERIGHT, NULL);
					}
					else if (p.y < 0)
					{	// 上移
						SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, NULL);
					}
					else if (p.x < 0)
					{	// 下移
						SendMessage(hWnd, WM_HSCROLL, SB_LINELEFT, NULL);
					}
					// 用户鼠标落点转化为绝对坐标
					p.x += cxWindowPos;
					p.y += cyWindowPos;

					RVALUE result;
					if ((result =
						UserMessageProc(hText, p.x, p.y, UM_CHOOSE, pStart.y << 32 | pStart.x, NULL)) != UR_ERROR)
					{
						// 更新光标
						cxCaretPos = (int)LODWORD(result);
						cyCaretPos = (int)HIDWORD(result);
						SetCaretPos(cxCaretPos - cxWindowPos, cyCaretPos - cyWindowPos);
						ShowCaret(hWnd);
						// ...

						// 更新选段
						pEnd.x = cxCaretPos;
						pEnd.y = cyCaretPos;
						// ...

						// 重绘显示区
						InvalidateRect(hWnd, NULL, TRUE);
						UpdateWindow(hWnd);
						// ...
					}
					else
					{	// 发生异常
						assert(!(result == UR_ERROR));
					}
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

			// 获得无效区域
			RECT rc = ps.rcPaint;
			LPWSTR szBuffer = nullptr;
			RVALUE result;
			for (int row = rc.top; row <= rc.bottom; row += cyChar)
			{
				if ((result = 
					UserMessageProc(hText, rc.left + cxWindowPos, row + cyWindowPos,
						UM_TEXT, NULL, (SPARAM)&szBuffer)) != UR_ERROR)
				{
					short count = (short)LOWORD(HIDWORD(result));	// 选中字符数目
					short flag = (short)HIWORD(HIDWORD(result));	// 选中字符位置
					int offset = (int)LODWORD(result);				// 待高亮字符偏移左端的像素大小
					if (flag == UR_FRONT)
					{	// 前面count个字符进行高亮
						SetTextColor(hdc, RGB(0xFF, 0xFF, 0xFF));
						SetBkColor(hdc, RGB(0x18, 0x74, 0xCD));
						TextOut(hdc, rc.left, row, szBuffer, count);

						SetTextColor(hdc, RGB(0x00, 0x00, 0x00));
						SetBkColor(hdc, RGB(0xFF, 0xFF, 0xFF));
						TextOut(hdc, rc.left + offset, row, szBuffer + count, wcslen(szBuffer) - count);
					}
					else if (flag == UR_BACK)
					{	// 后面count个字符进行高亮
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

// “查找”框的消息处理程序。
INT_PTR CALLBACK Find(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			else if (LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
		}
		break;
	}
	return (INT_PTR)FALSE;
}
