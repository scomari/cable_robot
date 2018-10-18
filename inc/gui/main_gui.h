#ifndef MAIN_GUI_H
#define MAIN_GUI_H

#include <QDialog>

#include "calibration_dialog.h"
#include "homing_dialog.h"

#include "robot/cablerobot.h"

namespace Ui
{
class MainGUI;
}

class MainGUI : public QDialog
{
  Q_OBJECT

public:
  MainGUI(QWidget* parent, QString& config_filename);
  ~MainGUI();

private slots:
  void on_pushButton_calib_clicked();

  void on_pushButton_homing_clicked();

  void on_pushButton_startApp_clicked();

  void on_pushButton_enable_clicked();
  void on_pushButton_faultReset_clicked();

  void on_radioButton_posMode_clicked();
  void on_radioButton_velMode_clicked();
  void on_radioButton_torqueMode_clicked();

  void EnableInterface(const bool op_outcome = false);

  void AppendText2Browser(const QString& text);

private:
  Ui::MainGUI* ui = NULL;
  CalibrationDialog* calib_dialog_ = NULL;
  HomingDialog* homing_dialog_ = NULL;

  QString config_filename_;
  bool manual_ctrl_enabled_ = false;

  CableRobot robot_;

  void DisablePosCtrlButtons(const bool value);
  void DisableVelCtrlButtons(const bool value);
  void DisableTorqueCtrlButtons(const bool value);
};

#endif // MAIN_GUI_H