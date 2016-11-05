#include "arraydimensiondialog.h"
#include "ui_arraydimensiondialog.h"

ArrayDimensionDialog::ArrayDimensionDialog(QWidget *parent) :
    QDialog(parent,  Qt::CustomizeWindowHint|Qt::WindowTitleHint),
    ui(new Ui::ArrayDimensionDialog)
{
    ui->setupUi(this);
}

ArrayDimensionDialog::~ArrayDimensionDialog()
{
    delete ui;
}

DimensionDescriptor ArrayDimensionDialog::getDimensionData(const QString &arrayName, unsigned int dimensionNumber)
{
    static ArrayDimensionDialog *dialog = new ArrayDimensionDialog(0);
    dialog->setWindowTitle(tr("Dimension %1 of \"%2\" array properties").arg(dimensionNumber).arg(arrayName));
    do {
        dialog->exec();
    } while (dialog->ui->dimSizeInput->value() <= 0);
    return DimensionDescriptor(dialog->ui->dimSizeInput->value(), dialog->ui->firstIndexInput->value());
}
