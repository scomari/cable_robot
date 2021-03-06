/**
 * @file calibration_dialog.h
 * @author Simone Comari
 * @date 10 Jan 2020
 * @brief This file include the calibration dialog class.
 */

#ifndef CABLE_ROBOT_CALIBRATION_DIALOG_H
#define CABLE_ROBOT_CALIBRATION_DIALOG_H

#include <QDialog>

#include "easylogging++.h"
#include "libcdpr/inc/cdpr_types.h"

#include "gui/calib/calib_interface_excitation.h"
#include "robot/cablerobot.h"

namespace Ui {
class CalibrationDialog;
}

/**
 * @brief A dialog to select calibration procedure type and bridge communication between
 * selected procedure and main GUI.
 */
class CalibrationDialog: public QDialog
{
  Q_OBJECT

 public:
  /**
   * @brief CalibrationDialog constructor.
   * @param parent The parent Qt object, in our case the main GUI.
   * @param robot Pointer to the cable robot instance, to be passed to the selected
   * calibration procedure interface.
   * @param[in] params Robot parameters.
   */
  CalibrationDialog(QWidget* parent, CableRobot* robot,
                    const grabcdpr::RobotParams& params);
  ~CalibrationDialog();

 signals:
  /**
   * @brief Enable main GUI command.
   */
  void enableMainGUI(const bool);
  /**
   * @brief Calibration end notice.
   */
  void calibrationEnd();

 private slots:
  void on_buttonBox_accepted();
  void on_buttonBox_rejected();

 private slots:
  void fwdCalibFinished();

 private:
  Ui::CalibrationDialog* ui;

  CalibInterfaceExcitation* interface_ = nullptr;
  CableRobot* robot_ptr_               = nullptr;
  grabcdpr::RobotParams params_;
};

#endif // CABLE_ROBOT_CALIBRATION_DIALOG_H
