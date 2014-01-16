#include "attitude_indicator.h"

const qreal   AttitudeIndicator::DEFAULTS_ROLL_ROTATE[AttitudeIndicator::NUM_ROLL_LINES] = {270.0, 30.0, 15.0, 15.0, 10.0, 10.0,
                                                                                            10.0, 10.0, 10.0, 10.0, 15.0, 15.0, 30.0};
const AttitudeIndicator::ATTITUDE_LINE_TYPE AttitudeIndicator::DEFAULTS_TYPE_ROLL[AttitudeIndicator::NUM_ROLL_LINES] = {AttitudeIndicator::NORMAL_ROLL_LINE,
                                                                                                                        AttitudeIndicator::NORMAL_ROLL_LINE,
                                                                                                                        AttitudeIndicator::SMALL_ROLL_LINE,
                                                                                                                        AttitudeIndicator::NORMAL_ROLL_LINE,
                                                                                                                        AttitudeIndicator::SMALL_ROLL_LINE,
                                                                                                                        AttitudeIndicator::SMALL_ROLL_LINE,
                                                                                                                        AttitudeIndicator::NORMAL_ROLL_LINE,
                                                                                                                        AttitudeIndicator::SMALL_ROLL_LINE,
                                                                                                                        AttitudeIndicator::SMALL_ROLL_LINE,
                                                                                                                        AttitudeIndicator::NORMAL_ROLL_LINE,
                                                                                                                        AttitudeIndicator::SMALL_ROLL_LINE,
                                                                                                                        AttitudeIndicator::NORMAL_ROLL_LINE,
                                                                                                                        AttitudeIndicator::NORMAL_ROLL_LINE};
const AttitudeIndicator::ATTITUDE_LINE_TYPE AttitudeIndicator::DEFAULTS_TYPE_PITCH[AttitudeIndicator::NUM_PITCH_LINES] = {AttitudeIndicator::NORMAL_PITCH_LINE,
                                                                                                                          AttitudeIndicator::SMALL_PITCH_LINE,
                                                                                                                          AttitudeIndicator::NORMAL_PITCH_LINE,
                                                                                                                          AttitudeIndicator::SMALL_PITCH_LINE,
                                                                                                                          AttitudeIndicator::SMALL_PITCH_LINE,
                                                                                                                          AttitudeIndicator::NORMAL_PITCH_LINE,
                                                                                                                          AttitudeIndicator::SMALL_PITCH_LINE,
                                                                                                                          AttitudeIndicator::NORMAL_PITCH_LINE};

AttitudeIndicator::AttitudeIndicator (QWidget* _parent)
    : QWidget (_parent),
      m_size (INDICATOR_SIZE_MIN)
{
    QTimer* timer = new QTimer(this);
    connect (timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start (1000);

    setMinimumSize (INDICATOR_SIZE_MIN, INDICATOR_SIZE_MIN);
    setMaximumSize (INDICATOR_SIZE_MAX, INDICATOR_SIZE_MAX);
    resize (m_size, m_size);

    // from 0 degrees;
    for (int i = 0; i < NUM_ROLL_LINES; i++)
    {
        m_rollRotate[i] = DEFAULTS_ROLL_ROTATE[i];
        getRollLine (DEFAULTS_TYPE_ROLL[i], &m_rollPoint[i][0], &m_rollPoint[i][1]);
    }

    for(int i = 0; i < NUM_PITCH_LINES;i++)
        getPitchLine (DEFAULTS_TYPE_PITCH[i], i, &m_pitchPoint[i][0], &m_pitchPoint[i][1]);

    initTargetChar ();
    initRollChar ();
    m_roll = 0.0;
    m_pitch = 0.0;

    setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy (Qt::StrongFocus);
}

AttitudeIndicator::~AttitudeIndicator ()
{
}

void AttitudeIndicator::paintEvent (QPaintEvent * _event)
{
    QPainter painter (this);
    QPoint center (0, 0);
    QPen whitePen (Qt::white);
    QPen blackPen (Qt::black);
    QBrush bgSky (QColor (48, 172, 220));
    QBrush bgGround (QColor (247, 168, 21));
    whitePen.setWidth (2);
    blackPen.setWidth (1);
    painter.setRenderHint (QPainter::Antialiasing);
    painter.translate (width () / 2, height () / 2);
    int side = qMin (width (), height ());
    painter.scale (side / static_cast<qreal> (m_size), side / static_cast<qreal> (m_size));
    painter.setPen (blackPen);
    painter.rotate (m_roll);
    painter.setBrush (bgSky);

    int y = 0.25 * m_size * m_pitch / 20.;

    int x = sqrt (m_size * m_size / 4 - y * y);
    qreal gr = atan (static_cast<double> (y) / x);
    gr = gr * 180. / 3.1415926;
    painter.drawChord ( -side / 2, -side / 2, side, side, gr * 16, (180 - 2 * gr) * 16);
    painter.setBrush (bgGround);
    painter.drawChord (-side / 2, -side / 2, side, side, gr * 16, -(180 + 2 * gr) * 16);
    painter.setPen (whitePen);

    painter.drawLine (-x, -y, x, -y);
    painter.setPen (blackPen);
    painter.rotate (-180.);
    for(int i = 0;i < NUM_ROLL_LINES; i++)
    {
        painter.rotate (m_rollRotate[i]);
        painter.drawLine (m_rollPoint[i][0], m_rollPoint[i][1]);
    }

    whitePen.setWidth (1);
    painter.setPen (whitePen);
    painter.rotate (-90.);
    for(int i = 0;i < NUM_PITCH_LINES; i++)
    {
        painter.drawLine (m_pitchPoint[i][0], m_pitchPoint[i][1]);
    }
    painter.rotate (-m_roll);
    blackPen.setWidth (3);
    painter.setPen (blackPen);
    painter.drawLines (m_target);
    painter.drawLines (m_rollPointer);
}

void AttitudeIndicator::resizeEvent (QResizeEvent* _event)
{
    m_size = qMin(width (), height ());

    for(int i = 0; i < NUM_ROLL_LINES; i++)
        getRollLine (DEFAULTS_TYPE_ROLL[i], &m_rollPoint[i][0], &m_rollPoint[i][1]);

    for(int i = 0; i < NUM_PITCH_LINES; i++)
        getPitchLine (DEFAULTS_TYPE_PITCH[i], i, &m_pitchPoint[i][0], &m_pitchPoint[i][1]);

    resizeTargetChar ();
    resizeRollChar ();
}

void AttitudeIndicator::keyPressEvent (QKeyEvent* _event)
{
    switch (_event->key())
    {
        case Qt::Key_Left:
            //if(roll>-90.)
            m_roll -= 1.0;
            break;
        case Qt::Key_Right:
            //if(roll<90.)
            m_roll += 1.0;
            break;
        case Qt::Key_Down:
            //if(pitch>-20.)
            m_pitch -=1.0;
            break;
        case Qt::Key_Up:
            //if(pitch<20.)
            m_pitch +=1.0;
            break;
        default:
            break;
            //QFrame::keyPressEvent(event);
    }
    update ();
}

void AttitudeIndicator::getRollLine (ATTITUDE_LINE_TYPE _type, QPoint* _from, QPoint* _to)
{
    quint8 ofs = (_type == SMALL_ROLL_LINE) ? (m_size / 40) : (m_size / 20);
    _from->setY (m_size / 2 - ofs);
    _from->setX (0);
    _to->setY (m_size / 2);
    _to->setX (0);
}

void AttitudeIndicator::getPitchLine (ATTITUDE_LINE_TYPE _type, quint32 _index, QPoint* _from, QPoint* _to)
{
    int x,y;
    if (_index >= 4)
        _index++;
    y = m_size * (0.25 - _index * 0.0625);
    x = (_type == SMALL_PITCH_LINE) ? (3 * m_size / 32) : (5 * m_size / 32);
    _from->setX (-x);
    _from->setY (y);
    _to->setX (x);
    _to->setY (y);
}

void AttitudeIndicator::initTargetChar ()
{
    QLine line;
    line.setLine (-m_size / 4, 0, -m_size / 16, 0);
    m_target.append (line);
    line.setLine (-m_size / 16, 0, 0, -m_size / 32);
    m_target.append (line);
    line.setLine (0, -m_size / 32, m_size / 16, 0);
    m_target.append (line);
    line.setLine (m_size / 16, 0, m_size / 4,0);
    m_target.append (line);
}

void AttitudeIndicator::resizeTargetChar ()
{
    m_target.clear ();
    initTargetChar ();
}

void AttitudeIndicator::initRollChar ()
{
    QLine line;
    line.setLine (-m_size / 32, 14 * m_size / 32, 0, 15 * m_size / 32);
    m_target.append (line);
    line.setLine (0, 15 * m_size / 32, m_size / 32, 14 * m_size / 32);
    m_target.append (line);
    line.setLine (m_size / 32, 14 * m_size / 32, -m_size / 32, 14 * m_size / 32);
    m_target.append (line);
}

void AttitudeIndicator::resizeRollChar ()
{
    m_rollPointer.clear ();
    initRollChar ();
}
