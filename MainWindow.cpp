#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ImageEditorDialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Image Wizard");

    // Default UI state (prior to loading an image)
    ui->actionConvert->setDisabled(true);
    ui->actionModify->setDisabled(true);
    ui->actionSave->setDisabled(true);

    // File Menu Actions
    connect(ui->actionLoad_Image,   &QAction::triggered, this, &MainWindow::loadImage);
    connect(ui->actionSave,         &QAction::triggered, this, &MainWindow::saveImage);
    connect(ui->actionExit,         &QAction::triggered, this, &MainWindow::exit);

    // Edit Menu Actions
    connect(ui->actionConvert,  &QAction::triggered, this, &MainWindow::convertImage);
    connect(ui->actionModify,   &QAction::triggered, this, &MainWindow::openImageEditor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openImageEditor()
{
    ImageEditorDialog *dialog = new ImageEditorDialog(this);
    connect(dialog, &ImageEditorDialog::emitSaturation,     this, &MainWindow::modifySaturation);
    connect(dialog, &ImageEditorDialog::emitHue,            this, &MainWindow::modifyHue);
    dialog->exec();
}

void MainWindow::loadImage()
{
    // Prompt user to load an image (either JPEG or PNG)
    QString img_file = QFileDialog::getOpenFileName(this,
                                                 "Select Image File",
                                                 QString(),
                                                 tr("Images (*.jpg *.jpeg *.png);; JPEG (*.jpg *.jpeg);; PNG (*.png)"));
    if(!img_file.isEmpty())
    {
        // Display image
        QPixmap img(img_file);
        ui->label_image->setPixmap(img);

        // Activate image-related menu options
        ui->actionConvert->setEnabled(true);
        ui->actionModify->setEnabled(true);
        ui->actionSave->setEnabled(true);
    }
}

void MainWindow::saveImage()
{
    // Prompt user to save image in any of the provided formats
    QString file_path = QFileDialog::getSaveFileName(this,
                                                     "Save Image As",
                                                     QString(),
                                                     tr("JPEG (*.jpg *.jpeg);; PNG (*.png)"));
    if(!file_path.isEmpty())
    {
        QImage image = ui->label_image->pixmap().toImage();
        if(image.save(file_path))
            QMessageBox::information(this, "Image Saved", "Image successfully saved as " + file_path);
        else
            QMessageBox::warning(this, "Error Saving", "There was an error attempting to save " + file_path);
    }
}

void MainWindow::exit()
{
    QApplication::quit();
}

void MainWindow::convertImage()
{
    // ToDo: Provide user option to convert image between YUV <-> RGB
}

void MainWindow::convertRGBtoYUV()
{
    // ToDo: Convert RGB image to YUV format
}

void MainWindow::convertYUVtoRGB()
{
    // ToDo: Convert YUV image to RGB format
}

void MainWindow::modifySaturation(int saturation)
{
    QImage image = ui->label_image->pixmap().toImage();
    // Go through each pixel and modify the saturation
    for(int i = 0; i < image.width(); ++i)
    {
        for(int j = 0; j < image.height(); ++j)
        {
            QColor color = image.pixelColor(i, j);
            color.setHsv(color.hue(), saturation, color.value(), color.alpha());
            image.setPixelColor(i, j, color);
        }
    }
    QPixmap pixmap;
    pixmap.convertFromImage(image);
    ui->label_image->setPixmap(pixmap);
    update();
}

void MainWindow::modifyHue(int hue)
{
    QImage image = ui->label_image->pixmap().toImage();
    // Go through each pixel and modify the hue
    for(int i = 0; i < image.width(); ++i)
    {
        for(int j = 0; j < image.height(); ++j)
        {
            QColor color = image.pixelColor(i, j);
            color.setHsv(hue, color.saturation(), color.value(), color.alpha());
            image.setPixelColor(i, j, color);
        }
    }
    QPixmap pixmap;
    pixmap.convertFromImage(image);
    ui->label_image->setPixmap(pixmap);
    update();
}

