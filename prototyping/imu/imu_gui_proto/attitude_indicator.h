#ifndef ATTITUDE_INDICATOR_H
#define ATTITUDE_INDICATOR_H

#include <QtGui>
#include <QtWidgets/QWidget>
#include <QDebug>

class AttitudeIndicator : public QWidget
{
    Q_OBJECT

public:
    typedef enum ATTITUDE_LINE_TYPE_ENUM
    {
        SMALL_ROLL_LINE = 0,
        NORMAL_ROLL_LINE,
        SMALL_PITCH_LINE,
        NORMAL_PITCH_LINE
    } ATTITUDE_LINE_TYPE;

    static const quint32            INDICATOR_SIZE_MAX = 600;
    static const quint32            INDICATOR_SIZE_MIN = 200;
    static const quint32            NUM_ROLL_LINES = 13;
    static const quint32            NUM_PITCH_LINES = 8;
    static const qreal              DEFAULTS_ROLL_ROTATE[];
    static const ATTITUDE_LINE_TYPE DEFAULTS_TYPE_ROLL[];
    static const ATTITUDE_LINE_TYPE DEFAULTS_TYPE_PITCH[];

    AttitudeIndicator (QWidget *parent = 0);
    ~AttitudeIndicator ();

    void setRoll (qreal _roll) {m_roll  = _roll;}
    void setPitch (qreal _val){m_pitch = _val;}
    qreal getRoll () const {return m_roll;}
    qreal getPitch () const {return m_pitch;}

    QSize size () const {return QSize(m_size, m_size);}
    QSize sizeHint () const {return QSize(m_size, m_size);}
protected:
    void paintEvent (QPaintEvent* _event);
    void resizeEvent (QResizeEvent* _event);
    void keyPressEvent (QKeyEvent* _event);

private:
    void getRollLine (ATTITUDE_LINE_TYPE _type, QPoint* _from, QPoint* _to);
    void getPitchLine (ATTITUDE_LINE_TYPE _type, quint32 _index, QPoint* _from, QPoint* _to);
    void initTargetChar ();
    void resizeTargetChar ();
    void initRollChar ();
    void resizeRollChar ();

    qint32          m_size;
    QPoint          m_rollPoint[NUM_ROLL_LINES][2];
    qreal           m_rollRotate[NUM_ROLL_LINES];
    QPoint          m_pitchPoint[NUM_PITCH_LINES][2];
    qreal           m_roll;
    qreal           m_pitch;
    QVector<QLine>  m_target;
    QVector<QLine>  m_rollPointer;
};

#endif // ATTITUDE_INDICATOR_H
