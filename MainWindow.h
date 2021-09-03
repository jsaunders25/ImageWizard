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
    void loadImage();
    void saveImage();
    void exit();

    // Edit menu actions
    void convertImage();
    void openImageEditor();

    // Image modifications
    void modifySaturation(int saturation);
    void modifyHue(int hue);
    void convertRGBtoYUV();
    void convertYUVtoRGB();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
