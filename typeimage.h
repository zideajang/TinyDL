#ifndef TYPEIMAGE_H
#define TYPEIMAGE_H
typedef struct
{
    unsigned long   imageWidth;
    unsigned long   imageHeight;
    unsigned long   imageLength;
    unsigned long   imageSize;
    unsigned long* imageData;
} typeImage;

long typeImageCreate(typeImage* parent, unsigned long width, unsigned long height);
long typeImageDestroy(typeImage* parent);

long typeImageCreate(typeImage* parent, unsigned long width, unsigned long height)
{
    void* data;
    unsigned long length;
    unsigned long datalength;

    if (parent == 0)
        return 0;

    length = width * height;
    datalength = length * sizeof(unsigned long);
    data = malloc(datalength);

    if (data)
    {
        parent->imageWidth = width;
        parent->imageHeight = height;
        parent->imageLength = length;
        parent->imageSize = datalength;
        parent->imageData = (unsigned long*)data;
        return 1;
    }

    parent->imageWidth = 0;
    parent->imageHeight = 0;
    parent->imageLength = 0;
    parent->imageSize = 0;
    parent->imageData = 0;
    return 0;

}

//Çå¿Õ typeImage
long typeImageDestroy(typeImage* parent)
{
    if (parent == 0)
        return 0;
    free(parent->imageData);

    parent->imageWidth = 0;
    parent->imageHeight = 0;
    parent->imageLength = 0;
    parent->imageSize = 0;
    parent->imageData = 0;
    return 0;
}
#endif // !TYPEIMAGE_H

