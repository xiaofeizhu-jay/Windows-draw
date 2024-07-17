#include "stdfax.h"
#include "math.h"
#include "windows.h"
#include <iostream>

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

DWORD WINAPI ThreadProcOne(LPVOID lpParameter);//线程函数1
DWORD WINAPI ThreadProcTwo(LPVOID lpParameter);//线程函数2
DWORD WINAPI ThreadProcThree(LPVOID lpParameter);//线程函数3
DWORD WINAPI ThreadProcFour(LPVOID lpParameter);//线程函数4
DWORD WINAPI ThreadProcFive(LPVOID lpParameter);//线程函数5
// DWORD WINAPI ThreadProcSeven(LPVOID lpParameter);//线程函数6

void RunThreadSequentially(LPTHREAD_START_ROUTINE threadProc, LPVOID param)
{
    HANDLE hThread = CreateThread(NULL, 0, threadProc, param, 0, NULL);
    if (hThread != NULL)
    {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
    else
    {
        std::cerr << "Failed to create thread\n";
    }
}

int APIENTRY WinMain(HINSTANCE hInstance,     //实例句柄
                     HINSTANCE hPrevInstance, //前一个实例句柄
                     LPSTR lpCmdLine,         //命令行参数
                     int nCmdShow)            //显示方式
{
    WNDCLASS wc = {0}; //窗口类结构
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //背景画刷
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);      //光标
    wc.hInstance = hInstance;                      //实例句柄
    wc.lpfnWndProc = WndProc;                      //窗口过程
    wc.lpszClassName = TEXT("WndClassName");       //窗口类名
    wc.style = CS_HREDRAW | CS_VREDRAW;            //窗口类风格
    if(!RegisterClass(&wc))                        //注册窗口类
    {
        MessageBox(NULL, TEXT("RegisterClass Failed!"), TEXT("Error"), MB_ICONERROR);
        return -1;
    }
    HWND hWnd = CreateWindow(TEXT("WndClassName"), TEXT("yuanyi"), WS_OVERLAPPEDWINDOW,
                             100, 100, 1000, 640, NULL, NULL, hInstance, NULL);
    if(hWnd == NULL){
        return -1;
    }
    UpdateWindow(hWnd);
    ShowWindow(hWnd, SW_SHOW);   //显示窗口;
    MSG uMsg;
    while(GetMessage(&uMsg, NULL, 0, 0)){
        TranslateMessage(&uMsg);   //翻译消息
        DispatchMessage(&uMsg);    //分派消息
    }
    return uMsg.wParam;
}

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int i = 0;
    switch (uMsg){
        case WM_CLOSE://关闭窗口消息
            DestroyWindow(hWnd); //销毁窗口
        break;
        case WM_DESTROY://销毁窗口消息
            PostQuitMessage(0); //退出消息循环
        break;
        case WM_PAINT://绘制窗口消息
        {
            PAINTSTRUCT ps;   //绘图结构
            HDC hdc = BeginPaint(hWnd, &ps); //开始绘图
            RECT rt = {0};    //矩形结构
            GetClientRect(hWnd, &rt); //获取客户区矩形

            MoveToEx(hdc, rt.left, (rt.top + rt.bottom) / 2, NULL); //移动到指定点
            LineTo(hdc, rt.right, (rt.top + rt.bottom) / 2);        //画线

            MoveToEx(hdc, (rt.left + rt.right) / 2, rt.top, NULL); //移动到指定点
            LineTo(hdc, (rt.left + rt.right) / 2, rt.bottom);      //画线
            EndPaint(hWnd, &ps); //结束绘图
        }
        break;
        case WM_LBUTTONDOWN://鼠标左键按下消息
        {
            InvalidateRect(hWnd, NULL, TRUE); //使窗口客户区无效
            DWORD lpParameter[6] = {0};       //线程参数
            //同时创建5个线程
            CloseHandle(CreateThread(NULL, 0, ThreadProcOne, (LPVOID)hWnd, 0, &lpParameter[0]));
            CloseHandle(CreateThread(NULL, 0, ThreadProcTwo, (LPVOID)hWnd, 0, &lpParameter[1]));
            CloseHandle(CreateThread(NULL, 0, ThreadProcThree, (LPVOID)hWnd, 0, &lpParameter[2]));
            CloseHandle(CreateThread(NULL, 0, ThreadProcFour, (LPVOID)hWnd, 0, &lpParameter[3]));
            CloseHandle(CreateThread(NULL, 0, ThreadProcFive, (LPVOID)hWnd, 0, &lpParameter[4]));
            // CloseHandle(CreateThread(NULL, 0, ThreadProcSeven, (LPVOID)hWnd, 0, &lpParameter[5]));
            // RunThreadSequentially(ThreadProcOne, (LPVOID)hWnd);
            // RunThreadSequentially(ThreadProcTwo, (LPVOID)hWnd);
            // RunThreadSequentially(ThreadProcThree, (LPVOID)hWnd);
            // RunThreadSequentially(ThreadProcFour, (LPVOID)hWnd);
            // RunThreadSequentially(ThreadProcFive, (LPVOID)hWnd);
	
        }
        break;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);//默认窗口过程
    }
    return 0;
}

//画横线
DWORD WINAPI ThreadProcOne(LPVOID lpParameter)
{
    HWND hWnd = (HWND)lpParameter; //窗口句柄
    HDC hdc;             //设备上下文
    hdc = GetDC(hWnd);    //获取设备上下文
    RECT rt = {0};    //矩形结构
    GetClientRect(hWnd, &rt); //获取客户区矩形

    int nX1 = rt.left + 80; //起点横坐标
    int nY1 = (rt.top + rt.bottom) / 4;  //起点纵坐标
    int nX2 = (rt.left + rt.right) / 2 - 80; //终点横坐标
    int nY2 = nY1; //终点纵坐标
    int nWind = nX2 - nX1; //线段长度
    for(int k = 0 ; k < 16 ; k++){
        int j = 5;
        for(int t = 0 ; t <= nWind ; t++){
            SetPixel(hdc, nX1 + t, nY1, RGB(124, 252, 0)); //设置像素点颜色
            Sleep(0.9);
        }
        nX1 += 2*j;
        nY1 += 2*j;
    }
    ReleaseDC(hWnd, hdc); //释放设备上下文
    return 0;
}

//画竖线
DWORD WINAPI ThreadProcTwo(LPVOID lpParameter)
{
    HWND hWnd = (HWND)lpParameter; //窗口句柄
    HDC hdc;             //设备上下文
    hdc = GetDC(hWnd);    //获取设备上下文
    RECT rt = {0};    //矩形结构
    GetClientRect(hWnd, &rt); //获取客户区矩形
    int nX1 = (rt.left + rt.right)*3 / 4; //起点横坐标
    int nY1 = rt.top + 80;  //起点纵坐标
    int nX2 = nX1; //终点横坐标
    int nY2 = (rt.top + rt.bottom) / 2 - 80; //终点纵坐标
    int nWind = nY2 - nY1; //线段长度

    for(int k = 0 ; k < 16 ; k++){
        int j = 5;
        for(int t = 0 ; t <= nWind ; t++){
            SetPixel(hdc, nX1, nY1 + t, RGB(255, 140, 0)); //设置像素点颜色
            Sleep(0.9);
        }
        nX1 += j;
        nY1 += j;
    }
    ReleaseDC(hWnd, hdc); //释放设备上下文
    return 0;
}
//画矩形
DWORD WINAPI ThreadProcThree(LPVOID lpParameter)
{
	HWND hWnd = (HWND)lpParameter;
	HDC hdc;
	hdc = GetDC(hWnd);
	RECT rt = { 0 };
	GetClientRect(hWnd, &rt);
	int nX_L = rt.left + 80;
	int nX_R = (rt.left + rt.right) / 2 - 80;
	int nY_T = (rt.top + rt.bottom) / 2 + 80;
	int nY_B = rt.bottom - 80;

	for (int k = 0; k < 16; k++)
	{
		int j = 5;
		int t = 0;
		for (int t = 0; t <= (nX_R - nX_L); t++)
		{
			SetPixel(hdc, nX_L + t, nY_T, RGB(238, 112, 214));
			Sleep(0.9);
		}

		for (t = 0; t <= (nY_B - nY_T); t++)
		{
			SetPixel(hdc, nX_R, nY_T + t, RGB(255, 192, 203));
			Sleep(0.9);
		}

		for (t = 0; t <= (nX_R - nX_L); t++)
		{
			SetPixel(hdc, nX_R - t, nY_B, RGB(255, 0, 255));
			Sleep(0.9);
		}

		for (t = 0; t <= (nY_B - nY_T); t++)
		{
			SetPixel(hdc, nX_L, nY_B - t, RGB(75, 0, 130));
			Sleep(0.9);
		}
		nX_L += j;
		nY_T += j;
		nX_R += j;
		nY_B += j;

	}

	ReleaseDC(hWnd, hdc);

	return 0;
}


//画等边三角形
DWORD WINAPI ThreadProcFour(LPVOID lpParameter)
{
	HWND hWnd = (HWND)lpParameter;
	HDC hdc;
	hdc = GetDC(hWnd);
	RECT rt = { 0 };
	GetClientRect(hWnd, &rt);
	int nWideFree = 120;
	double nX1 = (rt.left + rt.right) / 2 + nWideFree;
	double nX2 = rt.right - nWideFree;
	double nWide = nX2 - nX1;
	double nY_B = rt.bottom - 40;
	double nY_T = nY_B - (sqrt(3)) / 2 * nWide;
	for (int k = 0; k < 16; k++)
	{
		int j = 5;
		int t = 0;
		for (double t = 0; t <= (nWide / 2); t++)
		{
			SetPixel(hdc, (int)(nX1 + t), (int)(nY_B - (sqrt(3))*t), RGB(65, 105, 225));    //sqrt(3)   -->double   精度
			Sleep(0.9);
		}

		for (t = 0; t <= (nWide / 2); t++)
		{
			SetPixel(hdc, (int)((nX1 + nX2) / 2 + t), (int)(nY_T + (sqrt(3))*t), RGB(0, 191, 255));
			Sleep(0.9);
		}

		for (t = 0; t <= nWide; t++)
		{
			SetPixel(hdc, (int)(nX2 - t), (int)nY_B, RGB(0, 206, 209));
			Sleep(0.9);
		}
		int T = 0;

		nX1 += j; nY_B += j;
		T = (nX1 + nX2) / 2;
		t += j; nY_T += j;
		nX2 += j; nY_B += j;

	}

	ReleaseDC(hWnd, hdc);

	return 0;
}



//画圆
DWORD WINAPI ThreadProcFive(LPVOID lpParameter)
{
	HWND hWnd = (HWND)lpParameter;
	HDC hdc;
	hdc = GetDC(hWnd);
	RECT rt = { 0 };
	GetClientRect(hWnd, &rt);

	int nR = 200;
	int nX = (rt.left + rt.right) / 2;
	int nY = (rt.top + rt.bottom) / 2;
	int nX_L = nX - nR;
	int nX_R = nX + nR;
	int nY_T = nY - nR;
	int nY_B = nY + nR;

	for (int k = 0; k < 16; k++)
	{
		int j = 5;
		int t = 0;
		for (int t = 0; t <= nR; t++)
		{

			SetPixel(hdc, nX - t, nY - sqrt((pow(nR, 2) - pow(t, 2))), RGB(255, 0, 0));
			SetPixel(hdc, nX + t, nY + sqrt((pow(nR, 2) - pow(t, 2))), RGB(255, 0, 0));
			SetPixel(hdc, nX + t, nY - sqrt((pow(nR, 2) - pow(t, 2))), RGB(255, 0, 0));
			SetPixel(hdc, nX - t, nY + sqrt((pow(nR, 2) - pow(t, 2))), RGB(255, 0, 0));
			Sleep(0.9);
		}



		for (t = 0; t <= nR; t++)
		{
			SetPixel(hdc, nX_L + t, nY_T + sqrt((pow(nR, 2) - pow(t, 2))), RGB(255, 0, 0));
			SetPixel(hdc, nX_R - t, nY_B - sqrt((pow(nR, 2) - pow(t, 2))), RGB(255, 0, 0));
			Sleep(0.9);
		}

		for (t = 0; t <= nR; t++)
		{

			SetPixel(hdc, nX_L + t, nY_B - sqrt((pow(nR, 2) - pow(t, 2))), RGB(255, 0, 0));
			SetPixel(hdc, nX_R - t, nY_T + sqrt((pow(nR, 2) - pow(t, 2))), RGB(255, 0, 0));
			Sleep(0.9);
		}

		nX += j;
		nY += j;
		nX_L += j;
		nY_T += j;
		nX_R += j;
		nY_B += j;

	}



	ReleaseDC(hWnd, hdc);

	return 0;
}




