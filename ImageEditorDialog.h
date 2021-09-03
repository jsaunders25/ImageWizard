#ifndef IMAGEEDITORDIALOG_H
#define IMAGEEDITORDIALOG_H

#include <QDialog>

namespace Ui {
class ImageEditorDialog;
}

class ImageEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImageEditorDialog(QWidget *parent = nullptr);
    ~ImageEditorDialog();

signals:
    void emitSaturation(int);
    void emitHue(int);

private slots:
    void modifySaturation(int value);
    void modifyHue(int value);

private:
    Ui::ImageEditorDialog *ui;
};

#endif // IMAGEEDITORDIALOG_H
