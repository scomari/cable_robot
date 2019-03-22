/**
 * @file camera_widget.h
 * @author Simone Comari
 * @date 22 Mar 2019
 * @brief This file includes a widget to stream a USB camera.
 */

#ifndef CABLE_ROBOT_HOMING_CAMERA_WIDGET_H
#define CABLE_ROBOT_HOMING_CAMERA_WIDGET_H

#include <QCameraInfo>
#include <QCloseEvent>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QMessageBox>
#include <QObject>
#include <QPixmap>
#include <QTimer>
#include <QWidget>

#include "opencv2/opencv.hpp"

#include "easylogging++.h"

#include "gui/homing/camera_calib_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class CameraWidget;
}
QT_END_NAMESPACE

/**
 * @brief The VideoStreamType enum
 */
enum VideoStreamType
{
  ORIGINAL,
  GRAYSCALE
};

/**
 * @brief The QGraphicsVideoStreamerItem class
 */
class QGraphicsVideoStreamerItem: public QObject, public QGraphicsPixmapItem
{
  Q_OBJECT

 public:
  /**
   * @brief QGraphicsVideoStreamerItem
   */
  QGraphicsVideoStreamerItem();

  /**
   * @brief recording
   * @param value
   */
  void recording(const bool value);

 private slots:
  void changeRecSymbolStatus() { rec_symbol_active_ = !rec_symbol_active_; }

 private:
  static constexpr int kRecSymIntervalMsec_ = 1000;

  QTimer record_symbol_timer_;
  bool rec_symbol_active_ = false;

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override final;
};


/**
 * @brief The CameraWidget class
 */
class CameraWidget: public QWidget
{
  Q_OBJECT

 public:
  /**
   * @brief CameraWidget
   */
  CameraWidget();
  ~CameraWidget();

  const cv::Mat& getLatestFrame() const { return frame_; }

  void stopVideoStream();

 signals:
  /**
   * @brief printToQConsole
   */
  void printToQConsole(const QString&) const;
  /**
   * @brief newFrameGrabbed
   */
  void newFrameGrabbed(const cv::Mat&) const;
  /**
   * @brief calibParamsReady
   */
  void calibParamsReady(const CalibParams&) const;

 private slots:
  void on_comboBox_channel_currentIndexChanged(const QString& arg1);

  void on_pushButton_start_clicked();
  void on_pushButton_stop_clicked();
  void on_pushButton_calib_clicked();

  void on_pushButton_imgDir_clicked();
  void on_pushButton_takeImage_clicked();

  void on_pushButton_videoDir_clicked();
  void on_pushButton_record_clicked();
  void on_pushButton_stopRec_clicked();

private slots:
  void saveCalibParams(const CalibParams& params);
  void frwPrintToQConsole(const QString& msg) { emit printToQConsole(msg); }

private:
  Ui::CameraWidget* ui;
  CameraCalibDialog* calib_dialog_ = NULL;

  QGraphicsVideoStreamerItem video_streamer_;
  cv::VideoCapture video_;
  cv::VideoWriter video_rec_;
  cv::Mat frame_;
  QImage qimg_;
  VideoStreamType stream_type_;

  CalibParams calib_params_;

  void stream();

  void processFrame(const cv::Mat& raw_frame);
  void mapToQImage();

  void displayStream();
  void displayCapturedImage();

  void closeEvent(QCloseEvent* event);

  void deleteCalibDialog();
};

#endif // CABLE_ROBOT_HOMING_CAMERA_WIDGET_H
