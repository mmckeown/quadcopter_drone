#include "compass.h"

Compass::Compass(QWidget* parent)
  : QWidget(parent),
    m_size (COMPASS_SIZE_MIN)
{
    setMinimumSize (COMPASS_SIZE_MIN, COMPASS_SIZE_MIN);
    setMaximumSize (COMPASS_SIZE_MAX, COMPASS_SIZE_MAX);
    resize (COMPASS_SIZE_MIN, COMPASS_SIZE_MIN);

    setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy (Qt::StrongFocus);
}

void Compass::paintEvent (QPaintEvent* _event)
{
    QPainter painter (this);
    QPen blackPen (Qt::black);
    painter.setPen (blackPen);
    painter.setBrush (Qt::black);
    painter.translate (width () / 2, height () / 2);
    painter.drawEllipse (QPoint (0, 0), m_size / 2, m_size / 2);
}

void Compass::resizeEvent (QResizeEvent* _event)
{
    m_size = qMin(width (), height ());
    update();
}
