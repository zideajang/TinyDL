#ifndef TYPEBITMAP_H
#define TYPEBITMAP_H
#include <windows.h>
//���� Bitmap �ṹ��
typedef struct
{
    HBITMAP bitmapHandle;
    DWORD bitmapWidth;
    DWORD bitmapHeight;
    //�������
    VOID* bitmapData;
} typeBitmap;


long typeBitmapCreate(typeBitmap* parent, unsigned long width, unsigned long height);
long typeBitmapShow(typeBitmap* parent, typeWindow* window, long x, long y);
long typeBitmapDestroy(typeBitmap* parent);

long typeBitmapCreate(typeBitmap* parent, unsigned long width, unsigned long height)
{
    //���� bitmap ��Ϣ
    BITMAPINFO bmpinfo;
    VOID* bits = 0;

    if (parent == 0)
        return 0;

    ZeroMemory(&bmpinfo, sizeof(BITMAPINFO));

    bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmpinfo.bmiHeader.biWidth = width;
    //����ע��һ���� window ������Ҫ���� height
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

//��ʾ bitmap
long typeBitmapShow(typeBitmap* parent, typeWindow* window, long x, long y)
{
    // hdc Ŀ���豸�Ļ������
    HDC hdc;
    HDC hdcMem;


    if ((parent == 0) || (window == 0))
        return 0;
    //��ȡ hwnd ������ָ�����ڵĿͻ�������豸����
    hdc = GetDC(window->windowHandle);
    hdcMem = CreateCompatibleDC(hdc);

    //����(λͼ)ѡ��ָ�����豸�������滻ͬ�����ϵĶ���
    SelectObject(hdcMem, parent->bitmapHandle);

    // BitBlt �������ڻ��� bitmap��ָ����Դ�豸�������������ؽ��� bit_block ת��
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
