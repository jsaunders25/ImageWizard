#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File menu actions
    void selectImageFile();
    void loadImage(QString file);
    void loadImage(QImage image);
    void saveImageAs();
    void exit();

    // Edit menu actions
    void openImageEditor();

    // Image modifications
    void modifySaturation(int saturation);
    void modifyHue(int hue);
    void convertRGBtoYUV();
    void convertYUVtoRGB();
    void modifyYUVSaturation();

private:
    Ui::MainWindow *ui;    
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    bool saveImage(QString file_path);
    void delay();
    QString getImageResolution();   // returns as "<width>x<height>"
};
#endif // MAINWINDOW_H
