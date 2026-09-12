#ifndef SNOWFLAKEWIDGET_H
#define SNOWFLAKEWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QPainter>
#include <QPointF>
#include <QTimer>
#include <QLabel>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPushButton>

class SnowflakeWidget : public QWidget
{
  Q_OBJECT
public:
  explicit SnowflakeWidget (QWidget *parent = nullptr);
protected:
  void paintEvent(QPaintEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
private slots:
  void onSliderValueChanged(int value);
  void onTimerTick();
  void onResetClicked();
private:
  QSlider *m_slider;
  int m_currentlevel;
  void drawKoch(QPainter &painter, const QPointF &a, const QPointF &b, int depth);
  QTimer *m_drawTimer;
  int m_drawProgress;
  int m_segmentsDrawn;
  int m_segmentsToDraw;
  QPointF m_cursorPos;
  QLabel *m_label;
  QPushButton *m_resetButton;
  qreal m_zoom;
  QPointF m_offset;
  QPointF m_lastMousePos;

signals:
};

#endif // SNOWFLAKEWIDGET_H
