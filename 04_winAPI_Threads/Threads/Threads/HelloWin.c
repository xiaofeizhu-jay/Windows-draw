#include<windows.h>

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
//声明了一个 Windows API 函数 WndProc，它是一个窗口过程函数，用于处理窗口消息。

int WINAPI WinMain(HINSTANCE hInstance , HINSTANCE hPrevInstance,
                    PSTR szCmdLine , int iCmdShow)
{
    static TCHAR szAppName[] = TEXT ("HelloWin");  //窗口类名
    HWND   hwnd;                                   //窗口句柄
    MSG    msg;                                    //消息结构
    WNDCLASS wndclass;                             //窗口类结构

    wndclass.style         = CS_HREDRAW | CS_VREDRAW;  //窗口类风格
    wndclass.lpfnWndProc   = WndProc;                  //窗口过程函数
    wndclass.cbClsExtra    = 0;                        //窗口类额外字节数
    wndclass.cbWndExtra    = 0;                        //窗口额外字节数
    wndclass.hInstance     = hInstance;                //实例句柄
    wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION);  //图标句柄
    wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);       //光标句柄
    wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);  //背景画刷句柄
    wndclass.lpszMenuName  = NULL;                     //菜单名
    wndclass.lpszClassName = szAppName;                //窗口类名

    if(!RegisterClass (&wndclass))                     //注册窗口类
    {
        MessageBox (NULL, TEXT ("This program requires Windows NT!"),
                    szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow (szAppName,                  //窗口类名
                        TEXT ("The Hello Program"),  //窗口标题
                        WS_OVERLAPPEDWINDOW,         //窗口风格
                        CW_USEDEFAULT, CW_USEDEFAULT,//窗口位置
                        CW_USEDEFAULT, CW_USEDEFAULT,//窗口尺寸
                        NULL, NULL, hInstance, NULL);//父窗口句柄、菜单句柄、实例句柄、创建参数
    ShowWindow (hwnd , iCmdShow);                      //显示窗口
    UpdateWindow (hwnd);                               //更新窗口

    while(GetMessage (&msg, NULL, 0, 0))                //消息循环
    {
        TranslateMessage (&msg);                       //翻译消息
        DispatchMessage (&msg);                        //分派消息
    }
    return msg.wParam;                                 //返回消息参数
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC         hdc; //设备环境句柄
    PAINTSTRUCT ps;  //绘图结构
    RECT        rect;//矩形结构

    switch (message)
    {
    case WM_CREATE://窗口创建消息
        //PlaySound (TEXT ("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
        return 0;
    case WM_PAINT: //窗口绘制消息
        hdc = BeginPaint (hwnd, &ps); //开始绘图
        GetClientRect (hwnd, &rect);  //获取客户区矩形
        DrawText (hdc, TEXT ("Hello, Windows 98!"), -1, &rect,
                  DT_SINGLELINE | DT_CENTER | DT_VCENTER); //绘制文本
        EndPaint (hwnd, &ps);  //结束绘图
        return 0;
    case WM_DESTROY://窗口销毁消息
        PostQuitMessage (0);
        return 0;
    }
    return DefWindowProc (hwnd, message, wParam, lParam);
}