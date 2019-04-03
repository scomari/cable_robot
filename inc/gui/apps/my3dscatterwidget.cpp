#include "my3dscatterwidget.h"
#include "ui_my3dscatterwidget.h"

#include <QScreen>
#include <QtCore/qmath.h>
#include <QtCore/qrandom.h>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtDataVisualization/qscatter3dseries.h>
#include <QtDataVisualization/qscatterdataproxy.h>
#include <QtDataVisualization/qvalue3daxis.h>

#include "libgeom/inc/rotations.h"

using namespace QtDataVisualization;

static QVector3D randVector()
{
  return QVector3D((float)(QRandomGenerator::global()->bounded(100)) / 2.0f -
                     (float)(QRandomGenerator::global()->bounded(100)) / 2.0f,
                   (float)(QRandomGenerator::global()->bounded(100)) / 2.0f -
                     (float)(QRandomGenerator::global()->bounded(100)) / 2.0f,
                   (float)(QRandomGenerator::global()->bounded(100)) / 2.0f -
                     (float)(QRandomGenerator::global()->bounded(100)) / 2.0f);
}

My3DScatterWidget::My3DScatterWidget(QWidget* parent)
  : QWidget(parent), ui(new Ui::My3DScatterWidget)
{
  ui->setupUi(this);

  graph_ = new Q3DScatter();
  graph_->activeTheme()->setType(Q3DTheme::ThemeArmyBlue);
  QFont font = graph_->activeTheme()->font();
  font.setPointSize(40.0f);
  graph_->activeTheme()->setFont(font);
  graph_->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
  graph_->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetIsometricRight);
  QScatterDataProxy* proxy = new QScatterDataProxy;
  QScatter3DSeries* series = new QScatter3DSeries(proxy);
  series->setItemLabelFormat(
    QStringLiteral("@xTitle: @xLabel @zTitle: @zLabel @yTitle: @yLabel"));
  series->setMeshSmooth(true);
  series->setMesh(QAbstract3DSeries::MeshPoint);
  series->setItemSize(0.0f);
  graph_->addSeries(series);
  graph_->axisX()->setTitle("X [m]");
  graph_->axisX()->setTitleVisible(true);
  graph_->axisY()->setTitle("Z [m]");
  graph_->axisY()->setTitleVisible(true);
  graph_->axisZ()->setTitle("Y [m]");
  graph_->axisZ()->setTitleVisible(true);
  input_handler_ = new CustomInputHandler(this);
  graph_->setActiveInputHandler(input_handler_);

  QWidget* container = QWidget::createWindowContainer(graph_);
  QSize screenSize   = graph_->screen()->size();
  container->setMinimumSize(QSize(screenSize.width() / 3.5, screenSize.height() / 2.5));
  container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  container->setSizeIncrement(1, 1);
  container->setFocusPolicy(Qt::StrongFocus);

  ui->verticalLayout->insertWidget(0, container, 1);

  connect(graph_, &Q3DScatter::widthChanged, input_handler_,
          &CustomInputHandler::setWidgetWidth);
  connect(graph_, &Q3DScatter::heightChanged, input_handler_,
          &CustomInputHandler::setWidgetHeight);
}

My3DScatterWidget::~My3DScatterWidget() { delete ui; }

void My3DScatterWidget::setTrajectory(const vect<TrajectoryD>& trajectory)
{
  double data_min = 1e10;
  double data_max = -1e10;
  int m_itemCount = 900;

  QScatterDataArray* dataArray = new QScatterDataArray;
  dataArray->resize(m_itemCount);
  QScatterDataItem* ptrToDataArray = &dataArray->first();
  for (int i = 0; i < m_itemCount; i++)
  {
    QVector3D data_point = randVector();
    ptrToDataArray->setPosition(data_point);
    ptrToDataArray++;

    double point_min = qMin(data_point.x(), qMin(data_point.y(), data_point.z()));
    data_min         = qMin(data_min, point_min);
    double point_max = qMax(data_point.x(), qMax(data_point.y(), data_point.z()));
    data_max         = qMax(data_max, point_max);
  }

  ui->horizontalSlider_xmin->setRange(data_min, data_max);
  ui->horizontalSlider_ymin->setRange(data_min, data_max);
  ui->horizontalSlider_zmin->setRange(data_min, data_max);
  ui->horizontalSlider_xmin->setValue(data_min);
  ui->horizontalSlider_ymin->setValue(data_min);
  ui->horizontalSlider_zmin->setValue(data_min);

  ui->horizontalSlider_xmax->setRange(data_min, data_max);
  ui->horizontalSlider_ymax->setRange(data_min, data_max);
  ui->horizontalSlider_zmax->setRange(data_min, data_max);
  ui->horizontalSlider_xmax->setValue(data_min);
  ui->horizontalSlider_ymax->setValue(data_min);
  ui->horizontalSlider_zmax->setValue(data_min);

  graph_->seriesList().at(0)->dataProxy()->resetArray(dataArray);
  graph_->axisX()->setRange(data_min, data_max);
  graph_->axisY()->setRange(data_min, data_max);
  graph_->axisZ()->setRange(data_min, data_max);
  graph_->setAspectRatio(1.0);
  graph_->setHorizontalAspectRatio(1.0);
  graph_->scene()->activeCamera()->setZoomLevel(140);
}

void My3DScatterWidget::on_horizontalSlider_zmin_valueChanged(int value)
{
  graph_->axisY()->setMin(value);
}

void My3DScatterWidget::on_horizontalSlider_ymin_valueChanged(int value)
{
  graph_->axisZ()->setMin(value);
}

void My3DScatterWidget::on_horizontalSlider_xmin_valueChanged(int value)
{
  graph_->axisX()->setMin(value);
}

void My3DScatterWidget::on_horizontalSlider_zmax_valueChanged(int value)
{
  graph_->axisY()->setMax(ui->horizontalSlider_ymax->maximum() -
                          (value - ui->horizontalSlider_ymax->minimum()));
}

void My3DScatterWidget::on_horizontalSlider_ymax_valueChanged(int value)
{
  graph_->axisZ()->setMax(ui->horizontalSlider_zmax->maximum() -
                          (value - ui->horizontalSlider_zmax->minimum()));
}

void My3DScatterWidget::on_horizontalSlider_xmax_valueChanged(int value)
{
  graph_->axisX()->setMax(ui->horizontalSlider_xmax->maximum() -
                          (value - ui->horizontalSlider_xmax->minimum()));
}


CustomInputHandler::CustomInputHandler(QObject* parent)
  : Q3DInputHandler(parent), pressed_(false)
{}

CustomInputHandler::CustomInputHandler(QObject* parent, const QSize& parent_widget_size)
  : Q3DInputHandler(parent), pressed_(false)
{
  setWidgetSize(parent_widget_size);
}

void CustomInputHandler::setWidgetSize(const QSize& size)
{
  widget_size_ = size;
  qDebug() << widget_size_;
}

void CustomInputHandler::setWidgetWidth(const int& width)
{
  widget_size_.setWidth(width);
  qDebug() << widget_size_;
}

void CustomInputHandler::setWidgetHeight(const int& height)
{
  widget_size_.setHeight(height);
  qDebug() << widget_size_;
}

void CustomInputHandler::mousePressEvent(QMouseEvent* event, const QPoint& mousePos)
{
  Q3DInputHandler::mousePressEvent(event, mousePos);
  if (event->button() == Qt::LeftButton)
  {
    prev_target_ = scene()->activeCamera()->target();
    start_pos_   = mousePos;
    pressed_     = true;
  }
}

void CustomInputHandler::mouseReleaseEvent(QMouseEvent* event, const QPoint& mousePos)
{
  Q3DInputHandler::mouseReleaseEvent(event, mousePos);
  if (event->button() == Qt::LeftButton)
    pressed_ = false;
}

void CustomInputHandler::mouseMoveEvent(QMouseEvent* event, const QPoint& mousePos)
{
  Q3DInputHandler::mouseMoveEvent(event, mousePos);
  if (pressed_)
  {
    float du = -(2 * (mousePos.x() - start_pos_.x()) / widget_size_.width());
    float dv = 2 * (mousePos.y() - start_pos_.y()) / widget_size_.height();
    grabnum::Vector3d view_vec;
    view_vec.Fill({du, dv, 0});
    grabnum::Matrix3d R =
      grabgeom::EulerXYZ2Rot(0, qDegreesToRadians(scene()->activeCamera()->yRotation()),
                             qDegreesToRadians(scene()->activeCamera()->xRotation()));
    grabnum::Vector3d camera_view = R.Transpose() * view_vec;
    scene()->activeCamera()->setTarget(
      QVector3D(camera_view(1), camera_view(2), camera_view(3)) + prev_target_);
    //    qDebug() << du << scene()->activeCamera()->xRotation()
    //             << scene()->activeCamera()->yRotation();
  }
}
