#ifndef TYPEBITMAP_H
#define TYPEBITMAP_H
#include <windows.h>
//定义 Bitmap 结构体
typedef struct
{
    HBITMAP bitmapHandle;
    DWORD bitmapWidth;
    DWORD bitmapHeight;
    //存放数据
    VOID* bitmapData;
} typeBitmap;


long typeBitmapCreate(typeBitmap* parent, unsigned long width, unsigned long height);
long typeBitmapShow(typeBitmap* parent, typeWindow* window, long x, long y);
long typeBitmapDestroy(typeBitmap* parent);

long typeBitmapCreate(typeBitmap* parent, unsigned long width, unsigned long height)
{
    //设置 bitmap 信息
    BITMAPINFO bmpinfo;
    VOID* bits = 0;

    if (parent == 0)
        return 0;

    ZeroMemory(&bmpinfo, sizeof(BITMAPINFO));

    bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmpinfo.bmiHeader.biWidth = width;
    //这里注意一下在 window 这里需要负的 height
    bmpinfo.bmiHeader.biHeight = -(long)height;
    bmpinfo.bmiHeader.biPlanes = 1;
    bmpinfo.bmiHeader.biBitCount = 32;
    bmpinfo.bmiHeader.biCompression = BI_RGB;
    bmpinfo.bmiHeader.biSizeImage = 0;
    bmpinfo.bmiHeader.biXPelsPerMeter = 0;
    bmpinfo.bmiHeader.biYPelsPerMeter = 0;
    bmpinfo.bmiHeader.biClrUsed = 0;
    bmpinfo.bmiHeader.biClrImportant = 0;

    HBITMAP handle = CreateDIBSection(
        0,
        &bmpinfo,
        DIB_RGB_COLORS,
        &bits,
        NULL,
        NULL);

    if (handle)
    {
        parent->bitmapHandle = handle;
        parent->bitmapWidth = width;
        parent->bitmapHeight = height;
        parent->bitmapData = bits;
        return 1;
    }

    parent->bitmapHandle = 0;
    parent->bitmapWidth = 0;
    parent->bitmapHeight = 0;
    parent->bitmapData = 0;

    return 0;

}

//显示 bitmap
long typeBitmapShow(typeBitmap* parent, typeWindow* window, long x, long y)
{
    // hdc 目标设备的环境句柄
    HDC hdc;
    HDC hdcMem;


    if ((parent == 0) || (window == 0))
        return 0;
    //获取 hwnd 参数所指定窗口的客户区域的设备环境
    hdc = GetDC(window->windowHandle);
    hdcMem = CreateCompatibleDC(hdc);

    //对象(位图)选入指定的设备描述表，替换同类型老的对象
    SelectObject(hdcMem, parent->bitmapHandle);

    // BitBlt 函数用于绘制 bitmap，指定的源设备环境区域中像素进行 bit_block 转换
    BitBlt(hdc, x, y, parent->bitmapWidth, parent->bitmapHeight, hdcMem, 0, 0, SRCCOPY);

    ReleaseDC(window->windowHandle, hdc);
    DeleteObject(hdcMem);

    return 1;

}

long typeBitmapDestroy(typeBitmap* parent)
{
    if (parent == 0)
        return 0;

    DeleteObject(parent->bitmapHandle);

    parent->bitmapHandle = 0;
    parent->bitmapWidth = 0;
    parent->bitmapHeight = 0;
    parent->bitmapData = 0;

    return 1;
}

#endif // !TYPEBITMAP_H
