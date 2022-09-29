#include "typewindow.h"
#include "typebitmap.h"
#include "typeneuralarray.h"
#include "typeimage.h"

#include <stdio.h>
#include <math.h>

static unsigned char readByte(FILE* fp)
{
    // fgetc 字符读取函数，这里 fp 为指针
    // 读取成功返回读取字符串，读取文件末尾或者读取失败时返回 EOF(end of file)
    //返回 char 类型
    return fgetc(fp);
}

static unsigned short readWord(FILE* fp)
{
    // word 表示 16-bit ，也就是 2 个字节
    return (fgetc(fp) | (fgetc(fp) << 8 ));
}


static unsigned long readDWord(FILE* fp)
{
    // Double Word 
    return (fgetc(fp) | (fgetc(fp) << 8) | (fgetc(fp) << 16) | (fgetc(fp) << 24));
}

//加载 Bmp 格式文件
long typeFileBmpLoad(typeImage* parent, char* filename)
{
    //
    BITMAPFILEHEADER bfHeader{};
    BITMAPINFOHEADER bfInfo{};

    FILE* fp;
    // fopen 成功打开文件返回指针，否则返回一个 NULL fopen(filename,"w")
    // fopen_s 成功返回 0 否则返回非零，然后 fopen_s(&file
    // s 表示更加安全
    if (fopen_s(&fp, filename, "wb") != 0)
        return 0;

    while (1)
    {
        bfHeader.bfType = readWord(fp);
        bfHeader.bfSize = readDWord(fp);

        bfHeader.bfReserved1 = readWord(fp);
        bfHeader.bfReserved2 = readWord(fp);
        bfHeader.bfOffBits = readDWord(fp);

        if (bfHeader.bfType != 0x4D42)
            break;

        bfInfo.biSize = readDWord(fp);
        bfInfo.biWidth = readDWord(fp);
        bfInfo.biHeight = readDWord(fp);
        bfInfo.biPlanes = readWord(fp);
        bfInfo.biBitCount = readWord(fp);
        bfInfo.biCompression = readDWord(fp);
        bfInfo.biSizeImage = readDWord(fp);
        bfInfo.biXPelsPerMeter = readDWord(fp);
        bfInfo.biYPelsPerMeter = readDWord(fp);
        bfInfo.biClrUsed = readDWord(fp);
        bfInfo.biClrImportant = readDWord(fp);

        if (bfInfo.biCompression != 0)
            break;
        long bytesPerPixel = ((long)bfInfo.biBitCount) / 8;
        long paddedRowSize = (long)ceil((float)(bfInfo.biWidth * bytesPerPixel) / 4) * 4;

        unsigned long I, J, K;
        unsigned long r, g, b;
        unsigned char* buffer;

        if (typeImageCreate(parent, bfInfo.biWidth, bfInfo.biHeight))
        {
            buffer = (unsigned char*)parent->imageData;
            K = 0;
            r = g = b = readByte(fp);
            for (J = 0; J < bfInfo.biHeight; J++)
            {
                fseek(fp, bfHeader.bfOffBits + (bfInfo.biHeight - J  - 1) * (paddedRowSize), SEEK_SET);

                for (I = 0; I < bfInfo.biWidth; I++)
                {
                    if (bytesPerPixel == 1)
                    {
                        r = g = b = readByte(fp);
                    }
                    else if (bytesPerPixel == 3)
                    {
                        r = readByte(fp);
                        g = readByte(fp);
                        b = readByte(fp);
                    }
                    else if (bytesPerPixel == 4)
                    {
                        r = readByte(fp);
                        g = readByte(fp);
                        b = readByte(fp);
                        readByte(fp);
                    }

                    buffer[K + 0] = r;
                    buffer[K + 1] = g;
                    buffer[K + 2] = b;
                    buffer[K + 3] = 0;

                    K += 4;
                }
            }
        }
        fclose(fp);
        return 1;
        }

    fclose(fp);

    return 0;
}



LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// 声明窗口
typeWindow window;
typeBitmap bitmap;




// 应用程序运行入口函数
// APIENTRY 是 _stdcall , 函数调用的协议，_stdcall 调用约定用于 Win32 API 函数，
// wWinMain 是由系统调用的
int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance, // 应用程序实列的句柄
    _In_opt_ HINSTANCE hPrevInstance,//前一个实例
    _In_ LPWSTR    lpCmdLine,//命令行参数 LP 表示指针
    _In_ int       nCmdShow) //主窗口显示方式
{

    //创建窗口
    typeWindowCreate(&window, hInstance, windowProc);
    //创建 bitmap
    typeBitmapCreate(&bitmap, 100, 100);

    while (typeWindowGetEvent(&window)) {
        //
       
    }
    //退出销毁 bitmap
    typeBitmapDestroy(&bitmap);
    //返回值 0 表示应用程序正常退出
    return 1;
}
// wParam 和 lParam 通常消息出参数，其中 
LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
     // 绘制 bitmap
    case WM_PAINT:
    {
        // PAINSTRUCT 结构体包括用于在客户端绘制的信息
        PAINTSTRUCT ps;
        // 
        HDC hdc = BeginPaint(hWnd, &ps);

        typeBitmapShow(&bitmap, &window, 10, 10);

        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        typeWindowDestroy(&window);
        return 0;
        
    }
    // 不做任何处理，又把控制权交给操作系统默认处理
    return DefWindowProc(hWnd, message, wParam, lParam);
}