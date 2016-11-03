#ifndef DIMENSIONDESCRIPTOR_H
#define DIMENSIONDESCRIPTOR_H

#include <QString>
#include <QDebug>

class DimensionDescriptor
{
public:
    explicit DimensionDescriptor(int size = 0, int firstIndex = 0);

    int getSize() const;
    int getFirstIndex() const;

    QString toString() const;

private:
    int size;
    int firstIndex;
};

QDebug &operator<<(QDebug &debug, const DimensionDescriptor &dim);

#endif // DIMENSIONDESCRIPTOR_H
