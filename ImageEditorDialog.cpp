#include "ImageEditorDialog.h"
#include "ui_ImageEditorDialog.h"

ImageEditorDialog::ImageEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageEditorDialog)
{
    ui->setupUi(this);

    // Connect sliders with appropriate functions to modify image
    connect(ui->dial_saturation,    &QDial::valueChanged,   this, &ImageEditorDialog::modifySaturation);
    connect(ui->dial_saturation,    &QDial::sliderMoved,    this, &ImageEditorDialog::modifySaturation);
    connect(ui->dial_hue,           &QDial::valueChanged,   this, &ImageEditorDialog::modifyHue);
    connect(ui->dial_hue,           &QDial::sliderMoved,    this, &ImageEditorDialog::modifyHue);

    connect(ui->pushButton_close, &QPushButton::clicked, this, &ImageEditorDialog::close);
}

ImageEditorDialog::~ImageEditorDialog()
{
    delete ui;
}

void ImageEditorDialog::modifySaturation(int value)
{
    ui->label_saturation->setText("Saturation: " + QString::number(ui->dial_saturation->value()));
    emit emitSaturation(value);
}

void ImageEditorDialog::modifyHue(int value)
{
    ui->label_hue->setText("Hue: " + QString::number(ui->dial_hue->value()));
    emit emitHue(value);
}
