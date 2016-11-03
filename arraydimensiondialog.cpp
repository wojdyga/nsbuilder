#include "arraydimensiondialog.h"
#include "ui_arraydimensiondialog.h"

ArrayDimensionDialog::ArrayDimensionDialog(QWidget *parent) :
    QDialog(parent,  Qt::CustomizeWindowHint|Qt::WindowTitleHint),
    ui(new Ui::ArrayDimensionDialog)
{
    ui->setupUi(this);
}

ArrayDimensionDialog &ArrayDimensionDialog::getInstance()
{
    static ArrayDimensionDialog instance(0);
    return instance;
}

ArrayDimensionDialog::~ArrayDimensionDialog()
{
    delete ui;
}

DimensionDescriptor ArrayDimensionDialog::getDimensionData(const QString &arrayName, unsigned int dimensionNumber)
{
    setWindowTitle(tr("Dimension %1 of \"%2\" array properties").arg(dimensionNumber).arg(arrayName));
    do {
        exec();
    } while (ui->dimSizeInput->value() <= 0);
    return DimensionDescriptor(ui->dimSizeInput->value(), ui->firstIndexInput->value());
}
