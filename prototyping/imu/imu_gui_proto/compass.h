#ifndef COMPASS_H
#define COMPASS_H

#include <QWidget>
#include <QPainter>

class Compass : public QWidget
{
    Q_OBJECT
public:
    static const quint32            COMPASS_SIZE_MAX = 600;
    static const quint32            COMPASS_SIZE_MIN = 200;

    explicit Compass(QWidget *parent = 0);
    
    QSize size () const {return QSize (m_size, m_size);}
protected:
    void paintEvent (QPaintEvent* _event);
    void resizeEvent (QResizeEvent* _event);

private:
    qint32          m_size;
    
};

#endif // COMPASS_H
