#include "typewindow.h"
#include "typebitmap.h"
#include "typeneuralarray.h"
#include "typeimage.h"

#include <stdio.h>
#include <math.h>

static unsigned char readByte(FILE* fp)
{
    // fgetc �ַ���ȡ���������� fp Ϊָ��
    // ��ȡ�ɹ����ض�ȡ�ַ�������ȡ�ļ�ĩβ���߶�ȡʧ��ʱ���� EOF(end of file)
    //���� char ����
    return fgetc(fp);
}

static unsigned short readWord(FILE* fp)
{
    // word ��ʾ 16-bit ��Ҳ���� 2 ���ֽ�
    return (fgetc(fp) | (fgetc(fp) << 8 ));
}


static unsigned long readDWord(FILE* fp)
{
    // Double Word 
    return (fgetc(fp) | (fgetc(fp) << 8) | (fgetc(fp) << 16) | (fgetc(fp) << 24));
}

//���� Bmp ��ʽ�ļ�
long typeFileBmpLoad(typeImage* parent, char* filename)
{
    //
    BITMAPFILEHEADER bfHeader{};
    BITMAPINFOHEADER bfInfo{};

    FILE* fp;
    // fopen �ɹ����ļ�����ָ�룬���򷵻�һ�� NULL fopen(filename,"w")
    // fopen_s �ɹ����� 0 ���򷵻ط��㣬Ȼ�� fopen_s(&file
    // s ��ʾ���Ӱ�ȫ
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


// ��������
typeWindow window;
typeBitmap bitmap;




// Ӧ�ó���������ں���
// APIENTRY �� _stdcall , �������õ�Э�飬_stdcall ����Լ������ Win32 API ������
// wWinMain ����ϵͳ���õ�
int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance, // Ӧ�ó���ʵ�еľ��
    _In_opt_ HINSTANCE hPrevInstance,//ǰһ��ʵ��
    _In_ LPWSTR    lpCmdLine,//�����в��� LP ��ʾָ��
    _In_ int       nCmdShow) //��������ʾ��ʽ
{

    //��������
    typeWindowCreate(&window, hInstance, windowProc);
    //���� bitmap
    typeBitmapCreate(&bitmap, 100, 100);

    while (typeWindowGetEvent(&window)) {
        //
       
    }
    //�˳����� bitmap
    typeBitmapDestroy(&bitmap);
    //����ֵ 0 ��ʾӦ�ó��������˳�
    return 1;
}
// wParam �� lParam ͨ����Ϣ������������ 
LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
     // ���� bitmap
    case WM_PAINT:
    {
        // PAINSTRUCT �ṹ����������ڿͻ��˻��Ƶ���Ϣ
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
    // �����κδ����ְѿ���Ȩ��������ϵͳĬ�ϴ���
    return DefWindowProc(hWnd, message, wParam, lParam);
}