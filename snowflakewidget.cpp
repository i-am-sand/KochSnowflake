#include "snowflakewidget.h"
#include <QPainter>
#include <QColor>
#include <QVBoxLayout>
#include <QtMath>

SnowflakeWidget::SnowflakeWidget (QWidget *parent) : QWidget{ parent } {
  m_currentlevel = 0;
  m_zoom = 1.0;
  m_offset = QPointF(0, 0);
  m_opacity = 1.0;
  m_slider = new QSlider(Qt::Horizontal, this);
  m_slider->setRange(0, 10);
  m_slider->setValue(0);
  m_label = new QLabel("Уровень: 0", this);
  m_resetButton = new QPushButton("Сброс", this);

  QBoxLayout *layout = new QVBoxLayout(this);
  layout->addStretch();
  layout->addWidget(m_label);
  layout->addWidget(m_slider);
  layout->addWidget(m_resetButton);
  connect(m_slider, &QSlider::valueChanged, this, &SnowflakeWidget::onSliderValueChanged);
  connect(m_resetButton, &QPushButton::clicked, this, &SnowflakeWidget::onResetClicked);
  m_fadeTimer = new QTimer(this);
  m_fadeTimer->setInterval(16);
  connect(m_fadeTimer, &QTimer::timeout,
           this, &SnowflakeWidget::onFadeTick);
}

void SnowflakeWidget::onSliderValueChanged(int value){
  m_currentlevel = value;
  m_label->setText(QString("Уровень: %1").arg(value));
  m_opacity = 0.0;
  m_fadeTimer->start();
  update();
}

void SnowflakeWidget::onResetClicked()
{
  m_zoom = 1.0;
  m_offset = QPointF(0, 0);
  m_slider->setValue(0);
  update();
}

void SnowflakeWidget::onFadeTick() {
  m_opacity += 0.08;
  if (m_opacity >= 1.0) {
      m_opacity = 1.0;
      m_fadeTimer->stop();
    }
  update();
}

void SnowflakeWidget::wheelEvent(QWheelEvent *event) {
  if (event->angleDelta().y() > 0){
      m_zoom *= 1.1;
    } else {
      m_zoom /= 1.1;
    }
  if (m_zoom < 0.5) m_zoom = 0.5;
  if (m_zoom > 50.0) m_zoom = 50.0;
  update();
}

void SnowflakeWidget::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) {
      m_lastMousePos = event->pos();
    }
}

void SnowflakeWidget::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() & Qt::LeftButton) {
      QPointF delta = event->pos() - m_lastMousePos;
      m_offset += delta;
      m_lastMousePos = event->pos();
      update();
    }
}

void SnowflakeWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);
  painter.fillRect(rect(), QColor(20, 25, 40));
  painter.translate(width() / 2.0, height() / 2.0 - 20);
  painter.translate(m_offset);
  painter.scale(m_zoom, m_zoom);

  qreal R = qMin(width(), height()) * 0.35;
  QPointF p1(0, -R);
  QPointF p2(-R * qSqrt(3.0) / 2.0, R / 2.0);
  QPointF p3(R * qSqrt(3.0) / 2.0, R / 2.0);
  painter.setPen(QPen(QColor(150, 220, 255), 1.0));
  painter.setBrush(Qt::NoBrush);
  painter.setOpacity(m_opacity);

  drawKoch(painter, p1, p2, m_currentlevel);
  drawKoch(painter, p2, p3, m_currentlevel);
  drawKoch(painter, p3, p1, m_currentlevel);
}

void SnowflakeWidget::drawKoch(QPainter &painter, const QPointF &a, const QPointF &b, int depth){
  if (!depth){
      painter.drawLine(a, b);
      return;
    }
  QPointF delta, pA, pB;
  delta = (b - a) / 3;
  pA = a + delta;
  pB = a + 2 * delta;

  const qreal cos60 = qCos(M_PI / 3.0);
  const qreal sin60 = qSin(M_PI / 3.0);
  QPointF rotated(delta.x() * cos60 - delta.y() * sin60, delta.x() * sin60 + delta.y() * cos60);
  QPointF peak = pA + rotated;
  drawKoch(painter, a, pA, depth - 1);
  drawKoch(painter, pA, peak, depth - 1);
  drawKoch(painter, peak, pB, depth - 1);
  drawKoch(painter, pB, b, depth - 1);
}