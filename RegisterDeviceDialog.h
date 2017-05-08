#ifndef REGISTERDEVICEWINDOW_H
#define REGISTERDEVICEWINDOW_H

#include <QDialog>
#include <QDebug>

namespace Ui {
  class RegisterDeviceDialog;
}

class RegisterDeviceDialog : public QDialog
{
  Q_OBJECT

public:
  explicit RegisterDeviceDialog( QWidget *parent = 0 );
  ~RegisterDeviceDialog();

private:
  Ui::RegisterDeviceDialog *ui;

private slots:
  void on_buttonNewDeviceCancel_clicked();
  void on_buttonNewDeviceSubmit_clicked();
};

#endif // REGISTERDEVICEWINDOW_H
