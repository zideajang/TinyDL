#ifndef TYPENEURALARRAY_H
#define TYPENEURALARRAY_H

//定义神经元元素类型
typedef double typeNeuralUnit;


typedef struct
{
    unsigned long arrayLength; 
    unsigned long arraySize; 
    typeNeuralUnit* arrayData; 
} typeNeuralArray;

long typeNeuralArrayCreata(typeNeuralArray* parent, unsigned long length);
long typeNeuralArrayCopy(typeNeuralArray* parent, typeNeuralArray* source);
long typeNeuralArrayAdd(typeNeuralArray* parent, typeNeuralArray* value1, typeNeuralArray* value2);
long typeNeuralArraySubtract(typeNeuralArray* parent, typeNeuralArray* value1, typeNeuralArray* value2);
long typeNeuralArrayMatrixForward(typeNeuralArray* result, typeNeuralArray* source, typeNeuralArray* weights);
long typeNeuralArrayMultiply(typeNeuralArray* parent, typeNeuralArray* value1, typeNeuralArray* value2);
long typeNeuralArrayDivide(typeNeuralArray* parent, typeNeuralArray* value1, typeNeuralArray* value2);
long typeNeuralArrayMatrixForward(typeNeuralArray* result, typeNeuralArray* source, typeNeuralArray* weights);
long typeNeuralArrayMatrixReverse(typeNeuralArray* result, typeNeuralArray* source, typeNeuralArray* weights);


//创建 Tensor 
long typeNeuralArrayCreata(typeNeuralArray* parent, unsigned long length)
{
    typeNeuralUnit* data;
    unsigned long datalength;

    if (parent == 0)
        return 0;

    //分配内存大小
    datalength = sizeof(typeNeuralUnit) * length;

    //为数据分配内存
    data = (typeNeuralUnit*)malloc(datalength);

    if (data) {
        parent->arrayLength = length;
        parent->arraySize = datalength;
        parent->arrayData = data;

        return 1;
    }

    parent->arrayLength = 0;
    parent->arraySize = 0;
    parent->arrayData = 0;

    return 0;
}

// copy Tensor
long typeNeuralArrayCopy(typeNeuralArray* parent, typeNeuralArray* source)
{
    unsigned long I;

    //判断 parenet 和 source 是否为空
    if ((parent == 0) || (source == 0))
        return 0;

    //判断两个 tensor 长度是否相同
    if (parent->arrayLength != source->arrayLength)
        return 0;

    memcpy(parent->arrayData, source->arrayData, parent->arrayLength * sizeof(typeNeuralUnit));

    return 1;
}

long typeNeuralArrayAdd(typeNeuralArray* parent, typeNeuralArray* value1, typeNeuralArray* value2)
{
    unsigned long I;

    if ((parent == 0) || (value1 == 0) || (value2 == 0))
        return 0;

    if ((parent->arrayLength != value1->arrayLength) || 
        (parent->arrayLength != value2->arrayLength))
        return 0;

    for (I = 0;  I < parent->arrayLength;  I++)
    {
        parent->arrayData[I] = value1->arrayData[I] + value2->arrayData[I];
    }

    return 1;
}

long typeNeuralArraySubtract(typeNeuralArray* parent, typeNeuralArray* value1, typeNeuralArray* value2)
{
    unsigned long I;

    if ((parent == 0) || (value1 == 0) || (value2 == 0))
        return 0;

    if ((parent->arrayLength != value1->arrayLength) || (parent->arrayLength != value2->arrayLength))
        return 0;

    for (I = 0; I < parent->arrayLength; I++)
    {
        parent->arrayData[I] = value1->arrayData[I] - value2->arrayData[I];
    }

    return 1;
}

long typeNeuralArrayMultiply(typeNeuralArray* parent, typeNeuralArray* value1, typeNeuralArray* value2) {
    unsigned long I;

    if ((parent == 0) || (value1 == 0) || (value2 == 0))
        return 0;

    if ((parent->arrayLength != value1->arrayLength) || (parent->arrayLength != value2->arrayLength))
        return 0;

    for (I = 0; I < parent->arrayLength; I++)
    {
        parent->arrayData[I] = value1->arrayData[I] * value2->arrayData[I];
    }

    return 1;
}


long typeNeuralArrayDivide(typeNeuralArray* parent, typeNeuralArray* value1, typeNeuralArray* value2)
{
    unsigned long I;

    if ((parent == 0) || (value1 == 0) || (value2 == 0))
        return 0;

    if ((parent->arrayLength != value1->arrayLength) || (parent->arrayLength != value2->arrayLength))
        return 0;

    for (I = 0; I < parent->arrayLength; I++)
    {
        if (value2->arrayData[I] != 0)
            parent->arrayData[I] = value1->arrayData[I] / value2->arrayData[I];
        else
            parent->arrayData[I] = 0;
    }

    return 1;
}

//简单前向传播
long typeNeuralArrayMatrixForward(typeNeuralArray* result, typeNeuralArray* source, typeNeuralArray* weights)
{
    unsigned long I, J;
    typeNeuralUnit sum;

    if ((result == 0) || (source == 0) || (weights == 0))
        return 0;

    // output = parent.dot(weights)
    // (1,3) dot (3,3) = (1,3) 
    if (weights->arrayLength != (result->arrayLength * source->arrayLength))
        return 0;

    // result 输出数组进行赋值
    for (J = 0; J < result->arrayLength; J++)
    {
        sum = 0;
        for (I = 0; I < source->arrayLength; I++)
        {
            sum += source->arrayData[I] * weights->arrayData[J * source->arrayLength + I];
        }

        result->arrayData[J] = sum;
    }

    return 1;
}

long typeNeuralArrayMatrixReverse(typeNeuralArray* result, typeNeuralArray* source, typeNeuralArray* weights)
{
    //
    unsigned long I, J, K;
    typeNeuralUnit sum;

    if ((result == 0) || (source == 0) || (weights == 0))
        return 0;

    if (weights->arrayLength != (result->arrayLength * source->arrayLength))
        return 0;

    //
    for (I = 0; I < source->arrayLength; I++)
    {
        sum = 0;
        K = I;
        for (J = 0; J < result->arrayLength; J++)
        {
            sum += result->arrayData[J] * weights->arrayData[K];
            K += source->arrayLength;
        }

        source->arrayData[I] = sum;
    }

    return 1;
}

//清空 typeNeuralArray
long typeNeuralArrayDestroy(typeNeuralArray* parent)
{
    if (parent == 0)
        return 0;
    //清空内存
    free(parent->arrayData);

    parent->arrayLength = 0;
    parent->arraySize = 0;
    parent->arrayData = 0;

    return 1;
}

#endif // !TYPENEURALARRAY_H
