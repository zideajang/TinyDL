#ifndef TYPEWINDOW_H
#define TYPEWINDOW_H
#include <windows.h>

// 用 c++ 写神经网络
// 神经网络的可视化实现


/**
设计窗口类
注册窗口类
创建窗口
显示窗口
更新窗口
消息循环
*/

// 定义一个数据结构
typedef struct
{
    HWND    windowHandle; // 窗口句柄
    BOOL      Running;//状态
} typeWindow;

// 创建窗口
long typeWindowCreate(typeWindow* parent, HINSTANCE hInstance, WNDPROC windowProc);
//从消息队列获取消息
long typeWindowGetEvent(typeWindow* parent);
long typeWindowPeekEvent(typeWindow* parent);
//销毁窗口
long typeWindowDestroy(typeWindow* parent);

//
const WCHAR windowClass[] = L"windowClass";
const WCHAR windowTitle[] = L"windowTitle";

// 创建和显示主程序窗口
long typeWindowCreate(typeWindow* parent, HINSTANCE hInstance, WNDPROC windowProc)
{
    // EX 表示扩展的意思
    WNDCLASSEXW wcex;

    if (parent == 0)
        return 0;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;//窗口特征，并不是窗口外观特征，水平重画和垂直重绘制
    wcex.lpfnWndProc = windowProc;//设置用哪个 WindowProc 来处处理消息
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;//当前应用实例句柄
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;//菜单 ID
    wcex.lpszClassName = windowClass;//向系统注册的类名
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    //注册
    if (!RegisterClassExW(&wcex))
    {
        return 0;
    }

    HWND hwnd = CreateWindowW(
        windowClass,//类名称，和刚刚注册一致
        windowTitle,//标题名称
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, //窗口外观样式
        CW_USEDEFAULT,//窗口相对于父级的 x 坐标
        CW_USEDEFAULT,//窗口相对于父级的 y 坐标
        CW_USEDEFAULT,//窗口宽度
        CW_USEDEFAULT, //窗口高度
        NULL, //父窗口
        NULL, //菜单
        hInstance, //应用程序实例
        NULL);//附加数据

    if (!hwnd)
    {
        return 0;
    }

    parent->windowHandle = hwnd;
    parent->Running = 1;

    return 1;
}

/**
消息循环 消息队列
*/
long typeWindowGetEvent(typeWindow* parent)
{
    MSG msg;

    if (parent == 0)
        return 0;
    //获取消息，直到条件不成立，循环跳出
    if (GetMessage(&msg, NULL, 0, 0))
    {
        //用于将转换按键信息
        TranslateMessage(&msg);
        //将消息分发给 WindowProc 来处理
        DispatchMessage(&msg);
    }

    return parent->Running;
}


long typeWindowPeekEvent(typeWindow* parent)
{
    MSG msg;

    if (parent == 0)
        return 0;

    if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return parent->Running;
}

long typeWindowDestroy(typeWindow* parent)
{
    if (parent == 0)
        return 0;

    DestroyWindow(parent->windowHandle);

    parent->windowHandle = 0;
    parent->Running = 0;

    return 1;
}
#endif // !TYPEWINDOW_H
