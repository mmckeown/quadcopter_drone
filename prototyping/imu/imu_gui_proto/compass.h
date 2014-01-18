#ifndef COMPASS_H
#define COMPASS_H

#include <QtGui>
#include <QWidget>
#include <QPainter>

class Compass : public QWidget
{
    Q_OBJECT
public:
    static const quint32 COMPASS_SIZE_MAX = 600;
    static const quint32 COMPASS_SIZE_MIN = 200;

    explicit Compass(QWidget *parent = 0);
    
    QSize size () const {return QSize (m_size, m_size);}
    bool getRotateNeedle () const {return m_rotateNeedle;}
    qint32 getDirection () const {return m_direction;}
public slots:
    void setDirection (qint32 _direction);
    void setRotateNeedle (bool _rotate);

protected:
    void paintEvent (QPaintEvent* _event);
    void resizeEvent (QResizeEvent* _event);
    void keyPressEvent (QKeyEvent* _event);

private:
    qint32          m_size;

    bool            m_rotateNeedle;
    qint32          m_direction;
};

#endif // COMPASS_H
