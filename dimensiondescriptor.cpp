#include "dimensiondescriptor.h"

DimensionDescriptor::DimensionDescriptor(int size  /* = 0 */, int firstIndex /* = 0 */)
    : size(size), firstIndex(firstIndex)
{
}

int DimensionDescriptor::getFirstIndex() const
{
    return firstIndex;
}

int DimensionDescriptor::getSize() const
{
    return size;
}

QString DimensionDescriptor::toString() const
{
    return QString("%1 .. %2").arg(firstIndex).arg(size - 1);
}

QDebug &operator<<(QDebug &debug, const DimensionDescriptor &dim)
{
    debug << "[" << dim.toString() << "]";
    return debug;
}
