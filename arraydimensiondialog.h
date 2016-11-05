#ifndef ARRAYDIMENSIONDIALOG_H
#define ARRAYDIMENSIONDIALOG_H

#include <QDialog>
#include "dimensiondescriptor.h"

namespace Ui {
class ArrayDimensionDialog;
}

class ArrayDimensionDialog : public QDialog
{
    Q_OBJECT

public:
    ~ArrayDimensionDialog();

    static DimensionDescriptor getDimensionData(const QString &arrayName, unsigned int dimensionNumber);

    ArrayDimensionDialog(const ArrayDimensionDialog &) = delete;
    void operator=(const ArrayDimensionDialog &) = delete;

private:
    explicit ArrayDimensionDialog(QWidget *parent = 0);

    Ui::ArrayDimensionDialog *ui;
};

#endif // ARRAYDIMENSIONDIALOG_H
