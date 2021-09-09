#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ImageEditorDialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QColor>
#include <QInputDialog>

#define TEMP_IMG QString("temp_img.jpg")
#define TEMP_YUV QString("temp_yuv.yuv")
#define YUV_OUTPUT QString("yuv_output.yuv")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Image Wizard");
    setAcceptDrops(true);   // allow images to be dragged and dropped into app
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    // Default UI state (prior to loading an image)
    ui->actionConvert->setDisabled(true);
    ui->actionModify->setDisabled(true);
    ui->actionSave->setDisabled(true);
    ui->actionConvertYUV->setDisabled(true);
    ui->actionModifyYUV->setDisabled(true);

    // File Menu Actions
    connect(ui->actionLoad_Image,   &QAction::triggered, this, &MainWindow::selectImageFile);
    connect(ui->actionSave,         &QAction::triggered, this, &MainWindow::saveImageAs);
    connect(ui->actionExit,         &QAction::triggered, this, &MainWindow::exit);

    // Edit Menu Actions
    connect(ui->actionConvert,      &QAction::triggered, this, &MainWindow::convertRGBtoYUV);
    connect(ui->actionModify,       &QAction::triggered, this, &MainWindow::openImageEditor);
    connect(ui->actionConvertYUV,   &QAction::triggered, this, &MainWindow::convertYUVtoRGB);
    connect(ui->actionModifyYUV,    &QAction::triggered, this, &MainWindow::modifyYUVSaturation);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectImageFile()
{
    // Prompt user to load an image (either JPEG or PNG)
    QString img_file = QFileDialog::getOpenFileName(this,
                                                 "Select Image File",
                                                 QString(),
                                                 tr("Images (*.jpg *.jpeg *.png);; JPEG (*.jpg *.jpeg);; PNG (*.png)"));
    if(!img_file.isEmpty())
    {
        loadImage(img_file);
    }
}

void MainWindow::openImageEditor()
{
    ImageEditorDialog *dialog = new ImageEditorDialog(ui->label_image->pixmap().toImage(), this);
    connect(dialog, &ImageEditorDialog::emitSaturation,     this, &MainWindow::modifySaturation);
    connect(dialog, &ImageEditorDialog::emitHue,            this, &MainWindow::modifyHue);
    connect(dialog, SIGNAL(emitRevertChanges(QImage)),      this, SLOT(loadImage(QImage)));
    dialog->exec();
}

void MainWindow::loadImage(QString file)
{
    // Clear instructional text
    ui->label_image->setText("");

    // Display image
    QPixmap img(file);
    ui->label_image->setPixmap(img);

    // Activate image-related menu options
    ui->actionConvert->setEnabled(true);
    ui->actionModify->setEnabled(true);
    ui->actionSave->setEnabled(true);

    // Deactivate YUV actions until a new YUV is generated
    ui->actionConvertYUV->setDisabled(true);
    ui->actionModifyYUV->setDisabled(true);
}

void MainWindow::loadImage(QImage image)
{
    QPixmap pixmap;
    pixmap.convertFromImage(image);
    ui->label_image->setPixmap(pixmap);
}

void MainWindow::saveImageAs()
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
        {
            QMessageBox::information(this, "Image Saved", "Image successfully saved as " + file_path);
        }
        else
        {
            QMessageBox::warning(this, "Error Saving", "There was an error attempting to save " + file_path);
        }
    }
}

bool MainWindow::saveImage(QString file_path)
{
    QImage image = ui->label_image->pixmap().toImage();
    return image.save(file_path);
}

void MainWindow::delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

QString MainWindow::getImageResolution()
{
    int y = ui->label_image->pixmap().toImage().height(); // 525
    int x = ui->label_image->pixmap().toImage().width();   // 906
    return QString::number(x) + "x" + QString::number(y);
}

void MainWindow::exit()
{
    QApplication::quit();
}

void MainWindow::convertRGBtoYUV()
{
    if(QMessageBox::question(this,
                             "Convert to YUV",
                             "Would you like to convert this image to the YUV color space?")
                            == QMessageBox::Yes)
    {
        // save the current file temporarily and convert it to YUV
        QString temp_file = QCoreApplication::applicationDirPath() + "/" + TEMP_IMG;
        if(saveImage(temp_file))
        {
            QString ffmpeg_cmd = "ffmpeg -i " + TEMP_IMG + " -s " + getImageResolution() + " -pix_fmt yuv420p " + YUV_OUTPUT;
            QFile yuv(YUV_OUTPUT);
            yuv.remove();
            system(ffmpeg_cmd.toUtf8());
            ui->actionConvertYUV->setEnabled(true);
            ui->actionModifyYUV->setEnabled(true);
        }
    }
}

void MainWindow::convertYUVtoRGB()
{
    if(QMessageBox::question(this,
                             "Convert to RGB",
                             "Would you like to convert yuv_output.yuv to the RGB color space?")
                            == QMessageBox::Yes)
    {
        QFileInfo yuv_file_info(YUV_OUTPUT);
        if(yuv_file_info.exists())
        {
            QString ffmpeg_cmd = "ffmpeg -s " + getImageResolution() + " -pix_fmt yuv420p -i " + YUV_OUTPUT + " " + TEMP_IMG;
            QFile img(TEMP_IMG);
            img.remove();
            system(ffmpeg_cmd.toUtf8());
            loadImage(TEMP_IMG);
        }
    }
}

void MainWindow::modifyYUVSaturation()
{
    double saturation = QInputDialog::getDouble(this, "Modify YUV Saturation",
                                                "Set Saturation to:", 0, -3, 3);
    QString ffmpeg_cmd = "ffmpeg -s " + getImageResolution() + " -pix_fmt yuv420p -i " + YUV_OUTPUT + " -vf eg=saturation=" + QString::number(saturation) + " -c:a copy " + TEMP_YUV;
    system(ffmpeg_cmd.toUtf8());
    QFile yuv(YUV_OUTPUT);
    yuv.remove();
    QFile temp_yuv(TEMP_YUV);
    temp_yuv.copy(YUV_OUTPUT);
    temp_yuv.remove();
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

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData* data = event->mimeData();

    if(data->hasUrls())
    {
        for(const QUrl &url : data->urls())
        {
            if(url.toLocalFile().endsWith(".png", Qt::CaseInsensitive) ||
                    url.toLocalFile().endsWith(".jpg", Qt::CaseInsensitive) ||
                    url.toLocalFile().endsWith(".jpeg", Qt::CaseInsensitive))
            {
                loadImage(url.toLocalFile());
                return; // only accepting one file
            }
        }
    }
}
