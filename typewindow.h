#ifndef TYPEWINDOW_H
#define TYPEWINDOW_H
#include <windows.h>

// �� c++ д������
// ������Ŀ��ӻ�ʵ��


/**
��ƴ�����
ע�ᴰ����
��������
��ʾ����
���´���
��Ϣѭ��
*/

// ����һ�����ݽṹ
typedef struct
{
    HWND    windowHandle; // ���ھ��
    BOOL      Running;//״̬
} typeWindow;

// ��������
long typeWindowCreate(typeWindow* parent, HINSTANCE hInstance, WNDPROC windowProc);
//����Ϣ���л�ȡ��Ϣ
long typeWindowGetEvent(typeWindow* parent);
long typeWindowPeekEvent(typeWindow* parent);
//���ٴ���
long typeWindowDestroy(typeWindow* parent);

//
const WCHAR windowClass[] = L"windowClass";
const WCHAR windowTitle[] = L"windowTitle";

// ��������ʾ�����򴰿�
long typeWindowCreate(typeWindow* parent, HINSTANCE hInstance, WNDPROC windowProc)
{
    // EX ��ʾ��չ����˼
    WNDCLASSEXW wcex;

    if (parent == 0)
        return 0;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;//���������������Ǵ������������ˮƽ�ػ��ʹ�ֱ�ػ���
    wcex.lpfnWndProc = windowProc;//�������ĸ� WindowProc ����������Ϣ
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;//��ǰӦ��ʵ�����
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;//�˵� ID
    wcex.lpszClassName = windowClass;//��ϵͳע�������
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    //ע��
    if (!RegisterClassExW(&wcex))
    {
        return 0;
    }

    HWND hwnd = CreateWindowW(
        windowClass,//�����ƣ��͸ո�ע��һ��
        windowTitle,//��������
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, //���������ʽ
        CW_USEDEFAULT,//��������ڸ����� x ����
        CW_USEDEFAULT,//��������ڸ����� y ����
        CW_USEDEFAULT,//���ڿ��
        CW_USEDEFAULT, //���ڸ߶�
        NULL, //������
        NULL, //�˵�
        hInstance, //Ӧ�ó���ʵ��
        NULL);//��������

    if (!hwnd)
    {
        return 0;
    }

    parent->windowHandle = hwnd;
    parent->Running = 1;

    return 1;
}

/**
��Ϣѭ�� ��Ϣ����
*/
long typeWindowGetEvent(typeWindow* parent)
{
    MSG msg;

    if (parent == 0)
        return 0;
    //��ȡ��Ϣ��ֱ��������������ѭ������
    if (GetMessage(&msg, NULL, 0, 0))
    {
        //���ڽ�ת��������Ϣ
        TranslateMessage(&msg);
        //����Ϣ�ַ��� WindowProc ������
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
