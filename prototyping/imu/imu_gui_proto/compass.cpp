#include "compass.h"

Compass::Compass(QWidget* parent)
  : QWidget(parent),
    m_size (COMPASS_SIZE_MIN),
    m_direction (0),
    m_rotateNeedle (false)
{
    setMinimumSize (COMPASS_SIZE_MIN, COMPASS_SIZE_MIN);
    setMaximumSize (COMPASS_SIZE_MAX, COMPASS_SIZE_MAX);
    resize (COMPASS_SIZE_MIN, COMPASS_SIZE_MIN);

    setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy (Qt::StrongFocus);
}

void Compass::setDirection (qint32 _direction)
{
    m_direction = _direction;
    update ();
}

void Compass::setRotateNeedle (bool _rotate)
{
    m_rotateNeedle = _rotate;
    update ();
}

void Compass::paintEvent (QPaintEvent* _event)
{
    QPainter painter (this);
    painter.translate (m_size / 2, m_size / 2);

    // Draw black circle
    painter.setPen (Qt::black);
    painter.setBrush (Qt::black);
    painter.drawEllipse (QPoint (0, 0), m_size / 2, m_size / 2);

    // Draw inner white circles
    painter.setPen (Qt::white);
    painter.drawEllipse (QPointF (0, 0), m_size * 0.3, m_size * 0.3);
    painter.drawEllipse (QPointF (0, 0), m_size * 0.29, m_size * 0.29);

    // Draw center star
    QPainterPath starPath;
    starPath.moveTo(0, 0);
    qreal outerRadius = m_size * 0.29;
    qreal innerRadius = m_size * 0.05;
    for (int angle = 0; angle <= 360; angle += 45)
    {
        if (angle % 90 == 0)
            starPath.lineTo (QPointF (outerRadius * cos(angle / 180.0 * 3.14159),
                                      outerRadius * sin(angle / 180.0 * 3.14159)));
        else
            starPath.lineTo (QPointF (innerRadius * cos(angle / 180.0 * 3.14159),
                                      innerRadius * sin(angle / 180.0 * 3.14159)));
    }
    starPath.closeSubpath();
    starPath.moveTo (0,0);
    outerRadius = m_size * 0.29 / 1.5;
    innerRadius = m_size * 0.05 / 1.5;
    for (int angle = 0; angle <= 360; angle += 45)
    {
        //if ((angle >= 90 && angle <= 180) || (angle >= 270 && angle <= 360 ))
        //{
            if (angle % 90)
                starPath.lineTo (QPointF (outerRadius * cos(angle / 180.0 * 3.14159),
                                          outerRadius * sin(angle / 180.0 * 3.14159)));
            else
                starPath.lineTo (QPointF (innerRadius * cos(angle / 180.0 * 3.14159),
                                          innerRadius * sin(angle / 180.0 * 3.14159)));
        //}
    }
    starPath.setFillRule(Qt::WindingFill);

    // Need to rotate star if we are not rotating the needle
    if (!m_rotateNeedle)
        painter.rotate (-m_direction);

    painter.fillPath (starPath, Qt::white);

    // Undo rotatopn for star
    if (!m_rotateNeedle)
        painter.rotate (m_direction);

    // Rotate painter to rotate the needle
    if (m_rotateNeedle)
        painter.rotate (m_direction);

    // Draw red needle
    painter.setPen (Qt::red);
    QPainterPath redNeedlePath = QPainterPath ();
    redNeedlePath.moveTo (QPointF (0, -(m_size / 2) * 0.6));
    redNeedlePath.lineTo (QPointF (m_size * 0.02, -((m_size/2) * 0.6) + (m_size * 0.03)));
    redNeedlePath.quadTo (QPointF (m_size * 0.02, -(((m_size/2) * 0.6) + (m_size * 0.03)) / 4), QPointF(m_size * 0.05, 0));
    redNeedlePath.lineTo (QPointF (-m_size * 0.05, 0));
    redNeedlePath.quadTo (QPointF (-m_size * 0.02, -(((m_size/2) * 0.6) + (m_size * 0.03)) / 4), QPointF(-m_size * 0.02, -((m_size/2) * 0.6) + (m_size * 0.03)));
    redNeedlePath.closeSubpath ();
    painter.fillPath (redNeedlePath, Qt::red);

    // Draw white needle
    painter.setPen (Qt::white);
    QPainterPath whiteNeedlePath = QPainterPath ();
    whiteNeedlePath.moveTo (QPointF (0, (m_size / 2) * 0.6));
    whiteNeedlePath.lineTo (QPointF (m_size * 0.02, ((m_size/2) * 0.6) - (m_size * 0.03)));
    whiteNeedlePath.quadTo (QPointF (m_size * 0.02, (((m_size/2) * 0.6) - (m_size * 0.03)) / 4), QPointF(m_size * 0.05, 0));
    whiteNeedlePath.lineTo (QPointF (-m_size * 0.05, 0));
    whiteNeedlePath.quadTo (QPointF (-m_size * 0.02, (((m_size/2) * 0.6) - (m_size * 0.03)) / 4), QPointF(-m_size * 0.02, ((m_size/2) * 0.6) - (m_size * 0.03)));
    whiteNeedlePath.closeSubpath ();
    painter.fillPath (whiteNeedlePath, Qt::white);

    // We don't want the screw to rotate in either case
    if (m_rotateNeedle)
        painter.rotate(-m_direction);

    // Draw "screw" on top of needle
    painter.setPen (Qt::black);
    painter.drawEllipse (QPointF (0, 0), m_size * 0.03, m_size * 0.03);

    if (!m_rotateNeedle)
        painter.rotate (-m_direction);


    // Draw directions text and markers
    QPen thickWhite (Qt::white, m_size * 0.02);
    painter.setFont (QFont ("Helvetica",m_size * 0.08, 70));
    QString directions[4] = {"N", "E", "S", "W"};
    for (qint32 i = 0; i < 360; i += 90)
    {
        // Draw text
        painter.setPen (Qt::white);
        QFontMetrics fm = painter.fontMetrics ();
        qint32 dirIndex = i / 90;
        QPoint pos = QPoint (-fm.width (directions[dirIndex]) / 2, -(m_size / 2) + fm.height () + (m_size * 0.02));
        painter.drawText (pos, directions[dirIndex]);

        // Draw marker
        painter.setPen (thickWhite);
        painter.drawLine (QPointF (0, -(m_size / 2) + (m_size * 0.02)), QPointF (0, -(m_size / 2) + (m_size * 0.05)));

        painter.rotate (90);
    }

    // Draw 15 degree marks and text
    painter.setPen (Qt::white);
    painter.setFont (QFont ("Helvetica", m_size * 0.03));
    QFontMetrics fm = painter.fontMetrics ();
    for (qint32 i = 0; i < 360; i += 15)
    {
        if ((i % 90) != 0)
        {
            // Draw text
            QString text = QString::number (i);
            QPoint pos = QPoint (-fm.width (text) / 2, -(m_size / 2) + fm.height () + (m_size * 0.04));
            painter.drawText (pos, text);

            // Draw marker
            painter.drawLine (QPointF (0, -(m_size / 2) + (m_size * 0.02)), QPointF (0, -(m_size / 2) + (m_size * 0.05)));
        }

        painter.rotate (15);
    }

    // Draw 5 degree marks
    for (qint32 i = 0; i < 360; i += 5)
    {
        if ((i % 15) != 0)
            painter.drawLine (QPointF (0, -(m_size / 2) + (m_size * 0.02)), QPointF (0, -(m_size / 2) + (m_size * 0.03)));

        painter.rotate (5);
    }
}

void Compass::resizeEvent (QResizeEvent* _event)
{
    m_size = qMin(width (), height ());
}

void Compass::keyPressEvent (QKeyEvent* _event)
{
    switch (_event->key())
    {
        case Qt::Key_Left:
            m_direction -= 1.0;
            break;
        case Qt::Key_Right:
            m_direction += 1.0;
            break;
        default:
            break;
    }
    update ();
}
