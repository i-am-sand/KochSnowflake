#include "snowflakewidget.h"
#include <QPainter>
#include <QColor>
#include <QVBoxLayout>
#include <QtMath>

SnowflakeWidget::SnowflakeWidget (QWidget *parent) : QWidget{ parent } {
  m_currentlevel = 0;
  m_drawProgress = 100;
  m_segmentsDrawn = 0;
  m_segmentsToDraw = 0;
  m_slider = new QSlider(Qt::Horizontal, this);
  m_slider->setRange(0, 7);
  m_slider->setValue(0);
  m_label = new QLabel("Уровень: 0", this);

  QBoxLayout *layout = new QVBoxLayout(this);
  layout->addStretch();
  layout->addWidget(m_label);
  layout->addWidget(m_slider);
  connect(m_slider, &QSlider::valueChanged, this, &SnowflakeWidget::onSliderValueChanged);
  m_drawTimer = new QTimer(this);
  m_drawTimer->setInterval(30);
  connect(m_drawTimer, &QTimer::timeout, this, &SnowflakeWidget::onTimerTick);

}

void SnowflakeWidget::onSliderValueChanged(int value){
  m_currentlevel = value;
  m_label->setText(QString("Уровень: %1").arg(value));
  m_drawProgress = 0;
  m_drawTimer->start();
  update();
}

void SnowflakeWidget::onTimerTick(){
  m_drawProgress += 2;
  if (m_drawProgress >= 100) {
      m_drawProgress = 100;
      m_drawTimer->stop();
    }
  update();
}

void SnowflakeWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);
  painter.fillRect(rect(), QColor(20, 25, 40));
  painter.translate(width() / 2.0, height() / 2.0 - 20);

  qreal R = qMin(width(), height()) * 0.35;
  QPointF p1(0, -R);
  QPointF p2(-R * qSqrt(3.0) / 2.0, R / 2.0);
  QPointF p3(R * qSqrt(3.0) / 2.0, R / 2.0);
  painter.setPen(QPen(QColor(150, 220, 255), 1.5));
  painter.setBrush(Qt::NoBrush);
  int totalSegments = 3 * static_cast<int>(qPow(4, m_currentlevel));
  m_segmentsToDraw = totalSegments * m_drawProgress / 100;
  m_segmentsDrawn = 0;
  m_cursorPos = p1;
  drawKoch(painter, p1, p2, m_currentlevel);
  drawKoch(painter, p2, p3, m_currentlevel);
  drawKoch(painter, p3, p1, m_currentlevel);
  if (m_segmentsDrawn > 0 && m_segmentsDrawn < totalSegments) {
      painter.setPen(Qt::NoPen);
      painter.setBrush(QColor(255, 240, 150));
      painter.drawEllipse(m_cursorPos, 5, 5);
    }
}

void SnowflakeWidget::drawKoch(QPainter &painter, const QPointF &a, const QPointF &b, int depth){
  if (m_segmentsDrawn >= m_segmentsToDraw) return;
  if (!depth){
      painter.drawLine(a, b);
      ++m_segmentsDrawn;
      m_cursorPos = b;
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